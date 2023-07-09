#include "ds_adc.h"

/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"


#define DEFAULT_VREF 1100 // Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 64  // 多重采样次数

static esp_adc_cal_characteristics_t *adc_chars;
#if CONFIG_IDF_TARGET_ESP32
static const adc_channel_t channel = ADC_CHANNEL_6; // GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
#elif CONFIG_IDF_TARGET_ESP32S2
static const adc_channel_t channel = ADC_CHANNEL_6; // GPIO7 if ADC1, GPIO17 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_13;
#endif
static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;

// voltage to raw data struct
typedef struct
{
    float vol;
    uint32_t raw;
} vol_to_raw_t;

/* Set thresholds, approx. 2.00V - 3.30V */
#define VAL_TO_RAW_NUM 14
static const vol_to_raw_t g_v0l_to_raw[VAL_TO_RAW_NUM] = {
    {2.00, 3123},
    {2.10, 3207},
    {2.20, 3301},
    {2.27, 3361},
    {2.35, 3434},
    {2.43, 3515},
    {2.52, 3598},
    {2.60, 3682},
    {2.68, 3745},
    {2.76, 3825},
    {2.80, 3867},
    {2.90, 3891},
    {3.00, 3986},
    {3.30, 4095},
};

static void check_efuse(void)
{
#if CONFIG_IDF_TARGET_ESP32
    // Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("eFuse Two Point: Supported\n");
    }
    else
    {
        printf("eFuse Two Point: NOT supported\n");
    }
    // Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
    {
        printf("eFuse Vref: Supported\n");
    }
    else
    {
        printf("eFuse Vref: NOT supported\n");
    }
#elif CONFIG_IDF_TARGET_ESP32S2
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
    {
        printf("eFuse Two Point: Supported\n");
    }
    else
    {
        printf("Cannot retrieve eFuse Two Point calibration values. Default calibration values will be used.\n");
    }
#else
#error "This example is configured for ESP32/ESP32S2."
#endif
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
    {
        printf("Characterized using Two Point Value\n");
    }
    else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
    {
        printf("Characterized using eFuse Vref\n");
    }
    else
    {
        printf("Characterized using Default Vref\n");
    }
}

static float ulp_adc_raw_to_vol(uint32_t adc_raw)
{
    if (adc_raw < g_v0l_to_raw[0].raw || adc_raw > g_v0l_to_raw[VAL_TO_RAW_NUM - 1].raw)
    {
        return 0;
    }

    float ret_vol = 0;
    for (int i = 0; i < VAL_TO_RAW_NUM - 1; i++)
    {
        if (g_v0l_to_raw[i].raw <= adc_raw && g_v0l_to_raw[i + 1].raw >= adc_raw)
        {
            uint32_t raw_dif = g_v0l_to_raw[i + 1].raw - g_v0l_to_raw[i].raw;
            float vol_dif = g_v0l_to_raw[i + 1].vol - g_v0l_to_raw[i].vol;
            ret_vol = g_v0l_to_raw[i].vol + (float)(adc_raw - g_v0l_to_raw[i].raw) / (float)raw_dif * vol_dif;
            break;
        }
    }
    return ret_vol;
}

uint32_t getBatteryLevel(void)
{
    uint32_t adc_reading = 0;
    // Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++)
    {
        if (unit == ADC_UNIT_1)
        {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        else
        {
            int raw;
            adc2_get_raw((adc2_channel_t)channel, width, &raw);
            adc_reading += raw;
        }
    }
    adc_reading /= NO_OF_SAMPLES; // 取多重采样的平均值
    // Convert adc_reading to voltage in mV
    uint32_t voltage = 0;
    if (adc_reading < 3123)
    { // 如果电压小于2.00, 3123
        voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    }
    else
    {
        float voltage_f = ulp_adc_raw_to_vol(adc_reading) * 1000;
        voltage = (uint32_t)voltage_f;
    }
    printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
    return voltage;
}

void ds_batteryLevelMonitor_init(void)
{
    // Check if Two Point or Vref are burned into eFuse
    check_efuse();

    // Configure ADC
    if (unit == ADC_UNIT_1)
    {
        adc1_config_width(width);
        adc1_config_channel_atten(channel, atten);
    }
    else
    {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    // Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}