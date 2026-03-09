#include "esp_err.h"

esp_err_t bmp280_sensor_init(void);
esp_err_t bmp280_sensor_read( float *temperature, float *pressure,float *humidity);