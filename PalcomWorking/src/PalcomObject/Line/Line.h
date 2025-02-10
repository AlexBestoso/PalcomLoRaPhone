class PalcomLine : public PalcomObject{
  	private:
	public:
		PalcomLine();
		PalcomLine(bool useBaseStyle);
                PalcomLine(lv_style_t *baseStyle);

    		void create(lv_obj_t *parent);
		void vertexPush(lv_point_precise_t *points, size_t pointsCount);
		void setLineWidth(int width);
		void setLineColor(lv_color_t color);
		void setLineRounded(bool active);
};
