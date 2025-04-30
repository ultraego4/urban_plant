#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "i2cdev.h"
#include "soc/gpio_num.h"
#include <esp_log.h>
#include <driver/i2c.h>


#include <veml7700.h>
#include <bmp280.h>


typedef struct {
  i2c_dev_t *dev;
  veml7700_config_t *config;
} veml7700_task_params_t;

#define I2C_PORT I2C_NUM_0
#define SDA_GPIO GPIO_NUM_13
#define SCL_GPIO GPIO_NUM_14



esp_err_t veml7700_setup(i2c_dev_t *dev,veml7700_config_t *config){


  //init device descriptor
  //default i2c address set by lib is 0x10
  ESP_ERROR_CHECK(veml7700_init_desc(dev, I2C_PORT, SDA_GPIO, SCL_GPIO));


  /*START SENSOR CONFIG*/

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


esp_err_t bmp280_setup(bmp280_t *dev){

  bmp280_params_t params;
  bmp280_init_default_params(&params);
  //bmp280_t dev = {0};

  // default i2c address 0x76
  ESP_ERROR_CHECK(bmp280_init_desc(dev, BMP280_I2C_ADDRESS_0, I2C_PORT, SDA_GPIO, SCL_GPIO));

  /*STAR SENSOR CONFIG*/

  // check if the device is available and configures the params
  esp_err_t err = bmp280_init(dev, &params);
  if (err == ESP_OK) {
      ESP_LOGI("BMP280", "Sensor probe and configuration is successful");
  } else {
      ESP_LOGE("BMP280", "Sensor failed: %s", esp_err_to_name(err));
      return err;
  }
  //check if aliexpress sensor is bme280
  ESP_LOGI("BMP280", "Detected chip ID: 0x%02X", dev->id);

  return ESP_OK;
  /*END SENSOR CONFIG*/

}

void bmp280_task(void *pvParameters){
  
  bmp280_t *dev = (bmp280_t *) pvParameters;

  float temp,pressure,humidity;

  while (1)
    {
      
      esp_err_t err = bmp280_read_float(dev, &temp, &pressure, &humidity);

      if (err == ESP_OK){
        ESP_LOGI("BMP280", "Temperature: %.2f C, Humidity: %.2f %%, Pressure: %.2f Pa", temp,humidity, pressure);
      } else {
        ESP_LOGE("BMP280", "Read failed: %s", esp_err_to_name(err));    
      }

      //1 sec delay
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}




void app_main(void) {
  esp_log_level_set("*", ESP_LOG_INFO);

  

  static i2c_dev_t veml7700_device = {0};


  /*******************VEML7700 SENSOR******************/


  static veml7700_config_t veml7700_config = {0};


  //init i2c, call this before everything
  //i2c -> sensor init -> sensor conf order matters!!!
  ESP_ERROR_CHECK(i2cdev_init());

  if(veml7700_setup(&veml7700_device,&veml7700_config) != ESP_OK){
    return;
  }

  static veml7700_task_params_t veml7700_task_params = {
    .dev = &veml7700_device,
    .config = &veml7700_config,
};

  ESP_LOGI("VEML7700", "Starting veml7700_task");
  if (xTaskCreate(veml7700_task, "veml7700_read_lux", 4096, &veml7700_task_params, 5, NULL) != pdPASS) 
  {
    ESP_LOGE("VEML7700", "Failed to create task!");
  }

/*******************BMP280 SENSOR******************/

  static bmp280_t bmp280_device = {0};

  if(bmp280_setup(&bmp280_device) != ESP_OK){
    return;
  }

  ESP_LOGI("BMP280", "Starting bmp280_task");
  if (xTaskCreate(bmp280_task, "bmp280_read_sensor", 8192, &bmp280_device, 5, NULL) != pdPASS) 
  {
    ESP_LOGE("BMP280", "Failed to create task!");
    
  }

}


  /* for (int i = 10; i >= 0; i--) {
    printf("Restarting in %d seconds...\n", i);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  printf("Restarting now.\n");
  fflush(stdout);
  esp_restart(); */
