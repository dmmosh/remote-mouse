#ifndef HEADER_H
#define HEADER_H


#include "esp_log.h"
#include "esp_hidd_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_bt.h"
#include "esp_err.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_gap_bt_api.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include <string.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define REPORT_PROTOCOL_MOUSE_REPORT_SIZE      (4)
#define REPORT_BUFFER_SIZE                     REPORT_PROTOCOL_MOUSE_REPORT_SIZE
#define LED 2

#define VRX ADC2_CHANNEL_4 // pin 13
#define VRY ADC2_CHANNEL_5 // pin 12
#define SW 14 
#define TRANSISTOR 27 
#define GPIO_OUTPUT_PINS ((1ULL << TRANSISTOR) | (1ULL << LED)) //outputs power to pin 27 
#define GPIO_INPUT_PINS ((1ULL<< SW))
#define ON 1
#define OFF 0

typedef struct {
    esp_hidd_app_param_t app_param;
    esp_hidd_qos_param_t both_qos;
    uint8_t protocol_mode;
    SemaphoreHandle_t mouse_mutex;
    TaskHandle_t mouse_task_hdl;
    uint8_t buffer[REPORT_BUFFER_SIZE];
    int8_t x_dir;
} local_param_t;

extern local_param_t s_local_param;
extern const char local_device_name[];
extern uint8_t hid_mouse_descriptor[];
extern const int hid_mouse_descriptor_len;

char *bda2str(esp_bd_addr_t bda, char *str, size_t size);
inline int same_sign(const int num, const int sign);
bool check_report_id_type(uint8_t report_id, uint8_t report_type);
void send_mouse_report(uint8_t buttons, char dx, char dy, char wheel, uint8_t protocol_mode);
void mouse_move_task(void *pvParameters);
void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
void bt_app_task_start_up(void);
void bt_app_task_shut_down(void);
void esp_bt_hidd_cb(esp_hidd_cb_event_t event, esp_hidd_cb_param_t *param);










#endif