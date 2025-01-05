class MsgSenderStyle : public PalcomStyle{
	private:
		lv_style_t style;
		lv_style_t style2;
	public:
		MsgSenderStyle(void);
		~MsgSenderStyle();

		lv_style_t *getStyle2(void);

		void initStyle(void);
		void initStyle2(void);
};
