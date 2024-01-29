class DefaultTextareaStyle : public PalcomStyle{
	private:
		lv_style_t textarea;
                lv_style_t textareaFocused;
		lv_color_filter_dsc_t color_filter;

                static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa){
                        LV_UNUSED(dsc);
                        return lv_color_darken(color, opa);
                }
	public:
		lv_style_t *getStyle(void){
                        return (lv_style_t *)&textarea;
                }

		lv_style_t *getFocusedStyle(void){
			return (lv_style_t *)&textareaFocused;
		}

		lv_color_filter_dsc_t *getColorFilter(void){
                        return (lv_color_filter_dsc_t *)&color_filter;
                }

		void initStyle(void){
			// Setup the default style
			this->setStyle(this->getStyle());
			this->init();
			this->setRadius(10);
                        this->setBgOpacity(11);
                        this->setBgColor(this->makeColor(0x10, 0x05, 0x10));
                        this->setBgGradientColor(this->makeColor(0x80, 0x30, 0x80));
                        this->setBgGradientDirection(2);
                        this->setBorderColor(this->makeColor(0x55, 0, 0));
                        this->setBorderOpacity(2);
                        this->setBorderWidth(2);
                        this->setTextColor(this->makeColor(0xff, 0xff, 0xff));

			// Configure focused
			this->setColorFilter(this->getColorFilter(), darken);
                        this->initColorFilter();
                        this->setStyle(this->getFocusedStyle());
                        this->init();
                        this->applyColorFilter();
                        this->setColorFilterOpacity(2);
		}
}defaultTextareaStyle;
