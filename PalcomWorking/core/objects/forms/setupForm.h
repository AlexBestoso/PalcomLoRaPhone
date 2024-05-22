PalcomTextarea *setupForm_name = NULL;
PalcomTextarea *setupForm_lockTime = NULL;
PalcomCheckbox *setupForm_pmode = NULL;

#define SETUPFORM_ERROR_NONE 0
#define SETUPFORM_ERROR_EMPTYNAME 1
int setupForm_error = SETUPFORM_ERROR_NONE;
bool setupForm_submitted = false;

class PalcomSetupForm : public PalcomObject{
	private:
		
		bool validation_name = false;
		bool validation_lockTimer = false;

		PalcomLabel helpLabel;
		PalcomLabel paranoiaModeLabel;
		PalcomLabel lockTimerLabel;

		PalcomTextarea nameTextarea;
		PalcomTextarea lockTimerTextarea;
		PalcomCheckbox pcb;

		PalcomButton helpButton;
		PalcomButton submitButton;

		static void helpFunc_name(lv_event_t* e){
			if(lv_event_get_code(e) == LV_EVENT_RELEASED){
				PalcomMessageBox pmbox;
				pmbox.create(NULL, "Help", "The name input is used for caller ID and relay authentication.", {NULL}, true);
				pmbox.setAlignment(LV_ALIGN_TOP_LEFT, 40, 40);

			}
		}

		static void helpFunc_pmode(lv_event_t* e){
                        if(lv_event_get_code(e) == LV_EVENT_RELEASED){
                                PalcomMessageBox pmbox;
                                pmbox.create(NULL, "Help", "P-mode or, 'paranoia mode', makes it so that after 10 failed login attempts your device's SD card is wiped.", {NULL}, true);
                                pmbox.setAlignment(LV_ALIGN_TOP_LEFT, 40, 40);

                        }
                }

		static void helpFunc_lock(lv_event_t* e){
                        if(lv_event_get_code(e) == LV_EVENT_RELEASED){
                                PalcomMessageBox pmbox;
                                pmbox.create(NULL, "Help", "The lock timer is used to determine how many minutes of inactivity needs to pass before automatically locking the screen. The default is 3 minutes. Set to 0 to disable.", {NULL}, true);
                                pmbox.setAlignment(LV_ALIGN_TOP_LEFT, 40, 40);

                        }
                }

		static void submitForm(lv_event_t *e){
			if(lv_event_get_code(e) == LV_EVENT_RELEASED){
				setupForm_error = SETUPFORM_ERROR_NONE;
				bool pModeActive = lv_obj_get_state(setupForm_pmode->getObject()) & LV_STATE_CHECKED ? true : false;
				String name = setupForm_name->getText();
				if(name == ""){
					setupForm_name->setStyle(defaultTextareaStyle.getErrorStyle(), defaultTextareaStyle.getFocusedStyle());
					setupForm_error = SETUPFORM_ERROR_EMPTYNAME;
				}else{
					setupForm_name->setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
				}
				String lockTime = setupForm_lockTime->getText();
				if(lockTime == ""){
					lockTime = "3";
					setupForm_lockTime->setText("3");
				}

				if(setupForm_error == SETUPFORM_ERROR_NONE){
					setupForm_submitted = true;
				}else{
					setupForm_submitted = false;
				}
			}
		}
		void createNameInput(lv_obj_t *parent){
			defaultTextareaStyle.initStyle();
			nameTextarea.create(parent);
			nameTextarea.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
			nameTextarea.setAnyStyle(defaultTextareaStyle.getCursorStyle(), LV_STATE_FOCUSED | LV_PART_CURSOR);
			nameTextarea.setStylePlaceholder(defaultTextareaStyle.getPlaceHolderStyle());
			nameTextarea.setOneLine(true);
			nameTextarea.setMaxLength(20);
			nameTextarea.setAlignment(LV_ALIGN_TOP_LEFT, 10, 40+20);
			nameTextarea.setTextPlaceholder("Enter Your Name");
			nameTextarea.setSize(this->getHorizontalResolution()-75, 35);

			style_pinpadButton.initStyle();
			helpButton.create(parent);
			helpLabel.create(helpButton.getObject());
			helpLabel.setText("?");
                        helpLabel.center();
			helpButton.setLabel(helpLabel);
			helpButton.setDefaultStyle(style_pinpadButton.getStyle());
                        helpButton.setPressedStyle(style_pinpadButton.getPressedStyle());
			helpButton.setSize(10, 10);
			helpButton.setAlignment(LV_ALIGN_OUT_TOP_RIGHT, 260, 40+23);
			helpButton.setSimpleCallback(&helpFunc_name);
		}

		void createPModeInput(lv_obj_t *parent){
			pcb.create(parent);
			pcb.setText("P-Mode");
			pcb.unsetChecked();
			pcb.unsetDisabled();
			pcb.setAlignment(LV_ALIGN_OUT_TOP_RIGHT,  40, 120-10);

			style_pinpadButton.initStyle();
                        helpButton.create(parent);
                        helpLabel.create(helpButton.getObject());
                        helpLabel.setText("?");
                        helpLabel.center();
                        helpButton.setLabel(helpLabel);
                        helpButton.setDefaultStyle(style_pinpadButton.getStyle());
                        helpButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        helpButton.setSize(10, 10);
                        helpButton.setAlignment(LV_ALIGN_OUT_TOP_RIGHT, 190, 115-10);
                        helpButton.setSimpleCallback(&helpFunc_pmode);
		}

		void createLockTimerInput(lv_obj_t *parent){
			defaultTextareaStyle.initStyle();
                        lockTimerTextarea.create(parent);
                        lockTimerTextarea.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
                        lockTimerTextarea.setAnyStyle(defaultTextareaStyle.getCursorStyle(), LV_STATE_FOCUSED | LV_PART_CURSOR);
                        lockTimerTextarea.setStylePlaceholder(defaultTextareaStyle.getPlaceHolderStyle());
                        lockTimerTextarea.setOneLine(true);
                        lockTimerTextarea.setMaxLength(20);
                        lockTimerTextarea.setAlignment(LV_ALIGN_TOP_LEFT, 10, 150);
                        lockTimerTextarea.setTextPlaceholder("Screen Lock Time (default: 3)");
                        lockTimerTextarea.setSize(this->getHorizontalResolution()-75, 35);
			lockTimerTextarea.setValidChars("0123456789");

			style_pinpadButton.initStyle();
                        helpButton.create(parent);
                        helpLabel.create(helpButton.getObject());
                        helpLabel.setText("?");
                        helpLabel.center();
                        helpButton.setLabel(helpLabel);
                        helpButton.setDefaultStyle(style_pinpadButton.getStyle());
                        helpButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        helpButton.setSize(10, 10);
                        helpButton.setAlignment(LV_ALIGN_OUT_TOP_RIGHT, 260, 150+3);
                        helpButton.setSimpleCallback(&helpFunc_lock);
		}

		void createSubmitButton(lv_obj_t *parent){
			style_pinpadButton.initStyle();
			submitButton.create(parent);
			PalcomLabel submitLabel;
			submitLabel.create(submitButton.getObject());
			submitLabel.setText("Complete Setup");
			submitLabel.center();
			submitButton.setLabel(submitLabel);
			submitButton.setDefaultStyle(style_pinpadButton.getStyle());
			submitButton.setPressedStyle(style_pinpadButton.getPressedStyle());
			submitButton.setSize(50, 15);
			submitButton.setAlignment(LV_ALIGN_OUT_TOP_RIGHT, 60, 193);
			submitButton.setSimpleCallback(&submitForm);
		}

		void clearFlags(void){
			setupForm_error = SETUPFORM_ERROR_NONE;
			setupForm_submitted = false;

		}

	public:
		String name = "";
		bool paranoiaMode = false;
		int lockTimer = 3; // in minutes

		PalcomSetupForm(void){
			setupForm_name = &nameTextarea;
			setupForm_lockTime = &lockTimerTextarea;
			setupForm_pmode = &pcb;
		}

		void create(lv_obj_t *parent, String title){
			this->generate(parent, pal_base);
			this->setSize(this->getHorizontalResolution()-20, this->getVirticalResolution());
			
			PalcomLabel titleLabel;
			titleLabel.create(parent);
                        this->execute();
                        titleLabel.setLongMode(LV_LABEL_LONG_SCROLL);
                        titleLabel.setWidth(300);
                        titleLabel.setAlignment(LV_ALIGN_TOP_MID, 80, 20);
                        titleLabel.setText(title.c_str());
                        this->execute();

			createNameInput(parent);
			this->execute();

			createPModeInput(parent);
			this->execute();

			createLockTimerInput(parent);
			this->execute();

			createSubmitButton(parent);
			this->execute();
		}

		int getError(void){
			return setupForm_error;
		}

		bool formErrored(void){
			if(setupForm_error > 0){
				return true;
			}
			return false;
		}

		bool formSubmitted(void){
			if(setupForm_error == SETUPFORM_ERROR_NONE && setupForm_submitted){
				this->paranoiaMode = lv_obj_get_state(pcb.getObject()) & LV_STATE_CHECKED ? true : false;
                                this->name = nameTextarea.getText();
                                this->lockTimer = stoi(lockTimerTextarea.getText());
				return true;
			}else{
				return false;
			}
		}
};
