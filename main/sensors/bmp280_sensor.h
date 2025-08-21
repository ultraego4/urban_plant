#pragma once
#include <bmp280.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
    bmp280_t dev;
} bmp280_wrapper_t;

void bmp280_task(void *pvParameters);
esp_err_t bmp280_setup(bmp280_wrapper_t *sensor);
