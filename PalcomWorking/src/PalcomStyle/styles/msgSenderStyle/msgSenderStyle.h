class MsgSenderStyle : public PalcomStyle{
	private:
		lv_style_t style;
		lv_style_t style2;
		lv_style_t style3;
		lv_style_t style4;
	public:
		MsgSenderStyle(void);
		~MsgSenderStyle();

		lv_style_t *getStyle2(void);
		lv_style_t *getStyle3(void);
		lv_style_t *getStyle4(void);

		void initStyle(void);
		void initStyle2(void);
		void initStyle3(void);
		void initStyle4(void);
};
