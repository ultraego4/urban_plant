#include <stdint.h>
#include <string.h>

#include "esp_err.h"
#include "esp_log.h"

#include "veml7700.h"
#include "i2cdev.h"


//TODO move this out
//I2C setup
#define SDA_GPIO_NUM 18
#define SCL_GPIO_NUM 19


static const char *TAG = "VEML7700_SENSOR";

static i2c_dev_t veml7700_device;
static veml7700_config_t veml7700_configuration;

esp_err_t veml7700_sensor_init(void){

    //zero out garbage memory of structs
    memset(&veml7700_device,0,sizeof(i2c_dev_t));
    memset(&veml7700_configuration,0,sizeof(veml7700_config_t));

    //always init i2cdev before using it
    ESP_LOGI(TAG, "Initializing I2C...");
    ESP_ERROR_CHECK(i2cdev_init());

    ESP_LOGI(TAG, "Initializing hardware...");

    //init the device struct
    ESP_LOGI(TAG, "Setting up device descriptor...");
    ESP_ERROR_CHECK(veml7700_init_desc(&veml7700_device, I2C_NUM_0, SDA_GPIO_NUM, SCL_GPIO_NUM));

    //check if the device is available
    ESP_LOGI(TAG, "Probing VEML7700 sensor...");
    ESP_ERROR_CHECK(veml7700_probe(&veml7700_device));

    /*START CONFIG*/

    // gain to the lowest level to prevent saturation in direct sunlight over the
    // 0..120,000 lux
    veml7700_configuration.gain = VEML7700_GAIN_DIV_8;
    
    // moderate integration time
    // longer integration time allows the sensor to accumulate more light, can
    // lead to saturation shorter integration times mean the sensor collects light
    // data for a shorter period, avoid saturation
    veml7700_configuration.integration_time = VEML7700_INTEGRATION_TIME_100MS;

    // helps stabilize readings and reduce noise,
    veml7700_configuration.persistence_protect = VEML7700_PERSISTENCE_PROTECTION_4;

    // disable interrupt, not needed for continuous measurement
    veml7700_configuration.interrupt_enable = 0;

    // sensor is kept active during measurements
    veml7700_configuration.shutdown = 0;

    // disable power saving mode since its not needed when using USB power
    veml7700_configuration.power_saving_enable = 0;

    //veml7700_configuration.power_saving_mode = VEML7700_POWER_SAVING_MODE_1000MS;


    /*END CONFIG*/

    //write the config to the device
    ESP_LOGI(TAG, "Writing sensor configuration...");
    esp_err_t err = veml7700_set_config(&veml7700_device, &veml7700_configuration);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "VEML7700 sensor initialized successfully");
    } else {
        ESP_LOGE(TAG, "Failed to configure VEML7700: %d", err);
    }

    return err;

}

esp_err_t veml7700_sensor_read(uint32_t *als){
    return veml7700_get_ambient_light(&veml7700_device, &veml7700_configuration, als);
}


