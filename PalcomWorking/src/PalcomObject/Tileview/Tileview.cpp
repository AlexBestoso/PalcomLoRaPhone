#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

#include "../PalcomObject.h"
class PalcomObject;

#include "./Tileview.h"

void PalcomTileview::create(lv_obj_t *parent){
	this->generate(parent, pal_tileview);
}

lv_obj_t *PalcomTileview::newTile(int col, int row, int direction){
	return lv_tileview_add_tile(this->getObject(), col, row, (lv_dir_t)direction);
}

void PalcomTileview::setTile(int col, int row, bool animate){
	lv_tileview_set_tile_by_index(this->getObject(), col, row, animate ? LV_ANIM_ON : LV_ANIM_OFF);
}
