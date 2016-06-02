#include "SliderCtrl.h"

void SliderCtrl::create(CCObject *target, double minValue, double maxValue, double curValue, 
	const double x, const double y, const double scale) {
	CCControlSlider* slider = CCControlSlider::create(
		"sliderTrack2.png", "sliderProgress2.png", "sliderThumb.png");
	//slider->addTargetWithActionForControlEvents(target,
	//	cccontrol_selector(SliderCtrl::sliderAction), CCControlEventValueChanged);
	slider->setMinimumValue(minValue);
	slider->setMaximumValue(maxValue);
	slider->setValue(curValue);
	slider->setPosition(x, y);
	slider->setScale(scale);
	this->value = curValue;
	this->pSlider = slider;
}

void SliderCtrl::setPosition(const double x, const double y) {
	pSlider->setPosition(x, y);
}

void SliderCtrl::setScale(const double scale) {
	pSlider->setScale(scale);
}

//void SliderCtrl::sliderAction(CCObject *pSender, CCControlEvent controlEvent) {
//	CCControlSlider* slider = (CCControlSlider*)pSender;
//	this->value = slider->getValue();//this->value会自动改回，搞不明白
//}

double SliderCtrl::getValue() const{
	return this->value;//this->value会自动改回，搞不明白
}

CCControlSlider* SliderCtrl::getSlider() const {
	return pSlider;
}