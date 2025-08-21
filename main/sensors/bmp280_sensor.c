#include "bmp280_sensor.h"
#include "esp_log.h"
#include <stdio.h>
#include "esp_log.h"
#include <driver/i2c.h>
#include <bmp280.h>
#include "i2c_config.h"

#define I2C_PORT I2C_NUM_0
#define SDA_GPIO GPIO_NUM_4
#define SCL_GPIO GPIO_NUM_5

esp_err_t bmp280_setup(bmp280_wrapper_t *sensor) {
  bmp280_params_t params;
  bmp280_init_default_params(&params);

  // default i2c address 0x76 in lib
  ESP_ERROR_CHECK(bmp280_init_desc(&sensor->dev, BMP280_I2C_ADDRESS_0, I2C_PORT,
                                   SDA_GPIO, SCL_GPIO));

  // check if the device is available and configures the params
  esp_err_t err = bmp280_init(&sensor->dev, &params);

  if (err == ESP_OK) {
    ESP_LOGI("BMP280", "Sensor probe successful, ID: 0x%02X", sensor->dev.id);
  } else {
    ESP_LOGE("BMP280", "Sensor probe failed: %s", esp_err_to_name(err));
  }

  // check if aliexpress sensor is bme280
  ESP_LOGI("BMP280", "Detected chip ID: 0x%02X", sensor->dev.id);

  return err;
}

void bmp280_task(void *pvParameters) {
  bmp280_wrapper_t *sensor = (bmp280_wrapper_t *)pvParameters;
  float temp, pressure, humidity;

  while (1) {
    if (bmp280_read_float(&sensor->dev, &temp, &pressure, &humidity) ==
        ESP_OK) {
      printf("BMP280,%.2f,%.2f,%.2f\n", temp, humidity, pressure);
    }
    // 5*60*
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
