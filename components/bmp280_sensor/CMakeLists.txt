idf_component_register(
    SRCS "bmp280_sensor.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp-idf-lib__bmp280
)
