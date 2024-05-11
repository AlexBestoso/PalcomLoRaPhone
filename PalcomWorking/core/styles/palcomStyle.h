class PalcomStyle{
	private:
		lv_style_t *targetStyle = NULL;
		lv_color_filter_dsc_t *targetColorFilter = NULL;
									  /*dsc         color     opa */
		lv_color_t (*colorFilterHandler)(const lv_color_filter_dsc_t *, lv_color_t, lv_opa_t);
	public:
		/*
		 * You must call this function before running any other functions.
		 * */
		void setStyle(lv_style_t *s){
			this->targetStyle = s;
		}

		void init(void){
			lv_style_init(this->targetStyle);
		}

		void setColorFilter(lv_color_filter_dsc_t *f, lv_color_t (*colorFilterHandler)(const lv_color_filter_dsc_t *, lv_color_t, lv_opa_t)){
			this->targetColorFilter = f;
			this->colorFilterHandler = colorFilterHandler;
		}
		void initColorFilter(void){
			lv_color_filter_dsc_init(targetColorFilter, colorFilterHandler);
		}

		void setRadius(int r){
			lv_style_set_radius(this->targetStyle, r);
		}

		void setBgOpacity(int o){
			switch(o){
				case 0:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_0);
					break;
				case 1:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_10);
					break;
				case 2:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_20);
					break;
				case 3:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_30);
					break;
				case 4:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_40);
					break;
				case 5:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_50);
					break;
				case 6:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_60);
					break;
				case 7:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_70);
					break;
				case 8:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_80);
					break;
				case 9:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_90);
					break;
				case 10:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_100);
					break;
				case -1:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_TRANSP);
					break;
				default:
					lv_style_set_bg_opa(this->targetStyle, LV_OPA_COVER);
					break;

			}
		}

		lv_color_t makeColor(int r, int g, int b){
			return lv_color_make(r, g, b);
		}

		lv_color_t makeColorHex(int h){
			return lv_color_hex(h);
		}

		lv_color_t makeColorHex3(int h){
			return lv_color_hex3(h);
		}

		void setBgColor(lv_color_t c){
			lv_style_set_bg_color(this->targetStyle, c);	
		}

		void setBgGradientColor(lv_color_t c){
			lv_style_set_bg_grad_color(this->targetStyle, c);
		}

		void setBgGradientDirection(int d){
			switch(d){
				case 1:
					lv_style_set_bg_grad_dir(this->targetStyle, LV_GRAD_DIR_HOR);
					break;
				case 2: 
					lv_style_set_bg_grad_dir(this->targetStyle, LV_GRAD_DIR_VER);
					break;
				default:
					lv_style_set_bg_grad_dir(this->targetStyle, LV_GRAD_DIR_NONE);
					break;
			}
		}

		void setBorderColor(lv_color_t c){
			lv_style_set_border_color(this->targetStyle, c);
		}

		void setBorderOpacity(int o){
			switch(o){
                                case 0:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_0);
                                        break;
                                case 1:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_10);
                                        break;
                                case 2:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_20);
                                        break;
                                case 3:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_30);
                                        break;
                                case 4:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_40);
                                        break;
                                case 5:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_50);
                                        break;
                                case 6:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_60);
                                        break;
                                case 7:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_70);
                                        break;
                                case 8:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_80);
                                        break;
                                case 9:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_90);
                                        break;
				case 10:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_100);
                                        break;
                                case -1:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_TRANSP);
                                        break;
                                default:
                                        lv_style_set_border_opa(this->targetStyle, LV_OPA_COVER);
                                        break;
			}
		}

		void setBorderWidth(int w){
			lv_style_set_border_width(this->targetStyle, w);
		}

		void setTextColor(lv_color_t c){
			lv_style_set_text_color(this->targetStyle, c);
		}

		void applyColorFilter(){
			lv_style_set_color_filter_dsc(this->targetStyle, this->targetColorFilter);
		}

		void setColorFilterOpacity(int o){
			switch(o){
                                case 0:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_0);
                                        break;
                                case 1:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_10);
                                        break;
                                case 2:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_20);
                                        break;
                                case 3:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_30);
                                        break;
                                case 4:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_40);
                                        break;
                                case 5:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_50);
                                        break;
                                case 6:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_60);
                                        break;
                                case 7:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_70);
                                        break;
                                case 8:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_80);
                                        break;
                                case 9:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_90);
                                        break;
				case 10:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_100);
                                        break;
                                case -1:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_TRANSP);
                                        break;
                                default:
                                        lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_COVER);
                                        break;
                        }
		}

		void recolorImgOpacity(int o){
                        switch(o){
                                case 0:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_0);
                                        break;
                                case 1:
                                       	lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_10);
                                        break;
                                case 2:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_20);
                                        break;
                                case 3:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_30);
                                        break;
                                case 4:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_40);
                                        break;
                                case 5:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_50);
                                        break;
                                case 6:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_60);
                                        break;
                                case 7:
                                       	lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_70);
                                        break;
                                case 8:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_80);
                                        break;
                                case 9:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_90);
                                        break;
                                case 10:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_100);
                                        break;
                                case -1:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_TRANSP);
                                        break;
                                default:
                                        lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_COVER);
                                        break;

                        }
                }

		/*
		 *   enumerator LV_OPA_TRANSP = 0
		 *   enumerator LV_OPA_0 = 0
		 *   enumerator LV_OPA_10 = 25
		 *   enumerator LV_OPA_20 = 51
		 *   enumerator LV_OPA_30 = 76
		 *   enumerator LV_OPA_40 = 102
		 *   enumerator LV_OPA_50 = 127
		 *   enumerator LV_OPA_60 = 153
		 *   enumerator LV_OPA_70 = 178
		 *   enumerator LV_OPA_80 = 204
		 *   enumerator LV_OPA_90 = 229
		 *   enumerator LV_OPA_100 = 255
		 *   enumerator LV_OPA_COVER = 255
		 */

		void recolorImg(lv_color_t c){
			lv_style_set_img_recolor(this->targetStyle, c);
		}

		void setShadowWidth(int w){
			lv_style_set_shadow_width(this->targetStyle, w);
		}

		void setShadowColor(lv_color_t c){
			lv_style_set_shadow_color(this->targetStyle, c);
		}

		void setShadowOffsetY(int offset){
			lv_style_set_shadow_ofs_y(this->targetStyle, offset);
		}

		void setOutlineOpacity(int o){
			switch(o){
				case 0:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_0);
					break;
				case 10:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_10);
					break;
				case 20:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_20);
					break;
				case 30:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_30);
					break;
				case 40:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_40);
					break;
				case 50:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_50);
					break;
				case 60:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_60);
					break;
				case 70:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_70);
					break;
				case 80:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_80);
					break;
				case 90:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_90);
					break;
				case 100:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_100);
					break;
				case -1:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_TRANSP);
					break;
				default:
					lv_style_set_outline_opa(this->targetStyle, LV_OPA_COVER);
					break;
			}
		}
		
		void setOutlineColor(lv_color_t c){
			lv_style_set_outline_color(this->targetStyle, c);
		}

		void setOutlineWidth(int w){
			lv_style_set_outline_width(this->targetStyle, w);
		}

		void setAllPadding(int p){
			lv_style_set_pad_all(this->targetStyle, p);
		}

		void setTranslateY(int y){
			lv_style_set_translate_y(this->targetStyle, y);
		}

		void setTransition(lv_style_transition_dsc_t *t){
			lv_style_set_transition(this->targetStyle, t);
		}
};
