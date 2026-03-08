#include "veml7700_sensor.h"
#include "veml7700_task.h"
#include "esp_err.h"


//static const char *TAG = "MAIN";

void app_main(void) {

    ESP_ERROR_CHECK(veml7700_sensor_init());

    veml7700_task_start();

}