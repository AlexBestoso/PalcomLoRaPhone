static lv_style_prop_t pinButtProps[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, (lv_style_prop_t)NULL};

class PinPadButtonStyle : public PalcomStyle{
	private:
		lv_style_t stylePinBtn;
		lv_style_t stylePinBtnPressed;
		lv_style_transition_dsc_t pinButtTrans;
	public:
		lv_style_t *getStyle(void){
			//this->initStyle();
                        return (lv_style_t *)&stylePinBtn;
                }

                lv_style_t *getPressedStyle(void){
                        return (lv_style_t *)&stylePinBtnPressed;
                }


		void initStyle(void){
			/*
			 * Default style.
			 * */
			this->setStyle(this->getStyle());
			this->init();
			this->setRadius(100);
			this->setBgOpacity(10);
			this->setBgColor(lv_palette_main(LV_PALETTE_BLUE));
			this->setBgGradientColor(lv_palette_darken(LV_PALETTE_BLUE, 2));
			this->setBgGradientDirection(2);
			this->setBorderOpacity(4);
			this->setBorderWidth(2);
			this->setBorderColor(lv_palette_main(LV_PALETTE_GREY));

			this->setShadowWidth(8);
			this->setShadowColor(lv_palette_main(LV_PALETTE_GREY));
			this->setShadowOffsetY(8);
			this->setOutlineOpacity(200);
			this->setOutlineColor(lv_palette_main(LV_PALETTE_BLUE));
			this->setTextColor(lv_color_white());
			this->setAllPadding(10);

			/*
			 * Pressed Style
			 * */
			this->setStyle(this->getPressedStyle());
			this->init();
			this->setOutlineWidth(30);
			this->setOutlineOpacity(-1);
			this->setTranslateY(5);
			this->setShadowOffsetY(3);
			this->setBgColor(lv_palette_darken(LV_PALETTE_BLUE, 2));
			this->setBgGradientColor(lv_palette_darken(LV_PALETTE_BLUE, 4));

			/*
			 * Style Animation / Transition
			 * */
			lv_style_transition_dsc_init(&pinButtTrans, pinButtProps, lv_anim_path_linear, 300, 0, NULL);
			this->setTransition(&pinButtTrans);
		}
}style_pinpadButton;
