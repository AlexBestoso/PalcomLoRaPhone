class DefaultPopupStyle : public PalcomStyle{
	private:
		lv_style_t style;
		lv_color_filter_dsc_t color_filter;
		
	public:
		lv_style_t *getStyle(void){
			return (lv_style_t *)&style;
		}

		lv_color_filter_dsc_t *getColorFilter(void){
			return (lv_color_filter_dsc_t *)&color_filter;
		}
		void initStyle(){
			/*Create a simple button style*/
			this->setStyle(this->getStyle());
			this->init();
			this->setRadius(10);
			this->setBgOpacity(11);
			this->setBgColor(this->makeColor(0x10, 0x05, 0x10));
			this->setBorderColor(this->makeColor(0x55, 0, 0));
			this->setBorderOpacity(2);
			this->setBorderWidth(2);
			this->setTextColor(this->makeColor(0xff, 0xff, 0xff));
		}


}defaultPopupStyle;
