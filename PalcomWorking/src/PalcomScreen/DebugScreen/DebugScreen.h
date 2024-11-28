#include "./ButtonStyle.h"
class PalcomDebugScreen : public PalcomScreen{
        private:
		DebugScreenButtonStyle buttonStyle;

		static void buttonCallback(lv_event_t *e);
        public:
                PalcomDebugScreen(void);

                ~PalcomDebugScreen();

                void reset(void);
                void generateObjects(void);
		int run(void);
};
