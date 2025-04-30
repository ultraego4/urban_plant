#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2cdev.h"


#include <esp_log.h>
#include <driver/i2c.h>

#include <stdint.h>
#include <veml7700.h>


typedef struct {
  i2c_dev_t *dev;
  veml7700_config_t *config;
} veml7700_task_params_t;

#define I2C_PORT I2C_NUM_0
#define SDA_GPIO GPIO_NUM_13
#define SCL_GPIO GPIO_NUM_14



esp_err_t veml7700_init(i2c_dev_t *dev,veml7700_config_t *config){

  //i2c -> sensor init -> sensor conf order matters!!!

  //init i2c, call this before everything
  ESP_ERROR_CHECK(i2cdev_init());

  //init device descriptor
  ESP_ERROR_CHECK(veml7700_init_desc(dev, I2C_PORT, SDA_GPIO, SCL_GPIO));


  /*SENSOR CONFIG*/

  //gain to the lowest level to prevent saturation in direct sunlight over the 0..120,000 lux
  config->gain = VEML7700_GAIN_DIV_8;

  //moderate integration time
  //longer integration time allows the sensor to accumulate more light, can lead to saturation
  //shorter integration times mean the sensor collects light data for a shorter period, avoid saturation
  config->integration_time = VEML7700_INTEGRATION_TIME_100MS;

  //helps stabilize readings and reduce noise,
  config->persistence_protect = VEML7700_PERSISTENCE_PROTECTION_4;

  //disable interrupt, not needed for continuous measurement
  config->interrupt_enable = 0;

  //sensor is kept active during measurements
  config->shutdown = 0;

  //disable power saving mode since its not needed when using USB power
  config->power_saving_enable = 0;

  //write the config to the device
  ESP_ERROR_CHECK(veml7700_set_config(dev, config));

  /*END SENSOR CONFIG*/

  // check if the device is available
  esp_err_t err = veml7700_probe(dev);
  if (err == ESP_OK) {
      ESP_LOGI("VEML7700", "Sensor probe successful");
  } else {
      ESP_LOGE("VEML7700", "Sensor probe failed: %s", esp_err_to_name(err));
      return err;
  }

  return ESP_OK;
}


void veml7700_task(void *pvParameters){

  veml7700_task_params_t *params = (veml7700_task_params_t *) pvParameters;

  i2c_dev_t *dev = params->dev;
  veml7700_config_t *config = params->config;
  uint32_t lux;

  while (1) {
    esp_err_t err= veml7700_get_ambient_light(dev, config, &lux);
    if (err == ESP_OK){
      ESP_LOGI("VEML7700", "Lux: %u", lux);
    } else {
      ESP_LOGE("VEML7700", "Read failed: %s", esp_err_to_name(err));    
    }

    //1 sec delay
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }

}

void app_main(void) {
  esp_log_level_set("*", ESP_LOG_INFO);

  static i2c_dev_t veml7700_device = {0};
  static veml7700_config_t veml7700_config = {0};

  if(veml7700_init(&veml7700_device,&veml7700_config) != ESP_OK){
    return;
  }

  static veml7700_task_params_t veml7700_task_params = {
    .dev = &veml7700_device,
    .config = &veml7700_config,
};

  ESP_LOGI("VEML7700", "Starting veml7700_task");
  if (xTaskCreate(veml7700_task, "veml7700_read_lux", 2048, &veml7700_task_params, 5, NULL) != pdPASS) 
  {
    ESP_LOGE("VEML7700", "Failed to create task!");
  }

}


  /* for (int i = 10; i >= 0; i--) {
    printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart(); */
