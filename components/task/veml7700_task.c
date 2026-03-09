#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "veml7700_sensor.h"
#include "veml7700_task.h"
#include "telemetry.h"

static const char *TAG = "VEML7700_TASK";

extern QueueHandle_t sensor_data_queue;

static void veml7700_task(void *arg) {

    static sensor_data_t msg = {0};
    uint32_t als;

    //wait 1 sec before first read so the sensor has time to init
    //there was invalid reading on the first tries when testing
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1) {
        if (veml7700_sensor_read(&als) == ESP_OK) {
            ESP_LOGI(TAG,"ALS: %u lx", als);

            msg.type = SENSOR_VEML7700;
            msg.veml.lux = als;

            xQueueSend(sensor_data_queue, &msg, 0);
        }
        else{
            ESP_LOGW(TAG, "Sensor read failed!");
        }
        //poll every 1 minutes
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}


void veml7700_task_start(void) {
    xTaskCreate(veml7700_task, "VEML7700_TASK", 2048, NULL, 5, NULL);
}