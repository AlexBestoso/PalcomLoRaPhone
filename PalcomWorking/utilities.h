/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 *
 */
#pragma once



//! The board peripheral power control pin needs to be set to HIGH when using the peripheral
#define BOARD_POWERON       10

#define BOARD_I2S_WS        5
#define BOARD_I2S_BCK       7
#define BOARD_I2S_DOUT      6

#define BOARD_I2C_SDA       18
#define BOARD_I2C_SCL       8

#define BOARD_BAT_ADC       4

#define BOARD_TOUCH_INT     16
#define BOARD_KEYBOARD_INT  46

#define BOARD_SDCARD_CS     39
#define BOARD_TFT_CS        12
#define RADIO_CS_PIN        9

#define BOARD_TFT_DC        11
#define BOARD_TFT_BACKLIGHT 42

#define BOARD_SPI_MOSI      41
#define BOARD_SPI_MISO      38
#define BOARD_SPI_SCK       40

#define BOARD_TBOX_G02      2
#define BOARD_TBOX_G01      3
#define BOARD_TBOX_G04      1
#define BOARD_TBOX_G03      15

#define BOARD_ES7210_MCLK   48
#define BOARD_ES7210_LRCK   21
#define BOARD_ES7210_SCK    47
#define BOARD_ES7210_DIN    14

#define RADIO_BUSY_PIN      13
#define RADIO_RST_PIN       17
#define RADIO_DIO1_PIN      45

#define BOARD_BOOT_PIN      0

#define BOARD_BL_PIN        42

#define MESSAGE_LOCAL_START 0x5
#define MESSAGE_LOCAL_END 0x6
#define MESSAGE_REMOTE_START 0x7
#define MESSAGE_REMOTE_END 0x8

#define LVGL_BUFFER_SIZE    (TFT_WIDTH * TFT_HEIGHT ) * sizeof(lv_color_t)

#define SCREEN_HOR 320
#define SCREEN_VIR 240


bool checkKb(){
  Wire.requestFrom(0x55, 1);
  return Wire.read() != -1;
}

void handleEvent(AceButton * /* button */, uint8_t eventType,
                 uint8_t /* buttonState */);

TFT_eSPI        tft = TFT_eSPI();
size_t          bytes_read;
uint8_t         status;
TaskHandle_t    playHandle = NULL;
TaskHandle_t    radioHandle = NULL;

AceButton   button;
bool        clicked = false;
bool        txFlag =  true;
bool        rxFlag = false;
bool        transmissionFlag = true;
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

bool screenLockConditionSpace = false;
bool screenLockConditionBall = false;

lv_color_t *draw_buf = NULL;

lv_group_t *keyboardGroup = NULL;

lv_obj_t *keyboardFocusedObj = NULL;

int Setup_setupControl = 0;
int Sleep_interactionCtx = 0;
int Sleep_maxBrightness = 256;
int Sleep_brightness = 0;
float Sleep_timer = millis();

TouchLib *touch = NULL;

#define USER_BUF_SIZE 256
uint8_t userBuffer[USER_BUF_SIZE] = {0};
int userBufferIdx = 0;
size_t userBufferSize = 0;

#define GLOBAL_GUI_OBJECT_COUNT 1
lv_obj_t *globalGuiObjects[GLOBAL_GUI_OBJECT_COUNT] = {NULL};
int PalcomScreenError=0;

LV_IMG_DECLARE(mousePointerPng);
LV_IMG_DECLARE(spaceAI2);
const lv_img_dsc_t *spaceImg[1] = {&spaceAI2};

int palcome_message_mode=-1;

SX1262 _radio = new Module(9, 45, 17, 13);
volatile bool loraSnakeTransmit = false;
volatile bool loraSnakeReceive = false;

