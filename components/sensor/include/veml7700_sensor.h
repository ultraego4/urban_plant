#include "esp_err.h"
#include <stdint.h>


esp_err_t veml7700_sensor_init(void);
esp_err_t veml7700_sensor_read(uint32_t *als);