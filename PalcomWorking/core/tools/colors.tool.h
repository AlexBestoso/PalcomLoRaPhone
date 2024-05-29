class PalcomColors{
	private:

	public:
		int getColorBrightness(lv_color_t c){ // depreicated
			return lv_color_brightness(c);
		}
		int getBrightness(lv_color_t c){
			return lv_color_brightness(c);
		}

		lv_color_t darken(lv_color_t c, int adjuster){
			return lv_color_darken(c, adjuster);
		}
		
		lv_color_t make(int r, int g, int b){
			return LV_COLOR_MAKE(r, g, b);
		}

};
