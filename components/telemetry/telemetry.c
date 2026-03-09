#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "esp_err.h"
#include "esp_log.h"

#include "telemetry.h"


static const char* TAG = "TELEMETRY";

QueueHandle_t sensor_data_queue = NULL;

void telemetry_queue_init(void)
{
    // queue length, queue size
    sensor_data_queue = xQueueCreate(10, sizeof(sensor_data_t));

}