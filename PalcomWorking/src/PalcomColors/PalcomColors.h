class PalcomColors{
	private:

	public:
		int getColorBrightness(lv_color_t c);
		int getBrightness(lv_color_t c);
		lv_color_t darken(lv_color_t c, int adjuster);
		lv_color_t make(int r, int g, int b);
};
