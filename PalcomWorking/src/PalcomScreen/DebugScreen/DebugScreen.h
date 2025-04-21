#include "./ButtonStyle.h"
#include <src/PalcomObject/Tileview/Tileview.h>

#define PALMSG_MODE_MESH 0
#define PALMSG_MODE_NODE 1
#define PALMSG_MODE_USB 2
#define PALMSG_MODE_RELAY 3

class PalcomDebugScreen : public PalcomScreen{
        private:
		DebugScreenButtonStyle buttonStyle;
		TileStyle tileStyle;
		MsgSenderStyle msgSenderStyle;
		
		PalcomButton meshButton;
		PalcomButton nodeButton;
		PalcomButton usbButton;
		
		PalcomTriangle triangle;

		
		PalcomTileview tileView;
		lv_obj_t *tile1, *tile2, *tile3, *tile4;

		static void toggleMeshMode(lv_event_t *e);
		static void toggleNodeMode(lv_event_t *e);
		static void toggleUsbMode(lv_event_t *e);
		static void sendMessage(lv_event_t *e);
		static void applySettings(lv_event_t *e);

		void buildUsbSettings(lv_obj_t *target);
		void buildLoRaSettings(lv_obj_t *tabTarget);
		void buildMainSettings(lv_obj_t *tabTarget);

		void buildHomepage(lv_obj_t *target);
		void buildModeSelect(lv_obj_t *target);
		void buildSettingspage(lv_obj_t *target);
		void buildAboutpage(lv_obj_t *target);


		bool debugBool = false;
		int currentMode = 0;
        public:
                PalcomDebugScreen(void);

                ~PalcomDebugScreen();

                void reset(void);
                void generateObjects(void);
		int run(void);
};
