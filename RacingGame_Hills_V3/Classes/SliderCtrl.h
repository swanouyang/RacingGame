#ifndef __SLIDER_CTRL_H__
#define __SLIDER_CTRL_H__

#include "Common.h"
using namespace std;


class SliderCtrl {
private:
	CCControlSlider* pSlider;
	double value;
public:
	void create(CCObject *target, double minValue, double maxValue, double curValue, 
		const double x, const double y, const double scale);
	void sliderAction(CCObject *pSender, CCControlEvent controlEvent);
	void setPosition(const double x, const double y);
	void setScale(const double scale);
	double getValue() const;
	CCControlSlider* getSlider() const;
};

#endif