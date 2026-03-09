#include "esp_err.h"

#include "veml7700_sensor.h"
#include "veml7700_task.h"

#include "bmp280_sensor.h"
#include "bmp280_task.h"


//static const char *TAG = "MAIN";

void app_main(void) {

    ESP_ERROR_CHECK(veml7700_sensor_init());
    ESP_ERROR_CHECK(bmp280_sensor_init());

    veml7700_task_start();
    bmp280_task_start();

}