#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene() {
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

HelloWorld::~HelloWorld() {
	clean();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
	setTouchEnabled(true);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCRect rectSky = CCRect::CCRect(5, 495, 1280, 480);
	CCSprite* pSpriteSky = CCSprite::create("background.png", rectSky);
	pSpriteSky->setPosition(ccp(visibleSize.width / 2 + origin.x, 530));
	this->addChild(pSpriteSky, -1);

	CCRect rectHill = CCRect::CCRect(5, 5, 1280, 480);
	CCSprite* pSpriteHill = CCSprite::create("background.png", rectHill);
	pSpriteHill->setPosition(ccp(visibleSize.width / 2 + origin.x, 530));
	this->addChild(pSpriteHill, -1);

	CCRect rectTrees = CCRect::CCRect(5, 985, 1280, 480);
	CCSprite* pSpriteTrees = CCSprite::create("background.png", rectTrees);
	pSpriteTrees->setPosition(ccp(visibleSize.width / 2 + origin.x, 530));
	//pSpriteTrees->setScale(2);
	this->addChild(pSpriteTrees, -1);
    
	initVar();
	resetVar();
	//resetRoad();
	initDirectionRect();
	initSlider();

	//load all sprites' frame
	MySprites::getInstance()->init();

	//init my car's sprite
	pMyCarSprite = CCSprite::create();
	MySprites::getInstance()->setDisplayFrame(pMyCarSprite, g_SpriteMap.at("player_straight"));
	pMyCarSprite->setAnchorPoint(ccp(0.5, 0));
	pMyCarSprite->setPosition(ccp(width/2, 0));
	addChild(pMyCarSprite, 1);

	//init the sprite for four directions
	pDirection = CCSprite::create();
	MySprites::getInstance()->setDisplayFrame(pDirection, g_SpriteMap.at("directions"));
	pDirection->setAnchorPoint(ccp(0.5, 0.5));
	pDirection->setPosition(ccp(width-80, 80));
	pDirection->setScale(2);
	addChild(pDirection, 2);

	//call update for looping
	scheduleUpdate();

    return true;
}

void HelloWorld::registerWithTouchDispatcher() {
	//add touch pispatcher
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
}

void HelloWorld::initVar() {
	fps = 60;                      // how many 'update' frames per second
	step = 1 / fps;                   // how long is each frame (in seconds)
	width = 1024;                    // logical canvas width
	height = 768;                     // logical canvas height
	//segments = [];                      // array of road segments
	//stats = Game.stats('fps');       // mr.doobs FPS counter
	//canvas = Dom.get('canvas');       // our canvas...
	//ctx = canvas.getContext('2d'); // ...and its drawing context
	//background = null;                    // our background image (loaded below)
	//sprites = null;                    // our spritesheet (loaded below)
	centrifugal = 0.3f;                     // centrifugal force multiplier when going around curves
	//offRoadDecel = 0.99;                    // speed multiplier when off road (e.g. you lose 2% speed each update frame)
	skySpeed = 0.001f;                   // background sky layer scroll speed when going around curve (or up hill)
	hillSpeed = 0.002f;                   // background hill layer scroll speed when going around curve (or up hill)
	treeSpeed = 0.003f;                   // background tree layer scroll speed when going around curve (or up hill)
	skyOffset = 0;                       // current sky scroll offset
	hillOffset = 0;                       // current hill scroll offset
	treeOffset = 0;                       // current tree scroll offset

	resolution = 0;                    // scaling factor to provide resolution independence (computed)
	roadWidth = 2000;                    // actually half the roads width, easier math if the road spans from -roadWidth to +roadWidth
	segmentLength = 200;                     // length of a single segment
	rumbleLength = 3;                       // number of segments per red/white rumble strip
	trackLength = 0;                    // z length of entire track (computed)
	lanes = 3;                       // number of lanes
	fieldOfView = 100;                     // angle (degrees) for field of view
	cameraHeight = 1000;                    // z height of camera
	cameraDepth = 0;                    // z distance camera is from screen (computed)
	drawDistance = 200;                     // number of segments to draw
	playerX = 0;                       // player x offset from center of road (-1 to 1 to stay independent of roadWidth)
	playerZ = 0;                    // player relative z distance from camera (computed)
	fogDensity = 5;                       // exponential fog density
	position = 0;                       // current camera Z position (add playerZ to get player's absolute Z position)
	speed = 0;                       // current speed
	maxSpeed = segmentLength / step;      // top speed (ensure we can't move more than 1 segment in a single frame to make collision detection easier)
	accel = maxSpeed / 5;             // acceleration rate - tuned until it 'felt' right
	breaking = -maxSpeed;               // deceleration rate when braking
	decel = -maxSpeed / 5;             // 'natural' deceleration rate when neither accelerating, nor braking
	offRoadDecel = -maxSpeed / 2;             // off road deceleration is somewhere in between
	offRoadLimit = maxSpeed / 4;             // limit when off road deceleration no longer applies (e.g. you can always go at least this speed even when off road)
	//segmentsSize = 2000;

	keyLeft = false;
	keyRight = false;
	keyFaster = false;
	keySlower = false;

	pTmpPoint = new ComPoint();

	initColor();
}

void HelloWorld::initColor() {
	//init four kinds of color
	pLightColor = new ComColor(COLOR_LIGHT_ROAD, COLOR_LIGHT_GRASS, COLOR_LIGHT_RUMBLE, COLOR_LIGHT_LANE);
	pDarkColor = new ComColor(COLOR_DARK_ROAD, COLOR_DARK_GRASS, COLOR_DARK_RUMBLE, COLOR_DARK_LANE);
	pStartColor = new ComColor(COLOR_START, COLOR_START, COLOR_START, "");
	pFinishColor = new ComColor(COLOR_FINISH, COLOR_FINISH, COLOR_FINISH, "");
}

void HelloWorld::destroyColor() {
	if (pLightColor != nullptr) {
		delete pLightColor;
	}
	if (pDarkColor != nullptr) {
		delete pDarkColor;
	}
	if (pStartColor != nullptr) {
		delete pStartColor;
	}
	if (pFinishColor != nullptr) {
		delete pFinishColor;
	}
}

void HelloWorld::clean() {
	destroyColor();
	ComDraw::getInstance()->destroy();
	MySprites::getInstance()->destroy();
}

void HelloWorld::resetVar() {
	cameraDepth = 1 / tan((fieldOfView / 2) * M_PI / 180);
	playerZ = (cameraHeight * cameraDepth);
	resolution = height / 480;

	if ((segments.size() == 0) || (segmentLength) || (rumbleLength)) {
		resetRoad(); // only rebuild road when necessary
	}
}

void HelloWorld::addSegment(double curve) {
	//ComPoint* p = new ComPoint();
	Segment segment;
	double n = segments.size();

	segment.setIndex(n);
	segment.setCurve(curve);
	pTmpPoint->getWorld().setZ(n*segmentLength);
	segment.setPoint(*pTmpPoint, 0);
	pTmpPoint->getWorld().setZ((n + 1)*segmentLength);
	segment.setPoint(*pTmpPoint, 1);
	segment.setColor((int)floor(n / rumbleLength) % 2 ? *pDarkColor : *pLightColor);
	segments.push_back(segment);

	//delete p;
}

void HelloWorld::addRoad(double enter, double hold, double leave, double curve) {
	int n = 0;
	for (n = 0; n < enter; n++) {
		addSegment(ComUtil::easeIn(0, curve, (double)n / enter));
	}
	for (n = 0; n < hold; n++) {
		addSegment(curve);
	}
	for (n = 0; n < leave; n++) {
		addSegment(ComUtil::easeInOut(curve, 0, (double)n / leave));
	}
}

void HelloWorld::addStraight(double num) {
	addRoad(num, num, num, 0);
}

void HelloWorld::addCurve(double num, double curve) {
	addRoad(num, num, num, curve);
}

void HelloWorld::addSeqCurves() {
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, -ROAD_CURVE_EASY);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_CURVE_MEDIUM);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_CURVE_EASY);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, -ROAD_CURVE_EASY);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, -ROAD_CURVE_MEDIUM);
}

void HelloWorld::resetRoad() {
	segments.clear();
	
	addStraight(ROAD_LENGTH_SHORT / 4.f);
	addSeqCurves();
	addStraight(ROAD_LENGTH_LONG);
	addCurve(ROAD_LENGTH_MEDIUM, ROAD_CURVE_MEDIUM);
	addCurve(ROAD_LENGTH_LONG, ROAD_CURVE_MEDIUM);
	addStraight();
	addSeqCurves();
	addCurve(ROAD_LENGTH_LONG, -ROAD_CURVE_MEDIUM);
	addCurve(ROAD_LENGTH_LONG, ROAD_CURVE_MEDIUM);
	addStraight();
	addSeqCurves();
	addCurve(ROAD_LENGTH_LONG, -ROAD_CURVE_EASY);

	segments[findSegment(playerZ).getIndex() + 2].setColor(*pStartColor);
	segments[findSegment(playerZ).getIndex() + 3].setColor(*pStartColor);
	for (int n = 0; n < rumbleLength; n++) {
		segments[segments.size() - 1 - n].setColor(*pFinishColor);
	}
	trackLength = segments.size() * segmentLength;

	delete pTmpPoint;
}

Segment HelloWorld::findSegment(int z) {
	int index = (int)floor(z / segmentLength) % segments.size();
	return segments[index];
}

void HelloWorld::update(float dt) {
	Segment playerSegment = findSegment(position + playerZ);
	double speedPercent = speed / maxSpeed;
	double dx = dt * 2 * speedPercent;
	position = ComUtil::increase(position, dt * speed, trackLength);

	skyOffset = ComUtil::increase(skyOffset, skySpeed*playerSegment.getCurve()*speedPercent, 1);
	hillOffset = ComUtil::increase(hillOffset, hillSpeed*playerSegment.getCurve()*speedPercent, 1);
	treeOffset = ComUtil::increase(treeOffset, treeSpeed*playerSegment.getCurve()*speedPercent, 1);

	if (keyLeft) {
		playerX = playerX - dx;
	}
	else if (keyRight) {
		playerX = playerX + dx;
	}

	playerX = playerX - (dx * speedPercent * playerSegment.getCurve() * centrifugal);

	if (keyFaster) {
		speed = ComUtil::accelerate(speed, accel, dt);
	}
	else if (keySlower) {
		speed = ComUtil::accelerate(speed, breaking, dt);
	}
	else {
		speed = ComUtil::accelerate(speed, decel, dt);
	}

	if (((playerX < -1) || (playerX > 1)) && (speed > offRoadLimit)) {
		speed = ComUtil::accelerate(speed, offRoadDecel, dt);
	}

	//dont ever let player go too far out of bounds
	playerX = ComUtil::limit(playerX, -2, 2);
	// or exceed maxSpeed
	speed = ComUtil::limit(speed, 0, maxSpeed);
}

void HelloWorld::draw() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //cocos2d默认混合模式，如果没有设置blend混合模式，默认没有透明度
	render();
	player();
}

void HelloWorld::render() {
	if (pRoadWidthSlider) {
		roadWidth = pRoadWidthSlider->getSlider()->getValue();
	}
	if (pCameraHeightSlider) {
		cameraHeight = pCameraHeightSlider->getSlider()->getValue();
		playerZ = (cameraHeight * cameraDepth);
	}
	if (pDrawDistanceSlider) {
		drawDistance = pDrawDistanceSlider->getSlider()->getValue();
	}
	if (pFieldOfViewSlider) {
		fieldOfView = pFieldOfViewSlider->getSlider()->getValue();
		cameraDepth = 1 / tan((fieldOfView / 2) * M_PI / 180);
		playerZ = (cameraHeight * cameraDepth);
	}
	if (pFogDensitySlider) {
		fogDensity = pFogDensitySlider->getSlider()->getValue();
	}
	
	Segment baseSegment = findSegment(position);
	double basePercent = ComUtil::percentRemaining(position, segmentLength);
	double maxy = height;
	Segment segment;
	int n;
	double x = 0;
	double dx = -(baseSegment.getCurve()*basePercent);

	for (n = 0; n < drawDistance; n++) {
		segment = segments[(baseSegment.getIndex() + n) % segments.size()];
		segment.setLooped(segment.getIndex() < baseSegment.getIndex());
		segment.setFog(ComUtil::exponentialFog(n / drawDistance, fogDensity));

		segment.project((playerX * roadWidth), x, dx, cameraHeight, 
			position - (segment.isLooped() ? trackLength : 0), cameraDepth, width, height, roadWidth);
		x = x + dx;
		dx = dx + segment.getCurve();

		if ((segment.getPoint(0).getCamera().getZ() <= cameraDepth) || // behind us
			//(segment.getPoint(1).getScreen().getY() >= segment.getPoint(0).getScreen().getY()) ||//back face cull
			(segment.getPoint(1).getScreen().getY() >= maxy)) {        // clip by (already rendered) segment
			continue;
		}

		segment.show(width, lanes);
		maxy = segment.getPoint(1).getScreen().getY();
	}
}

void HelloWorld::player() {
	double speedPercent = speed / maxSpeed;
	vector<int> v;
	v.push_back(-1);
	v.push_back(1);
	double bounce = (1.5 * CCRANDOM_0_1() * speedPercent * resolution) * ComUtil::randomChoice(v);
	
	double steer = speed * (keyLeft ? -1 : keyRight ? 1 : 0);
	string carSpriteFrameName;
	double updown = 0;//is climbing hill
	if (steer < 0) {
		carSpriteFrameName = (updown > 0) ? "player_uphill_left" :"player_left";
	}
	else if (steer > 0) {
		carSpriteFrameName = (updown > 0) ? "player_uphill_right" : "player_right";
	}
	else {
		carSpriteFrameName = (updown > 0) ? "player_uphill_straight" : "player_straight";
	}
	MySprites::getInstance()->setDisplayFrame(pMyCarSprite, g_SpriteMap.at(carSpriteFrameName));
	pMyCarSprite->setPosition(ccp(width / 2, 0 + bounce));

	double carWidth = pMyCarSprite->getTextureRect().size.width;
	double carHeight = pMyCarSprite->getTextureRect().size.height;

	double scaleProject = cameraDepth / playerZ * width / 2;	//scale for projection
	double scaleSprite = 0.3 * (1 / carWidth) * roadWidth;	//scale for relative to roadWidth(for tweakUI)
	double scale = scaleProject * scaleSprite;
	pMyCarSprite->setScale(scale);

	double destW = pMyCarSprite->getTexture()->getContentSize().width * scale;
	double destH = pMyCarSprite->getTexture()->getContentSize().height * scale;

	//Render.sprite(ctx, width, height, resolution, roadWidth, sprites, sprite, scale, destX, destY + bounce, -0.5, -1);

	//var destW = (sprite.w * scale * width / 2) * (SPRITES.SCALE * roadWidth);
	//var destH = (sprite.h * scale * width / 2) * (SPRITES.SCALE * roadWidth);

	//destX = destX + (destW * (-0.5 || 0));
	//destY = destY + (destH * (-1 || 0));

	//var clipH = clipY ? Math.max(0, destY + destH - clipY) : 0;
	//if (clipH < destH)
	//	ctx.drawImage(sprites, sprite.x, sprite.y, sprite.w, sprite.h - (sprite.h*clipH / destH), destX, destY, destW, destH - clipH);

}

void HelloWorld::initDirectionRect() {
	rectAW = CCRect(width - 80 - 54, 80 + 18, 18 * 2, 18 * 2);
	rectDW = CCRect(width - 80 + 18, 80 + 18, 18 * 2, 18 * 2);
	rectAS = CCRect(width - 80 - 54, 80 - 54, 18 * 2, 18 * 2);
	rectDS = CCRect(width - 80 + 18, 80 - 54, 18 * 2, 18 * 2);
	rectA = CCRect(width - 80 - 54, 80 - 18 , 18 * 2, 18 * 2);
	rectS = CCRect(width - 80 - 18, 80 - 54, 18 * 2, 18 * 2);
	rectW = CCRect(width - 80 - 18, 80 + 18, 18 * 2, 18 * 2);
	rectD = CCRect(width - 80 + 18, 80 - 18, 18 * 2, 18 * 2);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
	CCPoint location = touch->getLocation();

	ccTouchesEnded(pTouches, pEvent);
	if (rectAW.containsPoint(location)) {
		keyLeft = true;
		keyFaster = true;
	}
	else if (rectDW.containsPoint(location)) {
		keyFaster = true;
		keyRight = true;
	}
	else if (rectAS.containsPoint(location)) {
		keyLeft = true;
		keySlower = true;
	}
	else if (rectDS.containsPoint(location)) {
		keyRight = true;
		keySlower = true;
	}
	else if (rectA.containsPoint(location)) {
		keyLeft = true;
	}
	else if (rectD.containsPoint(location)) {
		keyRight = true;
	}
	else if (rectS.containsPoint(location)) {
		keySlower = true;
	}
	else if (rectW.containsPoint(location)) {
		keyFaster = true;
	}
}
void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {
	ccTouchesBegan(pTouches, pEvent);
}
void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {
	keyFaster = false;
	keySlower = false;
	keyRight = false;
	keyLeft = false;
}



void HelloWorld::initSlider() {
	initOneSlider(&pRoadWidthSlider, "Road Width:", 740, 500, 3000, roadWidth);
	initOneSlider(&pCameraHeightSlider, "Camera Height:", 720, 500, 5000, cameraHeight);
	initOneSlider(&pDrawDistanceSlider, "Draw Distance:", 700, 100, 500, drawDistance);
	initOneSlider(&pFieldOfViewSlider, "Field Of View:", 680, 80, 140, fieldOfView);
	initOneSlider(&pFogDensitySlider, "Fog Density:", 660, 5, 50, fogDensity);
}

void HelloWorld::initOneSlider(SliderCtrl** slider, const char* string, double height, double min, double max, double cur) {
	CCLabelTTF * ttfName = CCLabelTTF::create(string, "Arial-BoldMT", 15);
	ttfName->setPosition(ccp(830, height));
	ttfName->setColor({ 50, 50, 50 });
	addChild(ttfName, 3);

	(*slider) = new SliderCtrl();
	(*slider)->create(this, min, max, cur, 940, height, 0.75f);
	addChild((*slider)->getSlider(), 3);
}