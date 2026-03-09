#include "esp_err.h"
#include "esp_log.h"

#include "bmp280.h"


//TODO move this out
//I2C setup
#define SDA_GPIO_NUM 18
#define SCL_GPIO_NUM 19


static const char *TAG = "BMP280_SENSOR";

static bmp280_t dev;
static bmp280_params_t params;


esp_err_t bmp280_sensor_init(void){

    //zero out garbage memory of structs
    memset(&dev, 0, sizeof(bmp280_t));
    memset(&params, 0, sizeof(bmp280_params_t)); 

    //set configuration, use default
    ESP_ERROR_CHECK(bmp280_init_default_params(&params));   

    //init the device struct
    ESP_LOGI(TAG, "Setting up device descriptor...");
    ESP_ERROR_CHECK(bmp280_init_desc(&dev, BMP280_I2C_ADDRESS_0, 0, SDA_GPIO_NUM, SCL_GPIO_NUM));

    ESP_LOGI(TAG, "Writing sensor configuration...");
    esp_err_t err = bmp280_init(&dev, &params);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "BMP280 sensor initialized successfully");
    } else {
        ESP_LOGE(TAG, "Failed to configure BMP280: %d", err);
    }

    return err;
}

esp_err_t bmp280_sensor_read( float *temperature,float *pressure, float *humidity){
    
    return bmp280_read_float(&dev, temperature, pressure, humidity);
}