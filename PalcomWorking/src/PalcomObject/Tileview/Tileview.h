class PalcomTileview : public PalcomObject{
	private:

	public:
		void create(lv_obj_t *parent);
		lv_obj_t *newTile(int col, int row, int direction);
		void setTile(int col, int row, bool animate);
};
