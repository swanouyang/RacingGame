#ifndef __MY_COMMON_H__
#define __MY_COMMON_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>
#include <cmath>
#include "stdlib.h"
using namespace std;
USING_NS_CC;		//using namespace cocos2d;
USING_NS_CC_EXT;	//using namespace cocos2d::extension;

#define DRAW_CHANGE_COLOR ComDraw::getInstance()->changeColor
#define DRAW_SOLID_RECT ComDraw::getInstance()->drawSolidRect
#define DRAW_SOLID_POLY ComDraw::getInstance()->drawSolidPoly

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const float ZERO_COORDINATE_VALUE = 0.f;
const float INVALID_COORDINATE_VALUE = 0XFFFF;

const int COLOR_STRING_NUM = 7;
const string COLOR_WHITE = "#FFFFFF";
const string COLOR_START = "#FFFFFF";
const string COLOR_BLACK = "#000000";
const string COLOR_FINISH = "#000000";

const string COLOR_LIGHT_ROAD = "#6B6B6B";
const string COLOR_LIGHT_GRASS = "#10AA10";
const string COLOR_LIGHT_RUMBLE = "#555555";
const string COLOR_LIGHT_LANE = "#CCCCCC";

const string COLOR_DARK_ROAD = "#696969";
const string COLOR_DARK_GRASS = "#009A00";
const string COLOR_DARK_RUMBLE = "#BBBBBB";
const string COLOR_DARK_LANE = "";

const string COLOR_SKY = "#72D7EE";
const string COLOR_TREE = "#005108";
const string COLOR_FOG = "#005108";
//const string COLOR_FOG = "#6FFF7D";

//===================================================================
// ComCoordinate
//===================================================================
class ComCoordinate {
private:
	float x;
	float y;
	float z;
	float w;
	float scale;
public:
	ComCoordinate();
	ComCoordinate(float x, float y, float z, 
		float w = INVALID_COORDINATE_VALUE, float scale = INVALID_COORDINATE_VALUE);

	void operator=(const ComCoordinate& c);

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);
	void setScale(float scale);
	float getX();
	float getY();
	float getZ();
	float getW();
	float getScale();
	void setXYZ(float x, float y, float z, 
		float w = INVALID_COORDINATE_VALUE, float scale = INVALID_COORDINATE_VALUE);
};


//===================================================================
// ComPoint
//===================================================================
class ComPoint {
private:
	ComCoordinate world;
	ComCoordinate camera;
	ComCoordinate screen;
public:
	ComPoint();
	ComPoint(ComCoordinate world);

	void operator=(const ComPoint& p);

	void setWorld(ComCoordinate c);
	void setWorld(float x, float y, float z);
	void setCamera(ComCoordinate c);
	void setCamera(float x, float y, float z);
	void setScreen(ComCoordinate c);
	void setScreen(float x, float y, float z);
	ComCoordinate& getWorld();
	ComCoordinate& getCamera();
	ComCoordinate& getScreen();
};


//===================================================================
// ComColor
//===================================================================
class ComColor {
	string road;
	string grass;
	string rumble;
	string lane;
public:
	ComColor();
	ComColor(string road, string grass, string rumble, string lane);

	void operator=(const ComColor& color);

	void setRoad(const string s);
	void setGrass(const string s);
	void setRumble(const string s);
	void setLane(const string s);
	string& getRoad();
	string& getGrass();
	string& getRumble();
	string& getLane();
};


//===================================================================
// ComDraw
//===================================================================
class ComDraw {
private:
	ComDraw() {}
	static ComDraw* s_ComDraw;

public:
	static ComDraw* getInstance();
	static void destroy();

	ccColor4F changeColor(string colorString, float fog);
	void drawSolidRect(CCPoint origin, CCPoint destination, string color, float fog);
	void drawSolidRect(float x, float y, float w, float h, string color, float fog);
	void drawSolidPoly(const CCPoint *poli, unsigned int numberOfPoints, string color);
};



//===================================================================
// ComUtil
//===================================================================
class ComUtil {
public:
	static float minValue(float a, float b);
	static float maxValue(float a, float b);
	static float limit(float value, float min, float max);
	static float interpolate(float a, float b, float percent);
	static float randomInt(float min, float max);
	static int randomChoice(vector<int> v);
	static float randomChoice(vector<float> v);
	static float percentRemaining(int n, int total);
	static float accelerate(float v, float accel, float dt);

	static float easeIn(float a, float b, float percent);
	static float easeOut(float a, float b, float percent);
	static float easeInOut(float a, float b, float percent);

	static float exponentialFog(float distance, float density);

	static float increase(float start, float increment, float max);
	static float overlap(float x1, float w1, float x2, float w2, float percent);
};





#endif