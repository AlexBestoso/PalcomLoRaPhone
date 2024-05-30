struct PalcomKeyboardData{
	bool pressed = false;
	bool released = false;
	uint32_t key = 0;
	
};
typedef struct PalcomKeyboardData pal_keyboard_data_t;

// Uncomment this line to active this keyboard.
pal_keyboard_data_t keyboard_data;
