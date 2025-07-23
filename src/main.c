/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 * 
 * 
 * 
 *  pio run --target upload; git-all; pio device monitor -b 115200
 */
#include "header.h"


const char local_device_name[] = "Remote Mouse";

 
local_param_t s_local_param = {0};
 
 // HID report descriptor for a generic mouse. The contents of the report are:
 // 3 buttons, moving information for X and Y cursors, information for a wheel.
uint8_t hid_mouse_descriptor[] = {
     0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
     0x09, 0x02,                    // USAGE (Mouse)
     0xa1, 0x01,                    // COLLECTION (Application)
 
     0x09, 0x01,                    //   USAGE (Pointer)
     0xa1, 0x00,                    //   COLLECTION (Physical)
 
     0x05, 0x09,                    //     USAGE_PAGE (Button)
     0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
     0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
     0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
     0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
     0x95, 0x03,                    //     REPORT_COUNT (3)
     0x75, 0x01,                    //     REPORT_SIZE (1)
     0x81, 0x02,                    //     INPUT (Data,Var,Abs)
     0x95, 0x01,                    //     REPORT_COUNT (1)
     0x75, 0x05,                    //     REPORT_SIZE (5)
     0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
 
     0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
     0x09, 0x30,                    //     USAGE (X)
     0x09, 0x31,                    //     USAGE (Y)
     0x09, 0x38,                    //     USAGE (Wheel)
     0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
     0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
     0x75, 0x08,                    //     REPORT_SIZE (8)
     0x95, 0x03,                    //     REPORT_COUNT (3)
     0x81, 0x06,                    //     INPUT (Data,Var,Rel)
 
     0xc0,                          //   END_COLLECTION
     0xc0                           // END_COLLECTION
 };


 

 const int hid_mouse_descriptor_len = sizeof(hid_mouse_descriptor);

 
 void app_main(void)
 {
     const char *TAG = "app_main";
     esp_err_t ret;
     char bda_str[18] = {0};
 
     ret = nvs_flash_init();
     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
         ESP_ERROR_CHECK(nvs_flash_erase());
         ret = nvs_flash_init();
     }
     ESP_ERROR_CHECK( ret );
 
     ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
 
     esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
     if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
         ESP_LOGE(TAG, "initialize controller failed: %s", esp_err_to_name(ret));
         return;
     }
 
     if ((ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM)) != ESP_OK) {
         ESP_LOGE(TAG, "enable controller failed: %s", esp_err_to_name(ret));
         return;
     }
 
     esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
     bluedroid_cfg.ssp_en = true;
     if ((ret = esp_bluedroid_init_with_cfg(&bluedroid_cfg)) != ESP_OK) {
         ESP_LOGE(TAG, "%s initialize bluedroid failed: %s", __func__, esp_err_to_name(ret));
         return;
     }
 
     if ((ret = esp_bluedroid_enable()) != ESP_OK) {
         ESP_LOGE(TAG, "enable bluedroid failed: %s", esp_err_to_name(ret));
         return;
     }
 
     if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
         ESP_LOGE(TAG, "gap register failed: %s", esp_err_to_name(ret));
         return;
     }
 
     ESP_LOGI(TAG, "setting device name");
     esp_bt_gap_set_device_name(local_device_name);
 
     ESP_LOGI(TAG, "setting cod major, peripheral");
     esp_bt_cod_t cod = {0};
     cod.major = ESP_BT_COD_MAJOR_DEV_PERIPHERAL;
     cod.minor = 0x20;
     esp_bt_gap_set_cod(cod, ESP_BT_SET_COD_MAJOR_MINOR);
 
     vTaskDelay(2000 / portTICK_PERIOD_MS);
 
     // Initialize HID SDP information and L2CAP parameters.
     // to be used in the call of `esp_bt_hid_device_register_app` after profile initialization finishes
     do {
         s_local_param.app_param.name = "Mouse";
         s_local_param.app_param.description = "Mouse Example";
         s_local_param.app_param.provider = "ESP32";
         s_local_param.app_param.subclass = ESP_HID_CLASS_MIC; // keep same with minor class of COD
         s_local_param.app_param.desc_list = hid_mouse_descriptor;
         s_local_param.app_param.desc_list_len = hid_mouse_descriptor_len;
 
         memset(&s_local_param.both_qos, 0, sizeof(esp_hidd_qos_param_t)); // don't set the qos parameters
     } while (0);
 
     // Report Protocol Mode is the default mode, according to Bluetooth HID specification
     s_local_param.protocol_mode = ESP_HIDD_REPORT_MODE;
 
     ESP_LOGI(TAG, "register hid device callback");
     esp_bt_hid_device_register_callback(esp_bt_hidd_cb);
 
     ESP_LOGI(TAG, "starting hid device");
     esp_bt_hid_device_init();

     /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_NONE;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
 
     /*
      * Set default parameters for Legacy Pairing
      * Use variable pin, input pin code when pairing
      */
     esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
     esp_bt_pin_code_t pin_code;
     esp_bt_gap_set_pin(pin_type, 0, pin_code);

 
     ESP_LOGI(TAG, "Own address:[%s]", bda2str((uint8_t *)esp_bt_dev_get_address(), bda_str, sizeof(bda_str)));
     ESP_LOGI(TAG, "exiting");
 }