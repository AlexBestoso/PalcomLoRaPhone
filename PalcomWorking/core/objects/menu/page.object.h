class PalcomPage : public PalcomObject{
	private:
	public:
		PalcomSection section;
		void create(lv_obj_t *parent, const char *title){
			this->generate(parent, pal_menupage, title);
                }

		void addSeparator(void){
			lv_menu_separator_create(this->getObject());
		}

		void createSection(void){
			this->section.create(this->getObject());
		}

		lv_obj_t *getSection(void){
			return section.getObject();
		}
};
