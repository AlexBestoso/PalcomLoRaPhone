class PalcomPopupMessage : public PalcomObject{
	private:
		string title = "title";
		string message = "message";
		string closeMessage = "Close";
		int popupType = 0;
		int popupWidth = 200;
		int popupHeight = 200;

		PalcomButton closeButton;
		PalcomLabel titleLabel;
		PalcomLabel messageLabel;

		void (*closeMessageHandler)(lv_event_t*);
	public:
		void setCloseHandler(void (*closeMessageHandler)(lv_event_t*)){
			this->closeMessageHandler = closeMessageHandler;
		}
		void setTitle(string t){
			title = t;
		}
		
		void setMessage(string m){
			message = m;
		}

		void setStyle(lv_style_t *style){
			Serial.printf("Setting default style.\n");
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
};
