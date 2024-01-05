/**
 * @file      UnitTest.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 * @note      Arduino Setting
 *            Tools ->
 *                  Board:"ESP32S3 Dev Module"
 *                  USB CDC On Boot:"Enable"
 *                  USB DFU On Boot:"Disable"
 *                  Flash Size : "16MB(128Mb)"
 *                  Flash Mode"QIO 80MHz
 *                  Partition Scheme:"16M Flash(3M APP/9.9MB FATFS)"
 *                  PSRAM:"OPI PSRAM"
 *                  Upload Mode:"UART0/Hardware CDC"
 *                  USB Mode:"Hardware CDC and JTAG"
 *
 *      https://lvgl.io/tools/imageconverter
 */
#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <SD.h>
#include "es7210.h"
#include <Audio.h>
#include <driver/i2s.h>
#include <esp_vad.h>

#define TOUCH_MODULES_GT911
#include "TouchLib.h"
#include "utilities.h"
#include <AceButton.h>

#include "PalcomFS.h"


using namespace ace_button;

#define USING_SX1262

#define VAD_SAMPLE_RATE_HZ              16000
#define VAD_FRAME_LENGTH_MS             30
#define VAD_BUFFER_LENGTH               (VAD_FRAME_LENGTH_MS * VAD_SAMPLE_RATE_HZ / 1000)
#define I2S_CH                          I2S_NUM_1

LV_IMG_DECLARE(palcomLogo)
LV_IMG_DECLARE(image1);
LV_IMG_DECLARE(image2);
LV_IMG_DECLARE(image3);
LV_IMG_DECLARE(image4);
LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/

TouchLib *touch = NULL;

#ifdef USING_SX1262
#define RADIO_FREQ          868.0
SX1262 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
#else
#define RADIO_FREQ          433.0
SX1268 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
#endif

void handleEvent(AceButton * /* button */, uint8_t eventType,
                 uint8_t /* buttonState */);

TFT_eSPI        tft;
Audio           audio;
size_t          bytes_read;
uint8_t         status;
int16_t         *vad_buff;
vad_handle_t    vad_inst;
TaskHandle_t    playHandle = NULL;
TaskHandle_t    radioHandle = NULL;

AceButton   button;
bool        clicked = false;
bool        txFlag =  true;
bool        rxFlag = false;
bool        transmissionFlag = true;
bool        enableInterrupt = true;
bool        transmitting = false;
int         transmissionState ;
bool        hasRadio = false;
bool        touchDected = false;
bool        kbDected = false;
bool        sender = true;
uint32_t    sendCount = 0;
uint32_t    runningMillis = 0;
uint8_t     touchAddress = GT911_SLAVE_ADDRESS2;

lv_indev_t  *kb_indev = NULL;
lv_indev_t  *mouse_indev = NULL;
lv_indev_t  *touch_indev = NULL;
lv_group_t  *kb_indev_group;
lv_obj_t    *hw_ta;
lv_obj_t    *radio_ta;
lv_obj_t    *tv ;
SemaphoreHandle_t xSemaphore = NULL;
#define GLOBAL_GUI_OBJECT_COUNT 10
lv_obj_t *globalGuiObjects[GLOBAL_GUI_OBJECT_COUNT] = {NULL};

// Palcom includes
#include "tools/tools.h"
#include "styles/styles.h"
#include "objects/objects.h"
#include "screens/screens.h"
#include "palcomCore.class.h"

bool setupCoder()
{
    uint32_t ret_val = ESP_OK;

    Wire.beginTransmission(ES7210_ADDR);
    uint8_t error = Wire.endTransmission();
    if (error != 0) {
        Serial.println("ES7210 address not found"); return false;
    }

    audio_hal_codec_config_t cfg = {
        .adc_input = AUDIO_HAL_ADC_INPUT_ALL,
        .codec_mode = AUDIO_HAL_CODEC_MODE_ENCODE,
        .i2s_iface =
        {
            .mode = AUDIO_HAL_MODE_SLAVE,
            .fmt = AUDIO_HAL_I2S_NORMAL,
            .samples = AUDIO_HAL_16K_SAMPLES,
            .bits = AUDIO_HAL_BIT_LENGTH_16BITS,
        },
    };

    ret_val |= es7210_adc_init(&Wire, &cfg);
    ret_val |= es7210_adc_config_i2s(cfg.codec_mode, &cfg.i2s_iface);
    ret_val |= es7210_adc_set_gain(
                   (es7210_input_mics_t)(ES7210_INPUT_MIC1 | ES7210_INPUT_MIC2),
                   (es7210_gain_value_t)GAIN_0DB);
    ret_val |= es7210_adc_set_gain(
                   (es7210_input_mics_t)(ES7210_INPUT_MIC3 | ES7210_INPUT_MIC4),
                   (es7210_gain_value_t)GAIN_37_5DB);
    ret_val |= es7210_adc_ctrl_state(cfg.codec_mode, AUDIO_HAL_CTRL_START);
    return ret_val == ESP_OK;

}

void taskplaySong(void *p)
{
    while (1) {
        if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ) {
            if (SD.exists("/key.mp3")) {
                const char *path = "key.mp3";
                audio.setPinout(BOARD_I2S_BCK, BOARD_I2S_WS, BOARD_I2S_DOUT);
                audio.setVolume(12);
                audio.connecttoFS(SD, path);
                Serial.printf("play %s\r\n", path);
                while (audio.isRunning()) {
                    audio.loop();
                }
                audio.stopSong();
            }
            xSemaphoreGive( xSemaphore );
        }
        vTaskSuspend(NULL);
    }
}

void addMessage(const char *str)
{
    lv_textarea_add_text(hw_ta, str);
    uint32_t run = millis() + 200;
    while (millis() < run) {
        lv_task_handler();
        delay(5);
    }
}
static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_VALUE_CHANGED) {
        Serial.printf("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        if (hasRadio) {
            if (lv_obj_has_state(obj, LV_STATE_CHECKED)) {
                // RX
                lv_textarea_set_text(radio_ta, "");
                Serial.print(F("[Radio] Starting to listen ... "));
                int state = radio.startReceive();
                if (state == RADIOLIB_ERR_NONE) {
                    Serial.println(F("success!"));
                } else {
                    Serial.print(F("failed, code "));
                    Serial.println(state);
                }
                sender = !sender;
            } else {
                // TX
                lv_textarea_set_text(radio_ta, "");
                // send the first packet on this node
                Serial.print(F("[Radio] Sending first packet ... "));
                transmissionState = radio.startTransmit("Hello World!");
                sender = !sender;
            }
        } else {
            lv_textarea_set_text(radio_ta, "Radio is not online");
        }
    }

}


static bool getTouch(int16_t &x, int16_t &y);



void disp_inver_event(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)return;
    uint8_t *index =  (uint8_t *) lv_event_get_user_data(e);
    if (!index)return;
    switch (*index) {
    case 0:
    case 1:
        tft.invertDisplay(*index);
        break;
    case 2:
        clicked = true;
        break;
    default:
        break;
    }
}


void initBoard(){
    /*clicked = false;
    while (!clicked) {
        lv_task_handler(); delay(5);
    }*/

    //lv_obj_del(cont);

    /*// test image
    const lv_img_dsc_t *img_src[4] = {&image1, &image2, &image3, &image4};
    lv_obj_t *img = lv_img_create(lv_scr_act());
    label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(label, 320);
    lv_label_set_text(label, "Press the key of the trackball in the middle of the board to enter the next picture");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);

    clicked = false;
    int i = 3;
    while (i > 0) {
        lv_img_set_src(img, (void *)(img_src[i]));
        while (!clicked) {
            lv_task_handler(); delay(5);
            button.check();
        }
        i--;
        clicked = false;
    }

    lv_obj_del(label);
    lv_obj_del(img);


    factory_ui(lv_scr_act());


    char buf[256];
    Serial.print("Touch:"); Serial.println(ret);
    snprintf(buf, 256, "%s:%s\n", "Touch", ret == true ? "Successed" : "Failed");
    addMessage(buf);

    ret = setupSD();
    Serial.print("SDCard:"); Serial.println(ret);
    snprintf(buf, 256, "%s:%s\n", "SDCard", ret == true ? "Successed" : "Failed");
    addMessage(buf);

    ret = setupRadio();
    Serial.print("Radio:"); Serial.println(ret);
    snprintf(buf, 256, "%s:%s\n", "Radio", ret == true ? "Successed" : "Failed");
    addMessage(buf);

    ret = setupCoder();
    Serial.print("Decoder:"); Serial.println(ret);
    snprintf(buf, 256, "%s:%s\n", "Decoder", ret == true ? "Successed" : "Failed");
    addMessage(buf);

    Serial.print("Keyboard:"); Serial.println(kbDected);
    snprintf(buf, 256, "%s:%s\n", "Keyboard", kbDected == true ? "Successed" : "Failed");
    addMessage(buf);


    if (SD.exists("/winxp.mp3")) {
        const char *path = "winxp.mp3";
        audio.setPinout(BOARD_I2S_BCK, BOARD_I2S_WS, BOARD_I2S_DOUT);
        audio.setVolume(12);
        audio.connecttoFS(SD, path);
        Serial.printf("play %s\r\n", path);
        while (audio.isRunning()) {
            audio.loop();
        }
    }

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ALL_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0,
        .mclk_multiple = I2S_MCLK_MULTIPLE_256,
        .bits_per_chan = I2S_BITS_PER_CHAN_16BIT,
        .chan_mask =
        (i2s_channel_t)(I2S_TDM_ACTIVE_CH0 | I2S_TDM_ACTIVE_CH1 |
                        I2S_TDM_ACTIVE_CH2 | I2S_TDM_ACTIVE_CH3),
        .total_chan = 4,
    };

    i2s_pin_config_t pin_config = {
        .mck_io_num = BOARD_ES7210_MCLK,
        .bck_io_num = BOARD_ES7210_SCK,
        .ws_io_num = BOARD_ES7210_LRCK,
        .data_in_num = BOARD_ES7210_DIN,
    };
    i2s_driver_install(I2S_CH, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_CH, &pin_config);
    i2s_zero_dma_buffer(I2S_CH);


    vad_inst = vad_create(VAD_MODE_0);
    vad_buff = (int16_t *)malloc(VAD_BUFFER_LENGTH * sizeof(short));
    if (vad_buff == NULL) {
        while (1) {
            Serial.println("Memory allocation failed!");
            delay(1000);
        }
    }

    // Wait until sound is detected before continuing
    uint32_t c = 0;
    while (1) {
        i2s_read(I2S_CH, (char *)vad_buff, VAD_BUFFER_LENGTH * sizeof(short), &bytes_read, portMAX_DELAY);
        // Feed samples to the VAD process and get the result
        vad_state_t vad_state = vad_process(vad_inst, vad_buff, VAD_SAMPLE_RATE_HZ, VAD_FRAME_LENGTH_MS);
        if (vad_state == VAD_SPEECH) {
            Serial.print(millis());
            Serial.println("Speech detected");
            c++;
            snprintf(buf, 256, "%s:%d\n", "Speech detected", c);
            addMessage(buf);
        }
        if (c >= 5)break;
        lv_task_handler();
        delay(5);
    }

    i2s_driver_uninstall(I2S_CH);

    pinMode(BOARD_BOOT_PIN, INPUT);

    while (!digitalRead(BOARD_BOOT_PIN)) {
        Serial.println("BOOT HAS PRESSED!!!"); delay(500);
    }

    if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ) {
        if (hasRadio) {
            if (sender) {
                transmissionState = radio.startTransmit("0");
                sendCount = 0;
                Serial.println("startTransmit!!!!");
            } else {
                int state = radio.startReceive();
                if (state == RADIOLIB_ERR_NONE) {
                    Serial.println(F("success!"));
                } else {
                    Serial.print(F("failed, code "));
                    Serial.println(state);
                }
            }
        }
        xSemaphoreGive( xSemaphore );
    }

    xTaskCreate(taskplaySong, "play", 1024 * 4, NULL, 10, &playHandle);*/

}


void setup(){
    //palcomCore.contextSwitch();
    //palcomCore.debug();
}


void loop(){
  palcomCore.contextSwitch();
}



























