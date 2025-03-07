class SetMsgModeButtonStyle : public PalcomStyle{
	private:
		lv_style_t submitButton;
		lv_style_t submitButtonPressed;
		lv_color_filter_dsc_t color_filter;
		
		static lv_color_t darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa);
	public:
		lv_style_t *getStyle(void);

		lv_style_t *getPressedStyle(void);

		lv_color_filter_dsc_t *getColorFilter(void);
		void initStyle();
};
