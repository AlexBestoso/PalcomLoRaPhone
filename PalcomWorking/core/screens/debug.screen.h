class PalcomDebugScreen : public PalcomScreen{
	private:

	public:

		PalcomDebugScreen(){

		}

		~PalcomDebugScreen(){

		}
	
		void reset(void){
			this->setBuildRequired(true);
                        this->globalDestroy();
                        this->destroy();
                        this->clearScreenError();
		}

		void generateObjects(void){

			lv_obj_t *screen = this->genScreen();
			this->setFullScreen();
			this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
			//Serial.printf("Executing...\n");
			//this->e();

			PalcomLabel title;
			title.create(screen);
			title.setLongMode(LV_LABEL_LONG_SCROLL);
			title.setWidth(300);
			title.setText("New DBG Screen");
			title.setAlignment(LV_ALIGN_OUT_TOP_MID, 80, 20);
		}
	
		int run(void){
			this->buildCheck();
			this->execute();
			return 0;
		}
};
