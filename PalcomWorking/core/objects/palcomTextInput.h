class PalcomTextInput : public PalcomObject{
	private:
		string labelText = "";
		string buttonText = "";
		string defaultText = "";
		string popupTitle = "";
		string popupText = "";
		string popupButtonText = "";

		size_t inputWidth = 100;
		size_t inputHeight = 50;
		int inputX = 0;
		int inputY = 0;
		lv_align_t inputAlignment = LV_ALIGN_OUT_TOP_LEFT;

		void (*submitHandler)(lv_event_t*);
		void (*closePopupHandler)(lv_event_t*);

		PalcomButton submitButton;
		PalcomLabel submitButtonLabel;

		PalcomTextarea textArea;
		size_t textAreaW = 100;
		size_t textAreaH = 50;
		int  textAreaX = 0;
		int  textAreaY = 0;
		size_t textareaMaxLen = 15;
		size_t textAreaGlobalId = 1;

		PalcomLabel inputLabel;

                PalcomPopupMessage infoPopup;
                PalcomLabel titleLabel;
                PalcomLabel messageLabel;

	public:
		void setTextareaX(int x){
			this->textAreaX = x;
		}

		void setTextareaY(int y){
			this->textAreaY = y;
		}
		
		void setTextareaId(size_t id){
			this->textAreaGlobalId = id;
		}

		void setTextareaWidth(size_t w){
			this->textAreaW = w;
		}

		void setTextareaHeight(size_t h){
			this->textAreaH = h;
		}
		
		void setLabelText(string s){
			this->labelText = s;
		}
                
		void setButtonText(string s){
			this->buttonText = s;
		}
                
		void setDefaultText(string s){
			this->defaultText = s;
		}
                
		void setPopupTitle(string s){
			this->popupTitle = s;
		}
                
		void setPopupText(string s){
			this->popupText = s;
		}

                void setPopupButtonText(string s){
			this->popupButtonText = s;
		}

                void setInputWidth(size_t w){
			this->inputWidth = w;
		}
                
		void setInputHeight(size_t h){
			this->inputHeight = h;
		}

		void setInputX(int x){
			this->inputX = x;
		}

		void setInputY(int y){
			this->inputY = y;
		}

		void setSubmitHandlerr(void (*func)(lv_event_t*)){
                        this->submitHandler = func;
                }

		void setClosePopupHandler(void (*func)(lv_event_t*)){
                        this->closePopupHandler = func;
                }

		void setStyle(lv_style_t *style){
                        this->setDefaultStyle(style);
                }

		void maintainText(void){
			PalcomTextarea grabber;
			grabber.loadGlobal(textAreaGlobalId);
			this->defaultText = grabber.getText();
		}

		void setMaxLength(size_t v){
			this->textareaMaxLen = v;
		}

		void create(lv_obj_t *parent){
			// Initalize object
			this->generate(parent, pal_base);
                        this->setSizeRaw(this->inputWidth, this->inputHeight);
			this->setAlignment(this->inputAlignment, this->inputX, this->inputY);
			this->setScrollMode(LV_SCROLLBAR_MODE_OFF);
			this->execute();

			// Create inputt Label
			if(labelText != ""){
				inputLabel.create(this->getObject());
				inputLabel.setText(labelText.c_str());
				this->execute();
			}

			// Create textarea
			textArea.createGlobal(this->getObject(), this->textAreaGlobalId);
			textArea.setSizeRaw(this->textAreaW, this->textAreaH);
			textArea.setAlignment(LV_ALIGN_OUT_TOP_LEFT, this->textAreaX, this->textAreaY);
			defaultTextareaStyle.initStyle();
			textArea.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
			textArea.setOneLine(true);
			textArea.setMaxLength(this->textareaMaxLen);
			textArea.setText(this->defaultText.c_str());

			// Create SubmitButton
			submitButton.create(this->getObject());
			submitButton.setSimpleCallback(this->submitHandler);
			defaultButtonStyle.initStyle();
			submitButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
			submitButton.setSizeRaw(60, 50); // use class variables
			submitButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, 255,  50); // use class variables
			submitButtonLabel.create(submitButton.getObj());
			submitButtonLabel.setText(this->buttonText.c_str());
			submitButtonLabel.center();
			submitButton.setLabel(submitButtonLabel);
			

			this->execute();
		}
};
