class PalcomTabview : public PalcomObject{
	private:
		void setTabBarPosition(lv_dir_t dir);
	public:
		void create(lv_obj_t *parent);
		
		void renameTab(uint32_t idx, const char *new_name);
		void makeTabActive(uint32_t idx, bool animation);
		
		void tabBarPositionNone(void);
		void tabBarPositionLeft(void);
		void tabBarPositionRight(void);
		void tabBarPositionTop(void);
		void tabBarPositionBottom(void);
		void tabBarPositionHorizontal(void);
		void tabBarPositionVertical(void);
		void tabBarPositionAll(void);

		void setTabBarSize(int32_t size);

		int tabCount(void);
		int getActiveTab(void);

		lv_obj_t *newTab(const char *name);
		lv_obj_t *getContent(void);
		lv_obj_t *getTabBar(void);
};
