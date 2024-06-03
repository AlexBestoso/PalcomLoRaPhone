class PalcomSlider : public PalcomObject{
	private:

	public:

		void create(lv_obj_t *parent){
			this->generate(parent, pal_slider);
		}

		void setValue(int32_t value, lv_anim_enable_t anim=LV_ANIM_OFF){
			lv_slider_set_value(this->getObject(), value, anim);
		}

		void setLeftValue(uint32_t value, lv_anim_enable_t anim=LV_ANIM_OFF){
			lv_slider_set_left_value(this->getObject(), value, anim);
		}

		void setRange(int32_t min, int32_t max){
			lv_slider_set_range(this->getObject(), min, max);
		}

		void setMode(lv_slider_mode_t mode){
			lv_slider_set_mode(this->getObject(), mode);
		}

		int32_t getValue(void){
			return lv_slider_get_value((const lv_obj_t *)this->getObject());
		}

		int32_t getLeftValue(void){
			return lv_slider_get_left_value((const lv_obj_t *)this->getObject());
		}

		int32_t getMinValue(void){
			return lv_slider_get_min_value((const lv_obj_t *)this->getObject());
		}


		int32_t getMaxValue(void){
			return lv_slider_get_max_value((const lv_obj_t *)this->getObject());
		}

		bool isDragged(void){
			return lv_slider_is_dragged((const lv_obj_t *)this->getObject());
		}

		lv_slider_mode_t getMode(void){
			return lv_slider_get_mode(this->getObject());
		}

		bool isSymmetrical(void){
			return lv_slider_is_symmetrical(this->getObject());
		}

};
