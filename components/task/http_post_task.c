#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "telemetry.h"
#include "http_client.h"

static const char *TAG = "HTTP_POST_TASK";

extern QueueHandle_t sensor_data_queue;

void http_post_task(void *arg)
{
    sensor_data_t msg;

static float temperature = 0;
static float humidity = 0;
static float pressure = 0;
static uint32_t lux = 0;

while (1)
{
    if (xQueueReceive(sensor_data_queue, &msg, portMAX_DELAY))
    {
        if (msg.type == SENSOR_BMP280) {
            temperature = msg.bmp.temperature;
            humidity = msg.bmp.humidity;
            pressure = msg.bmp.pressure;
        }

        if (msg.type == SENSOR_VEML7700) {
            lux = msg.veml.lux;
        }

        char payload[160];

        snprintf(payload, sizeof(payload),
            "{\"temperature\":%.2f,\"humidity\":%.2f,\"pressure\":%.2f,\"lux\":%lu}",
            temperature, humidity, pressure, lux);

        ESP_LOGI(TAG, "Sending: %s", payload);

        http_post_to_url(payload);
    }
}
}

void http_post_task_start(void)
{
    xTaskCreate(http_post_task, "HTTP_POST_TASK", 4096, NULL, 5, NULL);
}