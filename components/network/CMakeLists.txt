idf_component_register(SRCS "wifi_station.c" "http_client.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi nvs_flash esp_http_client)
