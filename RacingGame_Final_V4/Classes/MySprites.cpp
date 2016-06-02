#include "MySprites.h"

MySprites* MySprites::s_MySprites = nullptr;

MySprites::MySprites() {

}

MySprites* MySprites::getInstance() {
	if (!s_MySprites) {
		s_MySprites = new MySprites();
	}
	return s_MySprites;
}

void MySprites::destroy() {
	if (s_MySprites) {
		delete s_MySprites;
		s_MySprites = nullptr;
	}
}

bool MySprites::init() {
	this->frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	this->frameCache->addSpriteFramesWithFile("sprites.plist");

	return true;
}

void MySprites::setDisplayFrame(CCSprite * sprite, string name) {
	CCSpriteFrame *frame = frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str());
	sprite->setDisplayFrame(frame);
	double width = frame->getRect().size.width;
	double height = frame->getRect().size.height;

}

double MySprites::getWidth(CCSprite* sprite) {
	return sprite->getTextureRect().size.width;
}
double MySprites::getWidth(string name) {
	return this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->getRect().size.width;
}

double MySprites::getHeight(CCSprite* sprite) {
	return sprite->getTextureRect().size.height;
}

double MySprites::getHeight(string name) {
	return this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->getRect().size.height;
}

double MySprites::getOriginX(string name) {
	return this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->getRect().origin.x;
}
double MySprites::getOriginY(string name) {
	return this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->getRect().origin.y;
}

bool MySprites::isRotated(string name) const {
	return this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->isRotated();
}

void MySprites::getWidthAndHeight(CCSprite* sprite, double& width, double& height) {
	width = sprite->getTextureRect().size.width;
	height = sprite->getTextureRect().size.height;
}
void MySprites::getWidthAndHeight(string name, double& width, double& height) {
	width = this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->getRect().size.width;
	height = this->frameCache->spriteFrameByName((g_SpriteMap.at(name)).c_str())->getRect().size.height;
}

double MySprites::getPicWidth(CCSprite* sprite) {
	return sprite->getTexture()->getContentSize().width;
}
double MySprites::getPicHeight(CCSprite* sprite) {
	return sprite->getTexture()->getContentSize().height;
}

void MySprites::getPicWidthAndHeight(CCSprite* sprite, double& width, double& height) {
	width = sprite->getTexture()->getContentSize().width;
	height = sprite->getTexture()->getContentSize().height;
}
//===================================================================
// MyOneSprite
//===================================================================
MyOneSprite::MyOneSprite() {

}
MyOneSprite::MyOneSprite(string path, double offset) {
	this->path = path;
	this->offset = offset;
}
void MyOneSprite::setPath(string path) {
	this->path = path;
}
void MyOneSprite::setOffset(double offset) {
	this->offset = offset;
}
string MyOneSprite::getPath() const {
	return this->path;
}
double MyOneSprite::getOffset() const {
	return this->offset;
}



//===================================================================
// MyOneCar
//===================================================================
MyOneCar::MyOneCar() {

}
bool MyOneCar::operator==(MyOneCar car) {
	if (this->path == car.getPath()
		&& this->offset == car.getOffset()
		&& this->speed == car.getSpeed()
		&& this->z == car.getZ()
		&& this->percent == car.getPercent()) {
		return true;
	}
	return false;
}
void MyOneCar::setIndex(int index) {
	this->index = index;
}
int MyOneCar::getIndex() const {
	return this->index;
}
void MyOneCar::setZ(double z) {
	this->z = z;
}
double MyOneCar::getZ() const {
	return this->z;
}
void MyOneCar::setSpeed(double speed) {
	this->speed = speed;
}
double MyOneCar::getSpeed() const {
	return this->speed;
}
void MyOneCar::setPercent(double percent) {
	this->percent = percent;
}
double MyOneCar::getPercent() const {
	return this->percent;
}
void MyOneCar::setAll(int index, string path, double offset, double z, double speed, double percent) {
	this->index = index;
	this->path = path;
	this->offset = offset;
	this->z = z;
	this->speed = speed;
	this->percent = percent;
}