#define PALCOME_SCREEN
class PalcomScreen{ 
  	private:
    		lv_obj_t *screen;
		lv_obj_t *bgImage;
    		int objectId;
		bool buildRequired;
  	public:
		PalcomObject object;
	
		PalcomScreen(void);
		~PalcomScreen();

		void clearScreenError(void);

		bool isScreenError(void);

		void setScreenError(int e);

		int getScreenError();
		bool getBuildRequired(void);
		void setBuildRequired(bool v);

    		void create(void);

    		void createGlobal(void);

    		void destroy(void);

    		void globalDestroy(void);

    		void load(void);

    		lv_obj_t *getScreen(void);

    		void setFullScreen(void);

    		/*
    		 * Direction can be:
    		 * LV_DIR_TOP, 
    		 * LV_DIR_LEFT,
    		 * LV_DIR_BOTTOM,
    		 * LV_DIR_RIGHT,
    		 * LV_DIR_HOR,
    		 * LV_DIR_VER,
    		 * LV_DIR_ALL
    		 */
    		void setScreenScrollDirection(lv_dir_t direction);
		/*
		 * Scrollbars are displayed according to a configured mode. The following mode(s) exist:
    		* LV_SCROLLBAR_MODE_OFF: Never show the scrollbars
    		* LV_SCROLLBAR_MODE_ON: Always show the scrollbars
    		* LV_SCROLLBAR_MODE_ACTIVE: Show scroll bars while an object is being scrolled
    		* LV_SCROLLBAR_MODE_AUTO: Show scroll bars when the content is large enough to be scrolled
		*/
		void setScrollMode(lv_scrollbar_mode_t mode);

    		void setBgImage(const lv_img_dsc_t *img[1]);
		void setBgX(int offset);
		void setBgY(int offset);
    
		void execute(void);
		void e(void);

		virtual void generateObjects();

		void setFlag(lv_obj_flag_t f);

                void unsetFlag(lv_obj_flag_t f);
		
		bool buildCheck(void);

		lv_obj_t *genScreen(void);
};
