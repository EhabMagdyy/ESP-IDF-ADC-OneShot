/***************************************************************************************************************
***************************    Author  : Ehab Magdy Abdullah                      ******************************
***************************    Linkedin: https://www.linkedin.com/in/ehabmagdyy/  ******************************
***************************    Youtube : https://www.youtube.com/@EhabMagdyy      ******************************
****************************************************************************************************************/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

// Configuration
#define ADC_PIN             ADC_CHANNEL_6       // GPIO34 on ESP32
#define ADC_UNIT            ADC_UNIT_1          // ADC1
#define TAG                 "ADC"               // For logging

void ADC_Task(void *pvParameters);

void app_main(void)
{
    xTaskCreate(ADC_Task, "ADC_Task", 2048, NULL, 5, NULL);
}

void ADC_Task(void *pvParameters)
{
    int adc_value;
    adc_oneshot_unit_handle_t adc_handle;

    // Initialize ADC oneshot driver
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    // Configure ADC channel
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,       // 12-bit resolution (0-4095)
        .atten = ADC_ATTEN_DB_12,          // ~3.3V full-scale voltage
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_PIN, &config));

    for(;;)
    {
        // Read ADC value
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_PIN, &adc_value));
        // Log ADC value on the monitor
        ESP_LOGI(TAG, "ADC Value: %d", adc_value);
        // Wait for 1 second (Task enteres the waiting state)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}