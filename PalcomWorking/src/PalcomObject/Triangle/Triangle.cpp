#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomEvent/PalcomEvent.h>
#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

class PalcomObject;
#include "../PalcomObject.h"

#include "./Triangle.h"

void PalcomTriangle::defaultDraw(lv_event_t * e){
	PalcomEvent event(e);
	if(event.getCode() != LV_EVENT_DRAW_MAIN_END)
		return;
	struct palcom_triangle_t *triangle = (struct palcom_triangle_t*)event.getUserData();

    	lv_draw_triangle_dsc_t dsc;
    	lv_draw_triangle_dsc_init(&dsc);
	if(triangle == NULL){
		Serial.printf("Provided Triangle is Null\n");
		/*dsc.p[0].x = 10;
    		dsc.p[0].y = 10;
    		dsc.p[1].x = 10;
    		dsc.p[1].y = 50;
    		dsc.p[2].x = 50;
    		dsc.p[2].y = 50;*/
	}else{
		Serial.printf("Non-null triangle-> (%d, %d)\n", triangle->a[0], triangle->a[1]);
		dsc.p[0].x = triangle->a[0];
    		dsc.p[0].y = triangle->a[1];
    		dsc.p[1].x = triangle->b[0];
    		dsc.p[1].y = triangle->b[1];
    		dsc.p[2].x = triangle->c[0];
    		dsc.p[2].y = triangle->c[1];
	}

    	dsc.bg_color = lv_palette_main(LV_PALETTE_GREEN);
    	// dsc.bg_opa = ...;
    	// dsc.bg_grad = ...;

    	lv_obj_t * my_obj = lv_event_get_target_obj(e);
    	lv_area_t my_obj_coords;
    	lv_obj_get_coords(my_obj, &my_obj_coords);
    	for(int i = 0; i < 3; i++) {
    	    dsc.p[i].x += my_obj_coords.x1;
    	    dsc.p[i].y += my_obj_coords.y1;
    	}

    	lv_layer_t * layer = lv_event_get_layer(e);
    	lv_draw_triangle(layer, &dsc);
}


PalcomTriangle::PalcomTriangle() : PalcomObject(){
	drawCb = &defaultDraw;
	//this->setDrawMainEndCallback(&drawCb, NULL);
}
void PalcomTriangle::create(lv_obj_t *parent){
	this->generate(parent, pal_triangle);
	this->setDrawMainEndCallback(drawCb, NULL);
}


void PalcomTriangle::setCallback(void (*func)(lv_event_t *), void *input){
	//drawCb = &defaultDraw;
	this->setDrawMainEndCallback(func, input);
}

void PalcomTriangle::setPoints(void){
	this->setDrawMainEndCallback(this->drawCb, &this->tPoints);
}
