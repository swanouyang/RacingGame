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

ComCoordinate::ComCoordinate(double x, double y, double z, double w, double scale) {
	setAll(x, y, z, w, scale);
}

void ComCoordinate::operator = (const ComCoordinate& c) {
	setAll(c.x, c.y, c.z, c.w, c.scale);
}

void ComCoordinate::setX(double x) {
	this->x = x;
}
void ComCoordinate::setY(double y) {
	this->y = y;
}
void ComCoordinate::setZ(double z) {
	this->z = z;
}
void ComCoordinate::setW(double z) {
	this->w = z;
}
void ComCoordinate::setScale(double z) {
	this->scale = z;
}
double ComCoordinate::getX() {
	return this->x;
}
double ComCoordinate::getY() {
	return this->y;
}
double ComCoordinate::getZ() {
	return this->z;
}
double ComCoordinate::getW() {
	return this->w;
}
double ComCoordinate::getScale() {
	return this->scale;
}
void ComCoordinate::setAll(double x, double y, double z, double w, double scale) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	this->scale = scale;
}

//===================================================================
// ComPoint
//===================================================================
ComPoint::ComPoint() {
	initZero();
}

//ComPoint::ComPoint(ComCoordinate world) {
//	this->world = world;
//}
void ComPoint::initZero() {
	this->world.setAll(0.0, 0.0, 0.0);
	this->camera.setAll(0.0, 0.0, 0.0);
	this->screen.setAll(0.0, 0.0, 0.0);
}

void ComPoint::operator=(const ComPoint& p) {
	this->world = p.world;
	this->camera = p.camera;
	this->screen = p.screen;
}

void ComPoint::setWorld(ComCoordinate c) {
	this->world = c;
}
void ComPoint::setWorld(double x, double y, double z) {
	this->world.setAll(x, y, z);
}
void ComPoint::setCamera(ComCoordinate c) {
	this->camera = c;
}
void ComPoint::setCamera(double x, double y, double z) {
	this->camera.setAll(x, y, z);
}
void ComPoint::setScreen(ComCoordinate c) {
	this->screen = c;
}
void ComPoint::setScreen(double x, double y, double z) {
	this->screen.setAll(x, y, z);
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

ccColor4F ComDraw::changeColor(string colorString, double fog) {
	ccColor4F color = { INVALID_COORDINATE_VALUE };
	double tmpValue[3] = { 0.f };
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

void ComDraw::drawSolidRect(CCPoint origin, CCPoint destination, string color, double fog) {
	ccDrawSolidRect(origin, destination, changeColor(color, fog));
}
void ComDraw::drawSolidRect(double x, double y, double w, double h, string color, double fog) {
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
//int ComUtil::toInt(double a, double b) {
//	return (int)a;
//}

double ComUtil::minValue(double a, double b) {
	return (a > b) ? b : a;
}

double ComUtil::maxValue(double a, double b) {
	return (a > b) ? a : b;
}

double ComUtil::limit(double value, double min, double max) {
	return maxValue(min, minValue(value, max));
}

double ComUtil::interpolate(double a, double b, double percent) {
	return (a + (b - a) * percent);
}

int ComUtil::randomInt(double min, double max) {
	return round(interpolate(min, max, CCRANDOM_0_1()));
}

int ComUtil::randomChoice(vector<int> v) {
	return v.at(randomInt(0, v.size() - 1));
}

double ComUtil::randomChoice(vector<double> v) {
	return v.at(randomInt(0, v.size() - 1));
}

string ComUtil::randomChoice(vector<string> v) {
	return v.at(randomInt(0, v.size() - 1));
}

//LINK2019
//template <typename T>
//T ComUtil::randomChoice (vector<T> v) {
//	return v.at(randomInt(0, v.size() - 1));
//}

double ComUtil::percentRemaining(double n, double total) {
	return fmod(n, total) / total;//被坑，用整形变量取余方法(n%total)计算，导致上下坡路面抖动
}

double ComUtil::accelerate(double v, double accel, double dt) {
	return v + (accel * dt);
}

double ComUtil::easeIn(double a, double b, double percent) {
	return a + (b - a) * pow(percent, 2);
}

double ComUtil::easeOut(double a, double b, double percent) {
	return a + (b - a) * (1 - pow(1 - percent, 2));
}

double ComUtil::easeInOut(double a, double b, double percent) {
	return a + (b - a) * ((-cos(percent * M_PI) / 2) + 0.5);
}

double ComUtil::exponentialFog(double distance, double density) {
	return 1 / (pow(M_E, (distance * distance * density)));
}

double ComUtil::increase(double start, double increment, double max) {
	double result = start + increment;
	while (result >= max)
		result -= max;
	while (result < 0)
		result += max;
	return result;
}

double ComUtil::overlap(double x1, double w1, double x2, double w2, double percent) {
	double half = percent / 2;
	double min1 = x1 - (w1*half);
	double max1 = x1 + (w1*half);
	double min2 = x2 - (w2*half);
	double max2 = x2 + (w2*half);
	return !((max1 < min2) || (min1 > max2));
}

