class PalcomPopupMessage : public PalcomObject{
	private:
		string title = "title";
		string message = "message";
		string closeMessage = "Close";
		int popupWidth = 200;
		int popupHeight = 200;
		int popupType = 1;

		PalcomButton closeButton;
		PalcomButton yesButton;
		PalcomButton noButton;
		PalcomLabel titleLabel;
		PalcomLabel messageLabel;

		void (*closeMessageHandler)(lv_event_t*);
		void (*yesMessageHandler)(lv_event_t*);
		void (*noMessageHandler)(lv_event_t*);
	public:
		void setPopupType(int popupType){
			this->popupType = popupType;
		}
		void setCloseHandler(void (*func)(lv_event_t*)){
			this->closeMessageHandler = func;
		}
		
		void setYesHandler(void (*func)(lv_event_t*)){
			this->yesMessageHandler = func;
		}
		
		void setNoHandler(void (*func)(lv_event_t*)){
			this->noMessageHandler = func;
		}

		void setTitle(string t){
			title = t;
		}
		
		void setMessage(string m){
			message = m;
		}

		void setStyle(lv_style_t *style){
			this->setDefaultStyle(style);
		}
		
		void create(lv_obj_t *parent){
			this->generate(parent, pal_base);
			this->setSize(popupWidth, popupHeight);

			titleLabel.create(this->getObject());
			titleLabel.setText(title.c_str());
			titleLabel.setAlignment(LV_ALIGN_OUT_TOP_MID, 0, 0);
			this->execute();

			messageLabel.create(this->getObject());
			messageLabel.setText(message.c_str());
			messageLabel.setAlignment(LV_ALIGN_OUT_LEFT_MID, 0, 50);
			this->execute();

			Serial.printf("Creating popup of type %d\n", popupType);

			if(popupType == 2){ // Binary or Yes/no Buttons
				// yes button
				Serial.printf("Creating Binary Popup.\n");
				yesButton.create(this->getObject());
                                defaultButtonStyle.initStyle();
                                yesButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                                yesButton.setSize(15, 15);
                                PalcomLabel butlabel;
                                butlabel.create(yesButton.getObj());
                                butlabel.setText("Yes");
                                butlabel.center();
                                yesButton.setLabel(butlabel);
                                yesButton.setSimpleCallback(this->yesMessageHandler);
                                yesButton.setRelativeAlignment(LV_ALIGN_BOTTOM_LEFT, -20, 160);
				this->execute();

				// No button
                                noButton.create(this->getObject());
                                defaultButtonStyle.initStyle();
                                noButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                                noButton.setSize(15, 15);
                                butlabel.create(noButton.getObj());
                                butlabel.setText("No");
                                butlabel.center();
                                noButton.setLabel(butlabel);
                                noButton.setSimpleCallback(this->noMessageHandler);
                                noButton.setRelativeAlignment(LV_ALIGN_BOTTOM_RIGHT, 200, 160);
				this->execute();
			}
			
			if(popupType == 1){
				Serial.printf("Creating single optioned popup.\n");
				closeButton.create(this->getObject());
				defaultButtonStyle.initStyle();
				closeButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
				closeButton.setSize(15, 15);
				PalcomLabel butlabel; 
				butlabel.create(closeButton.getObj());
				butlabel.setText(closeMessage.c_str());
				butlabel.center();
				closeButton.setLabel(butlabel);
				closeButton.setSimpleCallback(this->closeMessageHandler);
				closeButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 50, 160);
				this->execute();
			}
		}	
};
