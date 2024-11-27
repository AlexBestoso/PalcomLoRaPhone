#include "../PalcomColors/PalcomColors.h"

#define GLOBAL_GUI_OBJECT_COUNT 1

enum PalcomObjectType{pal_label, pal_textarea, pal_button, pal_imgbutton, pal_base, pal_menu, pal_msgbox, pal_checkbox, pal_menusect, pal_menupage, pal_slider, pal_switch};

class PalcomObject{
	private:
		lv_obj_t *object = NULL;
		int id = -1;
	public:
		PalcomColors colors;

		lv_obj_t *getObject(void);

		void setObject(lv_obj_t *object);

		int getObjectId(void);
		void setObjectId(int id);

		void destroy(void);
		void create(lv_obj_t *parent);
                void createGlobal(lv_obj_t *parent, uint id);

		void addState(lv_state_t s);
		void removeState(lv_state_t s);
		bool stateInUse(lv_state_t s);

		void generate(lv_obj_t *parent, PalcomObjectType objtype, const char *data);
		void generate(lv_obj_t *parent, PalcomObjectType objtype);
		void generateGlobal(lv_obj_t *parent, uint id, PalcomObjectType objtype);

		void loadGlobal(int id);

		void setAnyStyle(lv_style_t *s, lv_style_selector_t sel);
		void setDefaultStyle(lv_style_t *s);
		void setCheckedStyle(lv_style_t *s);
		void setFocusedStyle(lv_style_t *s);
		void setFocusedKeyStyle(lv_style_t *s);
		void setEditedStyle(lv_style_t *s);
		void setHoveredStyle(lv_style_t *s);
		void setPressedStyle(lv_style_t *s);
		void setScrolledStyle(lv_style_t *s);
		void setDisabledStyle(lv_style_t *s);

		void setAlignment(lv_align_t ref, int x, int y);
		void setSize(int x, int y);
                void setSizeRaw(int x, int y);

                void setScreenScrollDirection(lv_dir_t direction);

                void setScrollMode(lv_scrollbar_mode_t mode);

		void center(void);

		int getHorizontalResolution(void);
		int getVirticalResolution(void);

		void fullScreen(void);

		void execute();

		void setSimpleCallback(void(*func)(lv_event_t*));
		void setParamCallback(void(*func)(lv_event_t *), void *input, lv_event_code_t event);
		void setParamCallback(void(*func)(lv_event_t *), void *input);

		void addFlag(lv_obj_flag_t f);
		void setFlag(lv_obj_flag_t f);
		void unsetFlag(lv_obj_flag_t f);

		lv_color_t getStyleBgColor(int v);
		lv_color_t getStyleBgColor(void);
		void setStyleBgColor(lv_color_t c, int v);
		void setStyleBgColor(lv_color_t c);

		void setStylePaddingHor(int padding, lv_style_selector_t sel);
		void setStylePaddingHor(int padding);
		int getStylePaddingLeft(lv_part_t p);
		int getStylePaddingLeft(void);

		void setFlexGrow(int v);
};
