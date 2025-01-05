#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include "./PalcomStyle.h"
/*
* You must call this function before running any other functions.
* */
void PalcomStyle::setStyle(lv_style_t *s){
	this->targetStyle = s;
}
lv_style_t *PalcomStyle::getStyle(void){
	return this->targetStyle;
}

void PalcomStyle::init(void){
	lv_style_init(this->targetStyle);
}

lv_state_t PalcomStyle::getStateDefault(void){
	return LV_STATE_DEFAULT;
}

lv_state_t PalcomStyle::getStateChecked(void){
	return LV_STATE_CHECKED;
}

lv_state_t PalcomStyle::getStateFocused(void){
	return LV_STATE_FOCUSED;
}

lv_state_t PalcomStyle::getStateFocusKey(void){
	return LV_STATE_FOCUS_KEY;
}

lv_state_t PalcomStyle::getStateEdited(void){
	return LV_STATE_EDITED;
}

lv_state_t PalcomStyle::getStateHovered(void){
	return LV_STATE_HOVERED;
}

lv_state_t PalcomStyle::getStatePressed(void){
	return LV_STATE_PRESSED;
}

lv_state_t PalcomStyle::getStateScrolled(void){
	return LV_STATE_SCROLLED;
}

lv_state_t PalcomStyle::getStateDisabled(void){
	return LV_STATE_DISABLED;
}

lv_state_t PalcomStyle::getStateCustomOne(void){
	return LV_STATE_USER_1;
}

lv_state_t PalcomStyle::getStateCustomTwo(void){
	return LV_STATE_USER_2;
}

lv_state_t PalcomStyle::getStateCustomThree(void){
	return LV_STATE_USER_3;
}

lv_state_t PalcomStyle::getStateCustomFour(void){
	return LV_STATE_USER_4;
}

void PalcomStyle::setColorFilter(lv_color_filter_dsc_t *f, lv_color_t (*colorFilterHandler)(const lv_color_filter_dsc_t *, lv_color_t, lv_opa_t)){
	this->targetColorFilter = f;
	this->colorFilterHandler = colorFilterHandler;
}
void PalcomStyle::initColorFilter(void){
	lv_color_filter_dsc_init(targetColorFilter, colorFilterHandler);
}

void PalcomStyle::setRadius(int r){
	lv_style_set_radius(this->targetStyle, r);
}

void PalcomStyle::setBgOpacity(int o){
	switch(o){
		case 0:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_0);
			break;
		case 1:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_10);
			break;
		case 2:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_20);
			break;
		case 3:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_30);
			break;
		case 4:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_40);
			break;
		case 5:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_50);
			break;
		case 6:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_60);
			break;
		case 7:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_70);
			break;
		case 8:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_80);
			break;
		case 9:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_90);
			break;
		case 10:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_100);
			break;
		case -1:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_TRANSP);
			break;
		default:
			lv_style_set_bg_opa(this->targetStyle, LV_OPA_COVER);
			break;
	}
}

lv_color_t PalcomStyle::makeColor(int r, int g, int b){
	return lv_color_make(r, g, b);
}

lv_color_t PalcomStyle::makeColorHex(int h){
	return lv_color_hex(h);
}

lv_color_t PalcomStyle::makeColorHex3(int h){
	return lv_color_hex3(h);
}

void PalcomStyle::setBgColor(lv_color_t c){
	lv_style_set_bg_color(this->targetStyle, c);	
}

void PalcomStyle::setBgGradientColor(lv_color_t c){
	lv_style_set_bg_grad_color(this->targetStyle, c);
}

void PalcomStyle::setBgGradientDirection(int d){
	switch(d){
		case 1:
			lv_style_set_bg_grad_dir(this->targetStyle, LV_GRAD_DIR_HOR);
			break;
		case 2: 
			lv_style_set_bg_grad_dir(this->targetStyle, LV_GRAD_DIR_VER);
			break;
		default:
			lv_style_set_bg_grad_dir(this->targetStyle, LV_GRAD_DIR_NONE);
			break;
	}
}

void PalcomStyle::setBorderColor(lv_color_t c){
	lv_style_set_border_color(this->targetStyle, c);
}

void PalcomStyle::setBorderOpacity(int o){
	switch(o){
		case 0:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_0);
			break;
		case 1:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_10);
			break;
		case 2:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_20);
			break;
		case 3:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_30);
			break;
		case 4:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_40);
			break;
		case 5:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_50);
			break;
		case 6:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_60);
			break;
		case 7:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_70);
			break;
		case 8:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_80);
			break;
		case 9:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_90);
			break;
		case 10:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_100);
			break;
		case -1:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_TRANSP);
			break;
		default:
			lv_style_set_border_opa(this->targetStyle, LV_OPA_COVER);
			break;
	}
}

void PalcomStyle::setAnimationTime(int t){
	lv_style_set_anim_time(this->targetStyle, t);
}
void PalcomStyle::setBorderWidth(int w){
	lv_style_set_border_width(this->targetStyle, w);
}

/*
 * LV_BORDER_SIDE_NONE/TOP/BOTTOM/LEFT/RIGHT/INTERNAL
 *  Can be ORed '||' to use multiple.
 *  */
void PalcomStyle::setBorderSide(lv_border_side_t s){
	lv_style_set_border_side(this->targetStyle, s);
}

void PalcomStyle::setTextColor(lv_color_t c){
	lv_style_set_text_color(this->targetStyle, c);
}

/*
 * LV_TEXT_ALIGN_AUTO
  * LV_TEXT_ALIGN_LEFT
 * LV_TEXT_ALIGN_CENTER
 * LV_TEXT_ALIGN_RIGHT
 * */
void PalcomStyle::setTextAlign(lv_text_align_t a){
	lv_style_set_text_align(this->targetStyle, a);	
}

void PalcomStyle::applyColorFilter(){
	lv_style_set_color_filter_dsc(this->targetStyle, this->targetColorFilter);
}

void PalcomStyle::setColorFilterOpacity(int o){
	switch(o){
		case 0:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_0);
			break;
		case 1:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_10);
			break;
		case 2:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_20);
			break;
		case 3:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_30);
			break;
		case 4:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_40);
			break;
		case 5:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_50);
			break;
		case 6:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_60);
			break;
		case 7:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_70);
			break;
		case 8:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_80);
			break;
		case 9:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_90);
			break;
		case 10:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_100);
			break;
		case -1:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_TRANSP);
			break;
		default:
			lv_style_set_color_filter_opa(this->targetStyle, LV_OPA_COVER);
			break;
	}
}

void PalcomStyle::recolorImgOpacity(int o){
	switch(o){
	case 0:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_0);
		break;
	case 1:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_10);
		break;
	case 2:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_20);
		break;
	case 3:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_30);
		break;
	case 4:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_40);
		break;
	case 5:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_50);
		break;
	case 6:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_60);
		break;
	case 7:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_70);
		break;
	case 8:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_80);
		break;
	case 9:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_90);
		break;
	case 10:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_100);
		break;
	case -1:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_TRANSP);
		break;
	default:
		lv_style_set_img_recolor_opa(this->targetStyle, LV_OPA_COVER);
		break;
	}
}

/*
 *   enumerator LV_OPA_TRANSP = 0
 *   enumerator LV_OPA_0 = 0
 *   enumerator LV_OPA_10 = 25
 *   enumerator LV_OPA_20 = 51
 *   enumerator LV_OPA_30 = 76
 *   enumerator LV_OPA_40 = 102
 *   enumerator LV_OPA_50 = 127
 *   enumerator LV_OPA_60 = 153
 *   enumerator LV_OPA_70 = 178
 *   enumerator LV_OPA_80 = 204
 *   enumerator LV_OPA_90 = 229
 *   enumerator LV_OPA_100 = 255
 *   enumerator LV_OPA_COVER = 255
 */

void PalcomStyle::recolorImg(lv_color_t c){
	lv_style_set_img_recolor(this->targetStyle, c);
}

void PalcomStyle::setShadowWidth(int w){
	lv_style_set_shadow_width(this->targetStyle, w);
}

void PalcomStyle::setShadowColor(lv_color_t c){
	lv_style_set_shadow_color(this->targetStyle, c);
}

void PalcomStyle::setShadowOffsetY(int offset){
	lv_style_set_shadow_ofs_y(this->targetStyle, offset);
}
void PalcomStyle::setShadowSpread(int s){
	lv_style_set_shadow_spread(this->targetStyle, s);
}

void PalcomStyle::setOutlineOpacity(int o){
	switch(o){
		case 0:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_0);
			break;
		case 10:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_10);
			break;
		case 20:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_20);
			break;
		case 30:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_30);
			break;
		case 40:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_40);
			break;
		case 50:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_50);
			break;
		case 60:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_60);
			break;
		case 70:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_70);
			break;
		case 80:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_80);
			break;
		case 90:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_90);
			break;
		case 100:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_100);
			break;
		case -1:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_TRANSP);
			break;
		default:
			lv_style_set_outline_opa(this->targetStyle, LV_OPA_COVER);
			break;
	}
}

void PalcomStyle::setOutlineColor(lv_color_t c){
	lv_style_set_outline_color(this->targetStyle, c);
}

void PalcomStyle::setOutlineWidth(int w){
	lv_style_set_outline_width(this->targetStyle, w);
}

void PalcomStyle::setAllPadding(int p){
	lv_style_set_pad_all(this->targetStyle, p);
}

void PalcomStyle::setHorizontalPadding(int p){
	lv_style_set_pad_hor(this->targetStyle, p);
}

void PalcomStyle::setVirticalPadding(int p){
	lv_style_set_pad_ver(this->targetStyle, p);
}

void PalcomStyle::setPaddingGap(int p){
	lv_style_set_pad_gap(this->targetStyle, p);
}

void PalcomStyle::setTranslateY(int y){
	lv_style_set_translate_y(this->targetStyle, y);
}

void PalcomStyle::setTransition(lv_style_transition_dsc_t *t){
	lv_style_set_transition(this->targetStyle, t);
}
