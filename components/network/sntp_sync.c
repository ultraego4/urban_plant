#include "esp_sntp.h"
#include <time.h>
#include "esp_log.h"

static const char* TAG = "SNTP";

void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");  // You can add more servers
    esp_sntp_init();
}

void wait_for_sntp_sync(void)
{
    ESP_LOGI(TAG, "Waiting for time to be set...");
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;

    while (timeinfo.tm_year < (2026 - 1900) && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for NTP... (%d/%d)", retry, retry_count);
        vTaskDelay(pdMS_TO_TICKS(2000));
        time(&now);
        localtime_r(&now, &timeinfo);
    }

    if (timeinfo.tm_year < (2026 - 1900)) {
        ESP_LOGW(TAG, "Failed to get NTP time, using fallback");
    } else {
        //continue with invalid fallback time ://
        ESP_LOGI(TAG, "Time synchronized: %s", asctime(&timeinfo));
    }
}