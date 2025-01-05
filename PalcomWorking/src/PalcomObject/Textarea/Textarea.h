#define PALCOM_TA_CURSOR_UP 0
#define PALCOM_TA_CURSOR_DOWN 1
#define PALCOM_TA_CURSOR_LEFT 2
#define PALCOM_TA_CURSOR_RIGHT 3

/*static void textarea_drive_keyboard(PalcomEvent e){
	PalcomEvent event(e);
	lv_obj_t *obj = (lv_obj_t*)event.getTarget();
	char *data = (char *)event.getUserData();

	if(event.getCode() == LV_EVENT_INSERT){
	
	}else if(event.getCode() == LV_EVENT_FOCUSED){
		keyboardFocusedObj = obj;
	}else if(event.getCode() == LV_EVENT_DEFOCUSED){
		keyboardFocusedObj == NULL;
	}
}*/
class PalcomTextarea : public PalcomObject{
  	private:
  		lv_style_t style;
		lv_style_t focusedStyle;

		static void simpleCb(lv_event_t *e);

	public:
    		lv_obj_t *getObj(void);

		void pushChar(char c);
		void popCharLeft(void);
		void popCharRight(void);
		void pushText(const char *t);
    		void setText(const char *text);
		void setTextPlaceholder(const char *t);
		void setStyle(lv_style_t *style, lv_style_t *focusedStyle);
		void setStyleMain(lv_style_t *style);
		void setStyleScrollbar(lv_style_t *style);
		void setStyleSelected(lv_style_t *style);
		void setStyleCursor(lv_style_t *style);
		void setStylePlaceholder(lv_style_t *style);
    		void create(lv_obj_t *parent);
    		void createGlobal(lv_obj_t *parent, uint id);
    		void setCursorClickPos(bool val);
    		void setTextSelection(bool val);
    		void setSize(int w, int h);
    		void setMaxLength(int len);
    		void setOneLine(bool val);
    		void setAlignment(lv_align_t ref, int x, int y);
    		void setPasswordMode(bool val);
   		const char *getText(void);
		void moveCursor(int direction);
		void setValidChars(const char *s);
};
