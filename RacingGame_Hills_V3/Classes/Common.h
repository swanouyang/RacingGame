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

const double SCREEN_WIDTH = 1024.f;
const double SCREEN_HEIGHT = 768.f;

const double ZERO_COORDINATE_VALUE = 0.f;
const double INVALID_COORDINATE_VALUE = 0XFFFF;

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

const int ROAD_LENGTH_NONE = 0;
const int ROAD_LENGTH_SHORT = 25;
const int ROAD_LENGTH_MEDIUM = 50;
const int ROAD_LENGTH_LONG = 100;
const int ROAD_HILL_NONE = 0;
const int ROAD_HILL_LOW = 20;
const int ROAD_HILL_MEDIUM = 40;
const int ROAD_HILL_HIGH = 60;
const int ROAD_CURVE_NONE = 0;
const int ROAD_CURVE_EASY = 2;
const int ROAD_CURVE_MEDIUM = 4;
const int ROAD_CURVE_HARD = 6;
//===================================================================
// ComCoordinate
//===================================================================
class ComCoordinate {
private:
	double x;
	double y;
	double z;
	double w;
	double scale;
public:
	ComCoordinate();
	ComCoordinate(double x, double y, double z, 
		double w = INVALID_COORDINATE_VALUE, double scale = INVALID_COORDINATE_VALUE);

	void operator=(const ComCoordinate& c);

	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void setW(double w);
	void setScale(double scale);
	double getX();
	double getY();
	double getZ();
	double getW();
	double getScale();
	void setXYZ(double x, double y, double z, 
		double w = INVALID_COORDINATE_VALUE, double scale = INVALID_COORDINATE_VALUE);
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
	void setWorld(double x, double y, double z);
	void setCamera(ComCoordinate c);
	void setCamera(double x, double y, double z);
	void setScreen(ComCoordinate c);
	void setScreen(double x, double y, double z);
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

	ccColor4F changeColor(string colorString, double fog);
	void drawSolidRect(CCPoint origin, CCPoint destination, string color, double fog);
	void drawSolidRect(double x, double y, double w, double h, string color, double fog);
	void drawSolidPoly(const CCPoint *poli, unsigned int numberOfPoints, string color);
};



//===================================================================
// ComUtil
//===================================================================
class ComUtil {
public:
	//static int toInt(double a, double b);
	static double minValue(double a, double b);
	static double maxValue(double a, double b);
	static double limit(double value, double min, double max);
	static double interpolate(double a, double b, double percent);
	static double randomInt(double min, double max);
	static int randomChoice(vector<int> v);
	static double randomChoice(vector<double> v);
	static double percentRemaining(double n, double total);
	static double accelerate(double v, double accel, double dt);

	static double easeIn(double a, double b, double percent);
	static double easeOut(double a, double b, double percent);
	static double easeInOut(double a, double b, double percent);

	static double exponentialFog(double distance, double density);

	static double increase(double start, double increment, double max);
	static double overlap(double x1, double w1, double x2, double w2, double percent=1);
};





#endif