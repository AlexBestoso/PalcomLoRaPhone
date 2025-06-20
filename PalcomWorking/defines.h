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
#define RATIO_X 3
#define RATIO_Y 4

#define MSG_DISPLAY_MAX 10
#define MSG_BUFFER_MAX 257

#define SYS_VERSION "2.0.2 Alpha"

#define CORE_ROUTING_KEY_SIZE 32
