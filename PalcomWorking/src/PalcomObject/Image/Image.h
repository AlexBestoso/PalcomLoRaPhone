class PalcomImage : public PalcomObject{
	private:

	public:
	PalcomImage(void);
	~PalcomImage();

	void setImageSource(const lv_image_dsc_t src);
	void create(lv_obj_t *parent);
	
	
};
