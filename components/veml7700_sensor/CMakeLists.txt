idf_component_register(
    SRCS "veml7700_sensor.c" "veml7700_task.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp-idf-lib__veml7700 
)

