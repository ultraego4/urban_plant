#pragma once
#include "veml7700.h"
#include "i2cdev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct {
    i2c_dev_t dev;
    veml7700_config_t config;
} veml7700_wrapper_t;

void veml7700_task(void *pvParameters);
esp_err_t veml7700_setup(veml7700_wrapper_t *sensor);
