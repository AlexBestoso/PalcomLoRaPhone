#include "./ButtonStyle.h"

#define PALMSG_MODE_MESH 0
#define PALMSG_MODE_NODE 1
#define PALMSG_MODE_USB 2
#define PALMSG_MODE_RELAY 3

class SetMsgModeScreen : public PalcomScreen{
        private:
		SetMsgModeButtonStyle buttonStyle;

		static void toggleMeshMode(lv_event_t *e);
		static void toggleNodeMode(lv_event_t *e);
		static void toggleUsbMode(lv_event_t *e);

		bool debugBool = false;
        public:
                SetMsgModeScreen(void);

                ~SetMsgModeScreen();

                void reset(void);
                void generateObjects(void);
		int run(void);
};
