#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Common.h"
#include "Segment.h"
#include "MySprites.h"
#include "SliderCtrl.h"
#include <cmath>

//#define __ENABLE_DIRECTION__
const int MAX_CAR_NUM = 40;
const int MAX_SPRITE_NUM = 400;

class HelloWorld : public CCLayer 
{
private:
	double fps;								// how many 'update' frames per second
	double step;							// how long is each frame (in seconds)
	double width = 1024;					// logical canvas width
	double height = 768;					// logical canvas height

	double centrifugal;                     // centrifugal force multiplier when going around curves//离心力
	//float offRoadDecel;                    // speed multiplier when off road (e.g. you lose 2% speed each update frame)
	double skySpeed;                   // background sky layer scroll speed when going around curve (or up hill)
	double hillSpeed;                   // background hill layer scroll speed when going around curve (or up hill)
	double treeSpeed;                   // background tree layer scroll speed when going around curve (or up hill)
	double skyOffset;                       // current sky scroll offset
	double hillOffset;                       // current hill scroll offset
	double treeOffset;                       // current tree scroll offset

	vector<Segment> segments;               // array of road segments
	vector<MyOneCar> cars;
	double resolution;						// scaling factor to provide resolution independence (computed)
	double roadWidth;						// actually half the roads width, easier math if the road spans from -roadWidth to +roadWidth
	double segmentLength;					// length of a single segment
	double rumbleLength;                    // number of segments per red/white rumble strip
	double trackLength;						// z length of entire track (computed)
	double lanes = 3;							// number of lanes
	double fieldOfView;						// angle (degrees) for field of view
	double cameraHeight;					// z height of camera
	double cameraDepth;						// z distance camera is from screen (computed)
	double drawDistance;					// number of segments to draw
	double playerX;							// player x offset from center of road (-1 to 1 to stay independent of roadWidth)
	double playerZ;							// player relative z distance from camera (computed)
	double fogDensity;                      // exponential fog density
	double position;						// current camera Z position (add playerZ to get player's absolute Z position)
	double speed;							// current speed
	double maxSpeed;						// top speed (ensure we can't move more than 1 segment in a single frame to make collision detection easier)
	double accel;							// acceleration rate - tuned until it 'felt' right
	double breaking;						// deceleration rate when braking
	double decel;							// 'natural' deceleration rate when neither accelerating, nor braking
	double offRoadDecel;					// off road deceleration is somewhere in between
	double offRoadLimit;					// limit when off road deceleration no longer applies (e.g. you can always go at least this speed even when off road)
	int    segmentsSize;					// the size of all segments in a round

	int	totalCars;		                    // total number of cars on the road

	double playerSpriteScale;				//scale for relative to roadWidth(for tweakUI)
	double playerWidth;						//the width of the player's car
	double playerHeight;					//the height of the player's car




	static bool keyLeft;
	static bool keyRight;
	static bool keyFaster;
	static bool keySlower;

	Segment* pTmpSegment;
	vector<Segment>::iterator iterTmpSegment;
	vector<Segment>::iterator iterBaseSegment;
	vector<Segment>::iterator iterOldCarSegment;
	vector<Segment>::iterator iterNewCarSegment;
	vector<Segment>::iterator iterPlayerSegment;

	ComColor* pLightColor;
	ComColor* pDarkColor;
	ComColor* pStartColor;
	ComColor* pFinishColor;

	ComPoint* pTmpPoint;

	MyOneSprite* pTmpSprite;
	MyOneCar* pTmpCar;
	vector<MyOneSprite>::iterator iterTmpSprite;
	vector<MyOneCar>::iterator iterTmpCar;
	vector<MyOneCar>::iterator iterOtherCar;

	CCSprite *pMyCarSprite;

	CCSprite *pOtherCarSprite[MAX_CAR_NUM];//先结合是本质
	CCSprite *pOtherTreeSprite[MAX_SPRITE_NUM];//先结合是本质

#ifdef __ENABLE_DIRECTION__
	CCSprite *pDirection;
	CCRect rectAW;
	CCRect rectDW;
	CCRect rectAS;
	CCRect rectDS;
	CCRect rectA;
	CCRect rectS;
	CCRect rectW;
	CCRect rectD;
#endif

	SliderCtrl* pRoadWidthSlider;
	SliderCtrl* pCameraHeightSlider;
	SliderCtrl* pDrawDistanceSlider;
	SliderCtrl* pFieldOfViewSlider;
	SliderCtrl* pFogDensitySlider;

	vector<string> vBillBoards;
	vector<string> vPlants;
	vector<string> vCars;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	~HelloWorld();
    virtual bool init();
	virtual void update(float dt);
	void clean();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	void resetVar();
	double getLastY() const;
	void addSegment(double curve, double y);
	void addRoad(double enter, double hold, double leave, double curve, double y);
	void addStraight(double num = ROAD_LENGTH_MEDIUM);
	void addCurve(double num = ROAD_LENGTH_MEDIUM, double curve = ROAD_CURVE_MEDIUM, double height = ROAD_HILL_NONE);
	void addSeqCurves();
	void addHill(double num = ROAD_LENGTH_MEDIUM, double height = ROAD_HILL_MEDIUM);
	void addLowRowingHills(double num = ROAD_LENGTH_SHORT, double height = ROAD_HILL_LOW);
	void addDownHillToEnd(double num = 200.0);
	void addBumps();
	void addSprite(int n, string path, double offset);
	void resetRoad();
	void resetSprites();
	void resetCars();
	void initVar();
	void initColor();

	void initSlider();
	void initOneSlider(SliderCtrl** slider, const char* string, double height, double min, double max, double cur);
	void destroyColor();
	//double getSpriteWidth(CCSprite* sprite) const;
	//Segment findSegment(int z);
	vector<Segment>::iterator findSegment(int z);

	void draw();
	void render();
	void showPlayer();
	void showCarsAndSprite();

	void updateCars(float dt, double playerW);
	double updateCarOffset(double playerW);
	
#ifdef __ENABLE_DIRECTION__
	void initDirectionRect();
	virtual void registerWithTouchDispatcher();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
#endif
	static void keyboardHook(UINT message, WPARAM wParam, LPARAM lParam);

	void calcSlider();
};

#endif // __HELLOWORLD_SCENE_H__
