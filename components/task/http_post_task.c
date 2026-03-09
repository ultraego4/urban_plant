#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "telemetry.h"
#include "http_client.h"

#include "time.h" 


static const char *TAG = "HTTP_POST_TASK";

extern QueueHandle_t sensor_data_queue;

static void get_iso_timestamp(char *buf, size_t len)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(buf, len, "%Y-%m-%dT%H:%M:%S", &timeinfo);
}

void http_post_task(void *arg)
{
    sensor_data_t msg;

    static float temperature = 0;
    static float humidity = 0;
    static float pressure = 0;
    static uint32_t lux = 0;

    static bool bmp_ready = false;
    static bool veml_ready = false;

    char timestamp[32];
    char payload[256];

    while (1)
    {
        if (!xQueueReceive(sensor_data_queue, &msg, portMAX_DELAY)) {
            continue;
        }

        switch (msg.type) {
            case SENSOR_BMP280:
                temperature = msg.bmp.temperature;
                humidity = msg.bmp.humidity;
                pressure = msg.bmp.pressure;
                bmp_ready = true;
                break;

            case SENSOR_VEML7700:
                lux = msg.veml.lux;
                veml_ready = true;
                break;
        }

        if (bmp_ready && veml_ready)
        {
            get_iso_timestamp(timestamp, sizeof(timestamp));

            snprintf(payload, sizeof(payload),
                     "{\"timestamp\":\"%s\",\"temperature\":%.2f,\"humidity\":%.2f,\"pressure\":%.2f,\"lux\":%lu}",
                     timestamp, temperature, humidity, pressure, lux);

            ESP_LOGI(TAG, "Sending: %s", payload);
            http_post_to_url(payload);

            //reset flags
            bmp_ready = false;
            veml_ready = false;
        }
    }
}

void http_post_task_start(void)
{
    xTaskCreate(http_post_task, "HTTP_POST_TASK", 4096, NULL, 5, NULL);
}