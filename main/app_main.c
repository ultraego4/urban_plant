#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "i2cdev.h"

#include "sensors/veml7700_sensor.h"
#include "sensors/bmp280_sensor.h"

void app_main(void) {
    ESP_ERROR_CHECK(i2cdev_init());

    static veml7700_wrapper_t veml_sensor = {0};
    static bmp280_wrapper_t bmp_sensor = {0};

    if(veml7700_setup(&veml_sensor) != ESP_OK) return;
    if(bmp280_setup(&bmp_sensor) != ESP_OK) return;

    xTaskCreate(veml7700_task, "veml_task", 4096, &veml_sensor, 5, NULL);
    xTaskCreate(bmp280_task, "bmp_task", 8192, &bmp_sensor, 5, NULL);
}
