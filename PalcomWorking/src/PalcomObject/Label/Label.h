class PalcomLabel : public PalcomObject{
  	private:
	public:
    		lv_obj_t *getObj(void);

    		void create(lv_obj_t *parent);

    		void createGlobal(lv_obj_t *parent, uint id);

		/*
    		 * Valid Modes : 
    		 * LV_LABEL_LONG_EXPAND, LV_LABEL_LONG_BREAK, LV_LABEL_LONG_DOT,
    		 * LV_LABEL_LONG_SCROLL, LV_LABEL_LONG_SCROLL_CIRC, LV_LABEL_LONG_CROP
    		 */
    		void setLongMode(lv_label_long_mode_t mode);

    		void setWidth(int width);

    		void setText(const char *text);
};
