#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "bmp280_sensor.h"

static const char *TAG = "BMP280_TASK";

static void bmp280_task(void *arg) {
    
    float temperature,pressure,humidity;

    while (1) {

        if (bmp280_sensor_read(&temperature,&pressure,&humidity) == ESP_OK) {
            ESP_LOGI(TAG," Temperature: %.2f C, Pressure: %.2f Pa, Humidity: %.2f",temperature,pressure,humidity);
        } else {
            ESP_LOGW(TAG, "Sensor read failed!");
        }

        //poll every 1s
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void bmp280_task_start(void) {
    xTaskCreate(bmp280_task, "BMP280_TASK", 2048, NULL, 5, NULL);
}