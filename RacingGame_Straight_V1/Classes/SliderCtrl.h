#ifndef __SLIDER_CTRL_H__
#define __SLIDER_CTRL_H__

#include "Common.h"
using namespace std;


class SliderCtrl {
private:
	CCControlSlider* pSlider;
	float value;
public:
	void create(CCObject *target, float minValue, float maxValue, float curValue, 
		const float x, const float y, const float scale);
	void sliderAction(CCObject *pSender, CCControlEvent controlEvent);
	void setPosition(const float x, const float y);
	void setScale(const float scale);
	float getValue() const;
	CCControlSlider* getSlider() const;
};

#endif