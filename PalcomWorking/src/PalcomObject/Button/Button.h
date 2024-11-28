class PalcomButton : public PalcomObject{
  	private:
    		PalcomLabel label;
		lv_style_t style;
		lv_style_t pressedStyle;

	public:
    		lv_obj_t *getObj();

		void create(lv_obj_t *parent);

    		void createGlobal(lv_obj_t *parent, int id);

		void setStyle(lv_style_t *style, lv_style_t *pressedStyle);

		lv_style_t getStyle(void);

    		void setLabel(PalcomLabel label);
    
    		void setSimpleCallback(void(*func)(lv_event_t*));
    
    		void setValuedCallback(void(*func)(lv_event_t*), uint8_t *val);
    	
		void setRelativeAlignment(lv_align_t ref, int x, int y);
};
