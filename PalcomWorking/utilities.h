/**
 * @file      utilities.h
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-11
 *
 */
#pragma once

#include "./defines.h"

//! The board peripheral power control pin needs to be set to HIGH when using the peripheral



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

bool sd_card_available = false;


char displayed_messages[MSG_DISPLAY_MAX][MSG_BUFFER_MAX];
int displayed_page = 0;


const uint8_t CORE_ROUTING_KEY[CORE_ROUTING_KEY_SIZE]= {
        162, 232, 0xff, 0x3a, 0x69, 0x22, 0xff, 0x82, 0x43 , 0x87,

        0x23, 0xe8, 0x18, 0x08, 0x17, 0x76, 0x6E, 0x69, 0x67, 0x67,

        0x65, 0x72, 0x10, 0x3a, 0x4a, 0x50, 0x03, 0x18, 0x07, 0x21,
        145, 33
};

