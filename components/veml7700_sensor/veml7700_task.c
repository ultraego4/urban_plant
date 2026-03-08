#include "veml7700_task.h"
#include "veml7700_sensor.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "VEML7700_TASK";

static void veml7700_task(void *arg) {
    uint32_t als;
    while (1) {

        if (veml7700_sensor_read(&als) == ESP_OK) {
            ESP_LOGI(TAG,"ALS: %u lx", als);
        } else {
            ESP_LOGW(TAG, "Sensor read failed!");
        }

        //poll every 1s
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void veml7700_task_start(void) {
    xTaskCreate(veml7700_task, "VEML7700_TASK", 2048, NULL, 5, NULL);
}