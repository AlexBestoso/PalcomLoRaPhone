#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

class PalcomObject;
#include "../PalcomObject.h"

#include "./Line.h"


PalcomLine::PalcomLine() : PalcomObject(){

}
PalcomLine::PalcomLine(bool useBaseStyle) : PalcomObject(useBaseStyle){
	
}
PalcomLine::PalcomLine(lv_style_t *baseStyle) : PalcomObject(baseStyle){
	Serial.printf("Setting base style...\n");
}
void PalcomLine::create(lv_obj_t *parent){
	this->generate(parent, pal_line);
}

void PalcomLine::vertexPush(lv_point_precise_t *points, size_t pointsCount){
	lv_line_set_points(this->getObject(), points, pointsCount);
}

void PalcomLine::setLineWidth(int width){
	lv_style_set_line_width(this->getStyle(), width);
}
void PalcomLine::setLineColor(lv_color_t color){
	lv_style_set_line_color(this->getStyle(), color);
}
void PalcomLine::setLineRounded(bool active){
	lv_style_set_line_rounded(this->getStyle(), active);
}
