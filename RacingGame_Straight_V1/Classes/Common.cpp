#include "Common.h"

//===================================================================
// ComCoordinate
//===================================================================

ComCoordinate::ComCoordinate() {
	this->x = ZERO_COORDINATE_VALUE;
	this->y = ZERO_COORDINATE_VALUE;
	this->z = ZERO_COORDINATE_VALUE;
	this->w = INVALID_COORDINATE_VALUE;
	this->scale = INVALID_COORDINATE_VALUE;
}

ComCoordinate::ComCoordinate(float x, float y, float z, float w, float scale) {
	setXYZ(x, y, z, w, scale);
}

void ComCoordinate::operator = (const ComCoordinate& c) {
	setXYZ(c.x, c.y, c.z, c.w, c.scale);
}

void ComCoordinate::setX(float x) {
	this->x = x;
}
void ComCoordinate::setY(float y) {
	this->y = y;
}
void ComCoordinate::setZ(float z) {
	this->z = z;
}
void ComCoordinate::setW(float z) {
	this->w = z;
}
void ComCoordinate::setScale(float z) {
	this->scale = z;
}
float ComCoordinate::getX() {
	return this->x;
}
float ComCoordinate::getY() {
	return this->y;
}
float ComCoordinate::getZ() {
	return this->z;
}
float ComCoordinate::getW() {
	return this->w;
}
float ComCoordinate::getScale() {
	return this->scale;
}
void ComCoordinate::setXYZ(float x, float y, float z, float w, float scale) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = z;
	this->scale = z;
}

//===================================================================
// ComPoint
//===================================================================
ComPoint::ComPoint() {
}

ComPoint::ComPoint(ComCoordinate world) {
	this->world = world;
}

void ComPoint::operator=(const ComPoint& p) {
	this->world = p.world;
	this->camera = p.camera;
	this->screen = p.screen;
}

void ComPoint::setWorld(ComCoordinate c) {
	this->world = c;
}
void ComPoint::setWorld(float x, float y, float z) {
	this->world.setXYZ(x, y, z);
}
void ComPoint::setCamera(ComCoordinate c) {
	this->camera = c;
}
void ComPoint::setCamera(float x, float y, float z) {
	this->camera.setXYZ(x, y, z);
}
void ComPoint::setScreen(ComCoordinate c) {
	this->screen = c;
}
void ComPoint::setScreen(float x, float y, float z) {
	this->screen.setXYZ(x, y, z);
}
ComCoordinate& ComPoint::getWorld() {
	return this->world;
}
ComCoordinate& ComPoint::getCamera() {
	return this->camera;
}
ComCoordinate& ComPoint::getScreen() {
	return this->screen;
}



//===================================================================
// ComColor
//===================================================================
ComColor::ComColor() {

}

ComColor::ComColor(string road, string grass, string rumble, string lane) {
	this->road = road;
	this->grass = grass;
	this->rumble = rumble;
	this->lane = lane;
}

void ComColor::operator = (const ComColor& color) {
	this->road = color.road;
	this->grass = color.grass;
	this->rumble = color.rumble;
	this->lane = color.lane;
}

void ComColor::setRoad(const string s) {
	this->road = s;
}
void ComColor::setGrass(const string s) {
	this->grass = s;
}
void ComColor::setRumble(const string s) {
	this->rumble = s;
}
void ComColor::setLane(const string s) {
	this->lane = s;
}
string& ComColor::getRoad() {
	return road;
}
string& ComColor::getGrass() {
	return grass;
}
string& ComColor::getRumble() {
	return rumble;
}
string& ComColor::getLane() {
	return lane;
}


//===================================================================
// ComDraw
//===================================================================
ComDraw* ComDraw::s_ComDraw = nullptr;

ComDraw* ComDraw::getInstance() {
	if (!s_ComDraw) {
		s_ComDraw = new ComDraw();
	}
	return s_ComDraw;
}
void ComDraw::destroy() {
	if (s_ComDraw) {
		delete s_ComDraw;
		s_ComDraw = nullptr;
	}
}

ccColor4F ComDraw::changeColor(string colorString, float fog) {
	ccColor4F color = { INVALID_COORDINATE_VALUE };
	float tmpValue[3] = { 0.f };
	if (!colorString.empty() && colorString.size() == COLOR_STRING_NUM) {
		for (int i = 0; i < 3; i++) {
			if (colorString.at(2 * i + 1) <= '9') {
				tmpValue[i] = (colorString.at(2 * i + 1) - '0') * 16;
			}
			else {
				tmpValue[i] = (colorString.at(2 * i + 1) - 'A' + 10) * 16;
			}
			if (colorString.at(2 * i + 2) <= '9') {
				tmpValue[i] += (colorString.at(2 * i + 2) - '0');
			}
			else {
				tmpValue[i] += (colorString.at(2 * i + 2) - 'A') + 10;
			}
		}
		color.r = tmpValue[0] / 255.f;
		color.g = tmpValue[1] / 255.f;
		color.b = tmpValue[2] / 255.f;
		color.a = fog;
	}
	return color;
}

void ComDraw::drawSolidRect(CCPoint origin, CCPoint destination, string color, float fog) {
	ccDrawSolidRect(origin, destination, changeColor(color, fog));
}
void ComDraw::drawSolidRect(float x, float y, float w, float h, string color, float fog) {
	CCPoint origin = ccp(x, SCREEN_HEIGHT - y);
	CCPoint destination = ccp(x + w, SCREEN_HEIGHT - (y + h));
	drawSolidRect(origin, destination, color, fog);
}

void ComDraw::drawSolidPoly(const CCPoint *poli, unsigned int numberOfPoints, string color) {
	ccDrawSolidPoly(poli, numberOfPoints, changeColor(color, 1.f));
}



//===================================================================
// ComUtil
//===================================================================
float ComUtil::minValue(float a, float b) {
	return (a > b) ? b : a;
}

float ComUtil::maxValue(float a, float b) {
	return (a > b) ? a : b;
}

float ComUtil::limit(float value, float min, float max) {
	return maxValue(min, minValue(value, max));
}

float ComUtil::interpolate(float a, float b, float percent) {
	return a + (b - a) * percent;
}

float ComUtil::randomInt(float min, float max) {
	return round(interpolate(min, max, CCRANDOM_0_1()));
}

int ComUtil::randomChoice(vector<int> v) {
	return v.at(randomInt(0, v.size() - 1));
}

float ComUtil::randomChoice(vector<float> v) {
	return v.at(randomInt(0, v.size() - 1));
}

float ComUtil::percentRemaining(int n, int total) {
	return ( n % total ) / total;
}

float ComUtil::accelerate(float v, float accel, float dt) {
	return v + (accel * dt);
}

float ComUtil::easeIn(float a, float b, float percent) {
	return a + (b - a) * pow(percent, 2);
}

float ComUtil::easeOut(float a, float b, float percent) {
	return a + (b - a) * (1 - pow(1 - percent, 2));
}

float ComUtil::easeInOut(float a, float b, float percent) {
	return a + (b - a) * ((-cos(percent * M_PI) / 2) + 0.5);
}

float ComUtil::exponentialFog(float distance, float density) {
	return 1 / (pow(M_E, (distance * distance * density)));
}

float ComUtil::increase(float start, float increment, float max) {
	float result = start + increment;
	while (result >= max)
		result -= max;
	while (result < 0)
		result += max;
	return result;
}

float ComUtil::overlap(float x1, float w1, float x2, float w2, float percent) {
	float half = (percent || 1) / 2;
	float min1 = x1 - (w1*half);
	float max1 = x1 + (w1*half);
	float min2 = x2 - (w2*half);
	float max2 = x2 + (w2*half);
	return !((max1 < min2) || (min1 > max2));
}

