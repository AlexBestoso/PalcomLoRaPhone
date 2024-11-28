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
		void setStyle(lv_style_t *s);

		void init(void);

		lv_state_t getStateDefault(void);

		lv_state_t getStateChecked(void);

		lv_state_t getStateFocused(void);

		lv_state_t getStateFocusKey(void);

		lv_state_t getStateEdited(void);

		lv_state_t getStateHovered(void);

		lv_state_t getStatePressed(void);

		lv_state_t getStateScrolled(void);

		lv_state_t getStateDisabled(void);

		lv_state_t getStateCustomOne(void);

		lv_state_t getStateCustomTwo(void);

		lv_state_t getStateCustomThree(void);

		lv_state_t getStateCustomFour(void);

		void setColorFilter(lv_color_filter_dsc_t *f, lv_color_t (*colorFilterHandler)(const lv_color_filter_dsc_t *, lv_color_t, lv_opa_t));
		void initColorFilter(void);

		void setRadius(int r);

		void setBgOpacity(int o);
		

		lv_color_t makeColor(int r, int g, int b);

		lv_color_t makeColorHex(int h);

		lv_color_t makeColorHex3(int h);

		void setBgColor(lv_color_t c);

		void setBgGradientColor(lv_color_t c);

		void setBgGradientDirection(int d);
		

		void setBorderColor(lv_color_t c);

		void setBorderOpacity(int o);
			

		void setAnimationTime(int t);
		void setBorderWidth(int w);

		/*
		 * LV_BORDER_SIDE_NONE/TOP/BOTTOM/LEFT/RIGHT/INTERNAL
		 *  Can be ORed '||' to use multiple.
		 *  */
		void setBorderSide(lv_border_side_t s);

		void setTextColor(lv_color_t c);

		/*
		 * LV_TEXT_ALIGN_AUTO
		 * LV_TEXT_ALIGN_LEFT
		 * LV_TEXT_ALIGN_CENTER
		 * LV_TEXT_ALIGN_RIGHT
		 * */
		void setTextAlign(lv_text_align_t a);

		void applyColorFilter();

		void setColorFilterOpacity(int o);

		void recolorImgOpacity(int o);

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

		void recolorImg(lv_color_t c);

		void setShadowWidth(int w);

		void setShadowColor(lv_color_t c);

		void setShadowOffsetY(int offset);

		void setOutlineOpacity(int o);
	
		void setOutlineColor(lv_color_t c);

		void setOutlineWidth(int w);

		void setAllPadding(int p);
		
		void setHorizontalPadding(int p);

		void setVirticalPadding(int p);

		void setPaddingGap(int p);

		void setTranslateY(int y);

		void setTransition(lv_style_transition_dsc_t *t);
};
