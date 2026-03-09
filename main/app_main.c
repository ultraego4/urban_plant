#include "esp_err.h"
#include "nvs_flash.h"

#include "wifi_station.h"

#include "veml7700_sensor.h"
#include "veml7700_task.h"

#include "bmp280_sensor.h"
#include "bmp280_task.h"

#include "telemetry.h"

#include "http_post_task.h"



//static const char *TAG = "MAIN";

void app_main(void) {

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_station_init();

    telemetry_queue_init(); 


    /*SENSORS*/

    ESP_ERROR_CHECK(veml7700_sensor_init());
    ESP_ERROR_CHECK(bmp280_sensor_init());

    http_post_task_start();

    veml7700_task_start();
    bmp280_task_start();



}