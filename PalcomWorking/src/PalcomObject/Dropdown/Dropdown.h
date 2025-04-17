class PalcomDropdown : public PalcomObject{
  	private:
		String listOptions;
	public:
		PalcomDropdown(void);
    		lv_obj_t *getObj(void);

    		void create(lv_obj_t *parent);

		void setList(const char *opts);
		const char *getList(void);
		void getSelection(char *out, size_t size);
		void setSelection(int id);
};
