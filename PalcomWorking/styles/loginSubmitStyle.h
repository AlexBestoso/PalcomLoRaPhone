
class LoginSubmitStyle : public PalcomStyle{
	private:
		lv_style_t submitButton;
		lv_style_t submitButtonPressed;
		lv_color_filter_dsc_t color_filter;
		
		static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa){
                        LV_UNUSED(dsc);
                        return lv_color_darken(color, opa);
                }
	public:
		lv_style_t *getStyle(void){
			return (lv_style_t *)&submitButton;
		}

		lv_style_t *getPressedStyle(void){
			return (lv_style_t *)&submitButtonPressed;
		}
		void initStyle(){
			/*Create a simple button style*/
    			lv_style_init(&submitButton);
    			lv_style_set_radius(&submitButton, 10);
    			lv_style_set_bg_opa(&submitButton, LV_OPA_COVER);
    			lv_style_set_bg_color(&submitButton, lv_color_make(0x10, 0x05, 0x10));
    			lv_style_set_bg_grad_color(&submitButton, lv_color_make(0x10, 0x05, 0x10));
    			lv_style_set_bg_grad_dir(&submitButton, LV_GRAD_DIR_VER);

    			lv_style_set_border_color(&submitButton, lv_color_make(0x55, 0, 0));
    			lv_style_set_border_opa(&submitButton, LV_OPA_20);
    			lv_style_set_border_width(&submitButton, 2);

			lv_style_set_text_color(&submitButton, lv_color_white());

			/*Create a style for the pressed state.
     			*Use a color filter to simply modify all colors in this state*/
    			lv_color_filter_dsc_init(&color_filter, darken);
    			lv_style_init(&submitButtonPressed);
    			lv_style_set_color_filter_dsc(&submitButtonPressed, &color_filter);
    			lv_style_set_color_filter_opa(&submitButtonPressed, LV_OPA_20);
		}


}lss;
