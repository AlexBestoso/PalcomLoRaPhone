struct palcom_triangle_t{
	lv_color_t color;
	int a[2];
	int b[2];
	int c[2];
};

class PalcomTriangle : public PalcomObject{
  	private:
		void (*drawCb)(lv_event_t*);
	public:
		struct palcom_triangle_t tPoints;
		static void defaultDraw(lv_event_t * e);

		PalcomTriangle();

    		void create(lv_obj_t *parent);
		void setCallback(void (*func)(lv_event_t *), void *input);
		
		void setPoints(void);
};
