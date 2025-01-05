#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include "../PalcomObject.h"
class PalcomObject;

#include "./Image.h"

PalcomImage::PalcomImage(void){}
PalcomImage::~PalcomImage(){}

void PalcomImage::setImageSource(const lv_image_dsc_t src){
	const lv_img_dsc_t *img_src[1] = {&src};
	lv_image_set_src(this->getObject(), (void *)img_src[0]);
}
void PalcomImage::create(lv_obj_t *parent){
	this->generate(parent, pal_image);
}
