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
	CCSpriteFrame *frame = frameCache->spriteFrameByName(name.c_str());
	sprite->setDisplayFrame(frame);
}
