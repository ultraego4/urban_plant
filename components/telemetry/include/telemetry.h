#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

typedef struct {
    enum {
        SENSOR_BMP280,
        SENSOR_VEML7700
    } type;

    union {
        struct {
            float temperature;
            float pressure;
            float humidity;
        } bmp;

        struct {
            uint32_t lux;
        } veml;
    };
} sensor_data_t;

// queue handle to pass sensor readings between tasks
extern QueueHandle_t sensor_data_queue;


void telemetry_queue_init(void);