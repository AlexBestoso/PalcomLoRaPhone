class PalcomSection : public PalcomObject{
	private:

	public:

		void create(lv_obj_t *parent){
			this->generate(parent, pal_menusect);
		}
};
