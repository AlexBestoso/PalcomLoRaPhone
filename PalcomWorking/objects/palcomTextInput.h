class PalcomTextInput : public PalcomObject{
	private:
		string labelText = "";
		string buttonText = "";
		string defaultText = "";
		string popupTitle = "";
		string popupText = "";
		string popupButtonText = "";

		void (*submitHandler)(lv_event_t*);
		void (*closePopupHandler)(lv_event_t*);

		PalcomButton submitButton;
                PalcomPopupMessage infoPopup;
                PalcomLabel titleLabel;
                PalcomLabel messageLabel;
	public:
		void setSubmitHandlerr(void (*func)(lv_event_t*)){
                        this->submitHandler = func;
                }

		void setClosePopupHandler(void (*func)(lv_event_t*)){
                        this->closePopupHandler = func;
                }
};
