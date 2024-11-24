class PalcomDebugScreen : public PalcomScreen{
	private:

	public:
		bool test = false;

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

			Serial.printf("Generating Screen Objects...\n");
			lv_obj_t *screen = this->genScreen();
			Serial.printf("Setting full screen.\n");
			this->setFullScreen();
			this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
			//Serial.printf("Executing...\n");
			//this->e();

			Serial.printf("Creating Title...\n");
			PalcomLabel title;
			title.create(screen);
			title.setLongMode(LV_LABEL_LONG_SCROLL);
			title.setWidth(300);
			if(!test)
				title.setText("Debug Screen");
			else
				title.setText("Filthy niggers...");
			title.setAlignment(LV_ALIGN_OUT_TOP_MID, 80, 20);
		}
	
		int run(void){
			this->buildCheck();
			this->execute();
			return 0;
		}
};
