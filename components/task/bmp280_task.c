#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "bmp280_sensor.h"
#include "bmp280_task.h"
#include "telemetry.h"

static const char *TAG = "BMP280_TASK";
extern QueueHandle_t sensor_data_queue;


static void bmp280_task(void *arg) {
    static sensor_data_t msg = {0};

    float temperature, pressure, humidity;

    while (1) {
        if (bmp280_sensor_read(&temperature, &pressure, &humidity) == ESP_OK) {
            ESP_LOGI(TAG," Temperature: %.2f C, Pressure: %.2f Pa, Humidity: %.2f",temperature,pressure,humidity);
            
            msg.type = SENSOR_BMP280;
            msg.bmp.temperature = temperature;
            msg.bmp.pressure = pressure;
            msg.bmp.humidity = humidity;

            xQueueSend(sensor_data_queue, &msg, 0);
        }
        else{
            ESP_LOGW(TAG, "Sensor read failed!");
        }
        //poll every 1 minutes
        vTaskDelay(pdMS_TO_TICKS(60000));
    }
}


void bmp280_task_start(void) {
    xTaskCreate(bmp280_task, "BMP280_TASK", 2048, NULL, 5, NULL);
}