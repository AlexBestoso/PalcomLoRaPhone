#define SOFTWARE_VERSION "0.2.4 Beta"

#define SCREEN_HOR 320
#define SCREEN_VIR 240

/*
 * Application Contexts
 * */
#define CONTEXT_SETUP -1
#define CONTEXT_LOGIN 0
#define CONTEXT_MAINMENU 1
#define CONTEXT_SETTINGS 2
#define CONTEXT_CONTACTS 3
#define CONTEXT_ABOUT 4


#define CONTEXT_PLAINTEXT_MESSAGING 3
#define CONTEXT_CIPHERTEXT_MESSAGING 4
#define CONTEXT_KEYSHARING 5
#define CONTEXT_WIFI 6

#define LVGL_BUFFER_SIZE    (TFT_WIDTH * TFT_HEIGHT ) * sizeof(lv_color_t)

lv_color_t *draw_buf = NULL;
lv_group_t *keyboardGroup = NULL;
lv_obj_t *keyboardFocusedObj = NULL;
int Setup_setupControl = 0;
int Sleep_interactionCtx = 0;
int Sleep_maxBrightness = 256;
int Sleep_brightness = 0;
float Sleep_timer = millis();

/*
 * Structs
 * */
#include "./structs/structs.h"
