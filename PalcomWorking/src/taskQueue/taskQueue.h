#include <cstddef>
#include <cstdint>

#define TASK_SPACE_COMMS 1
#define TASK_SPACE_GRAPHICS 2
#define TASK_SPACE_STORAGE 3
#define TASK_SPACE_USERINPUT 4
#define TASK_SPACE_USB 5
#define TASK_SPACE_GOD 99

#define GOD_INSTR_LORA_INIT 0x90
#define GOD_INSTR_LORA_DENIT 0x91
#define GOD_INSTR_USB_INIT 0x92
#define GOD_INSTR_USB_DENIT 0x93
#define GOD_INSTR_WIFI_INIT 0x94
#define GOD_INSTR_WIFI_DENIT 0x95
#define GOD_INSTR_PIN_INIT 0x96
#define GOD_INSTR_LCD_INIT 0x97
#define GOD_INSTR_TOUCH_INIT 0x98
#define GOD_INSTR_LVGL_INIT 0x99
#define GOD_INSTR_SD_INIT 0x9a

#define GRAPHICS_INSTR_SETUP 1
#define GRAPHICS_INSTR_HOMEPAGE 2
#define GRAPHICS_INSTR_PUSH_MSGM 3 // push message mine
#define GRAPHICS_INSTR_PUSH_MSGO 4 // push message others

#define COMMS_INSTR_SEND 11
#define COMMS_INSTR_RECV 12
#define COMMS_INSTR_SEND_USB 13
#define COMMS_INSTR_RECV_USB 14
#define COMMS_INSTR_SEND_LORA 15
#define COMMS_INSTR_RECV_LORA 16
#define COMMS_INSTR_SEND_WIFI 17
#define COMMS_INSTR_RECV_WIFI 18

#define STORAGE_INSTR_RECVED 23
#define STORAGE_INSTR_SENT 24
#define STORAGE_INSTR_REFRESH_MSG 25

#define USB_INSTR_SETUP 31

struct task_queue_task{
	bool active=false;
	uint8_t to=0;
	uint8_t from=0;
	uint8_t instruction=0;
	uint8_t msg[256] = {0};
};

#define TASK_QUEUE_MAX 20
class TaskQueue{
	private:
		bool locked;
		struct task_queue_task tasks[TASK_QUEUE_MAX];
		size_t taskCount;
		bool lock(void);
		void unlock(void);
	public:
		TaskQueue(void);
		~TaskQueue();
	
		bool mine(int spaceID);
		void push(struct task_queue_task task);
		void push(struct task_queue_task *task);
		void push(uint8_t to, uint8_t from, uint8_t instruction);
		struct task_queue_task pop(int spaceId);
		void exec(void);
		void clear(void);
		int getTaskCount(void);

		struct task_queue_task buildTask(uint8_t to, uint8_t from, uint8_t instruction);
		struct task_queue_task buildTask(uint8_t to, uint8_t from, uint8_t instruction, uint8_t msg[256]); 
};
