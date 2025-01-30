#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

class PalcomObject;
#include "../PalcomObject.h"

#include "./Line.h"

void PalcomLine::create(lv_obj_t *parent){
	this->generate(parent, pal_line);
}

void PalcomLine::vertexPush(lv_point_precise_t *points, size_t pointsCount){
	lv_line_set_points(this->getObject(), points, pointsCount);
}

