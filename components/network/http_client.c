#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "http_client.h"

static const char* TAG = "HTTP_CLIENT";

//hardcoded ;)
#define URL "http://192.168.1.103:5000/api/measurements"

void http_post_to_url(const char *payload)
{
    esp_http_client_config_t config = {
        .url = URL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");

    esp_http_client_set_post_field(client, payload, strlen(payload));

    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %lld",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }

    //esp_http_client_cleanup(client);
}

