#include "veml7700_sensor.h"
#include "esp_log.h"
#include <driver/i2c.h>
#include <veml7700.h>

#include "i2c_config.h"

esp_err_t veml7700_setup(veml7700_wrapper_t *sensor) {

    // init device descriptor
    // default i2c address set by lib is 0x10
    ESP_ERROR_CHECK(veml7700_init_desc(&sensor->dev, I2C_PORT, SDA_GPIO, SCL_GPIO));

    // gain to the lowest level to prevent saturation in direct sunlight over the
    // 0..120,000 lux
    sensor->config.gain = VEML7700_GAIN_DIV_8;

    // moderate integration time
    // longer integration time allows the sensor to accumulate more light, can
    // lead to saturation shorter integration times mean the sensor collects light
    // data for a shorter period, avoid saturation
    sensor->config.integration_time = VEML7700_INTEGRATION_TIME_100MS;

    // helps stabilize readings and reduce noise,
    sensor->config.persistence_protect = VEML7700_PERSISTENCE_PROTECTION_4;

    // disable interrupt, not needed for continuous measurement
    sensor->config.interrupt_enable = 0;

    // sensor is kept active during measurements
    sensor->config.shutdown = 0;

    // disable power saving mode since its not needed when using USB power
    sensor->config.power_saving_enable = 0;

    // write the config to the device
    ESP_ERROR_CHECK(veml7700_set_config(&sensor->dev, &sensor->config));


    // check if the device is available
    esp_err_t err = veml7700_probe(&sensor->dev);
    if (err == ESP_OK) {
        ESP_LOGI("VEML7700", "Sensor probe successful");
    } else {
        ESP_LOGE("VEML7700", "Sensor probe failed: %s", esp_err_to_name(err));
    }
    return err;
}

void veml7700_task(void *pvParameters){
    
    veml7700_wrapper_t *sensor = (veml7700_wrapper_t *)pvParameters;
    uint32_t lux;

    while(1){
        if(veml7700_get_ambient_light(&sensor->dev, &sensor->config, &lux) == ESP_OK){
            printf("VEML7700,%u\n", (unsigned int)lux);
        }
        // 5*60
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
