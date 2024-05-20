class DefaultTextareaStyle : public PalcomStyle{
	private:
		lv_style_t textarea;
                lv_style_t textareaFocused;
		lv_style_t textareaCursor;
		lv_style_t textareaErrored;
		lv_style_t placeHolder;
	public:
		lv_style_t *getStyle(void){
                        return (lv_style_t *)&textarea;
                }

		lv_style_t *getFocusedStyle(void){
			return (lv_style_t *)&textareaFocused;
		}

		lv_style_t *getCursorStyle(void){
			return (lv_style_t *)&textareaCursor;
		}

		lv_style_t *getPlaceHolderStyle(void){
			return (lv_style_t *)&placeHolder;
		}

		lv_style_t *getErrorStyle(void){
			return (lv_style_t *)&textareaErrored;
		}

		void initStyle(void){
			// Setup the default style
			this->setStyle(this->getStyle());
			this->init();
			this->setRadius(10);
                        this->setBgOpacity(11);
                        this->setBgColor(lv_palette_main(LV_PALETTE_BLUE));
                        this->setBgGradientColor(lv_palette_darken(LV_PALETTE_BLUE, 2));
                        this->setBgGradientDirection(2);
                        this->setBorderColor(lv_palette_main(LV_PALETTE_GREY));
                        this->setBorderOpacity(2);
                        this->setBorderWidth(2);
                        this->setTextColor(lv_color_white());
			this->setTextAlign(LV_TEXT_ALIGN_CENTER);
			this->setVirticalPadding(7);

			// Setup the errored style
			this->setStyle(this->getErrorStyle());
                        this->init();
                        this->setRadius(10);
                        this->setBgOpacity(11);
                        this->setBgColor(lv_palette_main(LV_PALETTE_RED));
                        this->setBgGradientColor(lv_palette_darken(LV_PALETTE_RED, 2));
                        this->setBgGradientDirection(2);
                        this->setBorderColor(lv_palette_main(LV_PALETTE_GREY));
                        this->setBorderOpacity(2);
                        this->setBorderWidth(2);
                        this->setTextColor(lv_color_white());
                        this->setTextAlign(LV_TEXT_ALIGN_CENTER);
                        this->setVirticalPadding(7);

			// Configure focused
                        this->setStyle(this->getFocusedStyle());
                        this->init();
			this->setVirticalPadding(7);

			// Configure Cursor
			this->setStyle(this->getCursorStyle());
			this->init();
			this->setAnimationTime(500);
		//	this->setVirticalPadding(7);
			this->setBorderSide(LV_BORDER_SIDE_LEFT);
                        this->setBorderOpacity(10);
			this->setBorderColor(lv_color_white());

			this->setStyle(this->getPlaceHolderStyle());
			this->init();
			this->setTextColor(lv_color_black());

		}
}defaultTextareaStyle;
