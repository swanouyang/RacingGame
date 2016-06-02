#include "HelloWorldScene.h"
//#include "Common.cpp"

USING_NS_CC;

bool HelloWorld::keyLeft = false;
bool HelloWorld::keyRight = false;
bool HelloWorld::keyFaster = false;
bool HelloWorld::keySlower = false;


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
	//CCDirector::sharedDirector()->getOpenGLView()->setAccelerometerKeyHook(HelloWorld::keyboardHook);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCRect rectSky = CCRect::CCRect(5, 495, 1280, 480);
	CCSprite* pSpriteSky = CCSprite::create("background.png", rectSky);
	pSpriteSky->setPosition(ccp(visibleSize.width / 2 + origin.x, 530));
	this->addChild(pSpriteSky, -1000);

	CCRect rectHill = CCRect::CCRect(5, 5, 1280, 480);
	CCSprite* pSpriteHill = CCSprite::create("background.png", rectHill);
	pSpriteHill->setPosition(ccp(visibleSize.width / 2 + origin.x, 530));
	this->addChild(pSpriteHill, -1000);

	CCRect rectTrees = CCRect::CCRect(5, 985, 1280, 480);
	CCSprite* pSpriteTrees = CCSprite::create("background.png", rectTrees);
	pSpriteTrees->setPosition(ccp(visibleSize.width / 2 + origin.x, 530));
	//pSpriteTrees->setScale(2);
	this->addChild(pSpriteTrees, -1000);
    
	initVar();
	resetVar();
	//resetRoad();
#ifdef __ENABLE_DIRECTION__
	initDirectionRect();
	//init the sprite for four directions
	pDirection = CCSprite::create();
	MySprites::getInstance()->setDisplayFrame(pDirection, "directions");
	pDirection->setAnchorPoint(ccp(0.5, 0.5));
	pDirection->setPosition(ccp(width - 80, 80));
	pDirection->setScale(2);
	addChild(pDirection, 1000);
#endif
	initSlider();

	//load all sprites' frame
	MySprites::getInstance()->init();

	//init my car's sprite
	pMyCarSprite = CCSprite::create();
	MySprites::getInstance()->setDisplayFrame(pMyCarSprite, "player_straight");
	pMyCarSprite->setAnchorPoint(ccp(0.5, 0));
	pMyCarSprite->setPosition(ccp(width/2, 0));
	addChild(pMyCarSprite, 300);

	MySprites::getWidthAndHeight(pMyCarSprite, playerWidth, playerHeight);
	playerSpriteScale = 0.3 * (1 / playerWidth);

	for (int i = 0; i < MAX_CAR_NUM; i++) {
		pOtherCarSprite[i] = CCSprite::create();
		MySprites::getInstance()->setDisplayFrame(pOtherCarSprite[i], "truck");
		pOtherCarSprite[i]->setAnchorPoint(ccp(0.5, 0));
		pOtherCarSprite[i]->setPosition(ccp(0, 0));
		addChild(pOtherCarSprite[i]);
		pOtherCarSprite[i]->setVisible(false);
	}

	for (int i = 0; i < MAX_SPRITE_NUM; i++) {
		pOtherTreeSprite[i] = CCSprite::create();
		MySprites::getInstance()->setDisplayFrame(pOtherTreeSprite[i], "truck");
		pOtherTreeSprite[i]->setAnchorPoint(ccp(0.5, 0));
		pOtherTreeSprite[i]->setPosition(ccp(0, 0));
		addChild(pOtherTreeSprite[i]);
		pOtherTreeSprite[i]->setVisible(false);
	}

	//unsigned long nSize = 0;
	//unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData("sliderThumb.png", "rb", &nSize);
	//CCImage image;
	//image.initWithImageFile("sliderThumb.png");
	//FILE* imageFile = 
	//CCTexture2D *texture = 

	//call update for looping
	scheduleUpdate();

    return true;
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
	drawDistance = 300;                     // number of segments to draw
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
	totalCars = 200;

	keyLeft = false;
	keyRight = false;
	keyFaster = false;
	keySlower = false;

	pTmpSegment = new Segment();
	pTmpSprite = new MyOneSprite();
	pTmpCar = new MyOneCar();
	pTmpPoint = new ComPoint();

	for (int i = 0; i < g_BillBoardsNum; i++) {
		vBillBoards.push_back(g_BillBoardsName[i]);
	}
	for (int i = 0; i < g_PlantsNum; i++) {
		vPlants.push_back(g_PlantsName[i]);
	}
	for (int i = 0; i < g_CarsNum; i++) {
		vCars.push_back(g_CarsName[i]);
	}

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
	if (pTmpSegment != nullptr) {
		delete pTmpSegment;
		pTmpSegment = nullptr;
	}
	if (pTmpPoint != nullptr) {
		delete pTmpPoint;
		pTmpPoint = nullptr;
	}
	if (pTmpSprite != nullptr) {
		delete pTmpSprite;
		pTmpSprite = nullptr;
	}
	if (pTmpCar != nullptr) {
		delete pTmpCar;
		pTmpCar = nullptr;
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

//double HelloWorld::getSpriteWidth(CCSprite* sprite) const {
//	return sprite->getTextureRect().size.width;
//}

double HelloWorld::getLastY() const {
	return (segments.size() == 0) ? 0 : segments.back().getPoint(1).getWorld().getY();
}

void HelloWorld::addSegment(double curve, double y) {
	double n = segments.size();

	pTmpSegment->setIndex(n);
	pTmpSegment->setCurve(curve);
	pTmpPoint->initZero();
	pTmpPoint->getWorld().setY(getLastY());
	pTmpPoint->getWorld().setZ(n*segmentLength);
	pTmpSegment->setPoint(*pTmpPoint, 0);
	pTmpPoint->getWorld().setY(y);
	pTmpPoint->getWorld().setZ((n + 1)*segmentLength);
	pTmpSegment->setPoint(*pTmpPoint, 1);
	pTmpSegment->setColor((int)floor(n / rumbleLength) % 2 ? *pDarkColor : *pLightColor);
	segments.push_back(*pTmpSegment);

	//delete p;
}

void HelloWorld::addRoad(double enter, double hold, double leave, double curve, double y) {
	double startY = getLastY();
	double endY = startY + (double)(int)y * segmentLength;
	double total = enter + hold + leave;
	int n = 0;
	for (n = 0; n < enter; n++) {
		addSegment(ComUtil::easeIn(0, curve, (double)n / enter),
			ComUtil::easeInOut(startY, endY, (double)n / total));
	}
	for (n = 0; n < hold; n++) {
		addSegment(curve, ComUtil::easeInOut(startY, endY, (double)(enter + n) / total));
	}
	for (n = 0; n < leave; n++) {
		addSegment(ComUtil::easeInOut(curve, 0, (double)n / leave), 
			ComUtil::easeInOut(startY, endY, (double)(enter + leave + n) / total));
	}
}

void HelloWorld::addStraight(double num) {
	addRoad(num, num, num, 0, 0);
}

void HelloWorld::addCurve(double num, double curve, double height) {
	addRoad(num, num, num, curve, height);
}

void HelloWorld::addSeqCurves() {
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, -ROAD_CURVE_EASY, ROAD_HILL_NONE);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_CURVE_MEDIUM, ROAD_HILL_MEDIUM);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_CURVE_EASY, -ROAD_HILL_LOW);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, -ROAD_CURVE_EASY, ROAD_HILL_MEDIUM);
	addRoad(ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, ROAD_LENGTH_MEDIUM, -ROAD_CURVE_MEDIUM, -ROAD_HILL_MEDIUM);
}

void HelloWorld::addHill(double num, double height) {
	addRoad(num, num, num, 0, height);
}

void HelloWorld::addLowRowingHills(double num, double height) {
	addRoad(num, num, num, 0, height / 2.0);
	addRoad(num, num, num, 0, -height);
	addRoad(num, num, num, ROAD_CURVE_EASY, height);
	addRoad(num, num, num, 0, 0);
	addRoad(num, num, num, -ROAD_CURVE_EASY, height / 2.0);
	addRoad(num, num, num, 0, 0);
}

void HelloWorld::addDownHillToEnd(double num) {
	addRoad(num, num, num, -ROAD_CURVE_EASY, -getLastY() / segmentLength);
}

void HelloWorld::addBumps() {
	addRoad(10, 10, 10, 0, 5);
	addRoad(10, 10, 10, 0, -2);
	addRoad(10, 10, 10, 0, -5);
	addRoad(10, 10, 10, 0, 8);
	addRoad(10, 10, 10, 0, 5);
	addRoad(10, 10, 10, 0, -7);
	addRoad(10, 10, 10, 0, 5);
	addRoad(10, 10, 10, 0, -2);

}
void HelloWorld::addSprite(int n, string path, double offset) {
	pTmpSprite->setPath(path);
	pTmpSprite->setOffset(offset);
	segments[n].sprites.push_back(*pTmpSprite);
}

void HelloWorld::resetRoad() {
	segments.clear();
	
	addStraight(ROAD_LENGTH_SHORT);
	addLowRowingHills();
	addSeqCurves();
	addCurve(ROAD_LENGTH_MEDIUM, ROAD_CURVE_MEDIUM, ROAD_HILL_LOW);
	addBumps();
	addLowRowingHills();
	addCurve(ROAD_LENGTH_LONG*2, ROAD_CURVE_MEDIUM, ROAD_HILL_MEDIUM);
	addStraight();
	addHill(ROAD_LENGTH_MEDIUM, ROAD_HILL_HIGH);
	addSeqCurves();
	addCurve(ROAD_LENGTH_LONG, -ROAD_CURVE_MEDIUM, ROAD_HILL_NONE);
	addHill(ROAD_LENGTH_LONG, ROAD_HILL_HIGH);
	addCurve(ROAD_LENGTH_LONG, ROAD_CURVE_MEDIUM, -ROAD_HILL_LOW);
	addBumps();
	addHill(ROAD_LENGTH_LONG, -ROAD_HILL_MEDIUM);
	addStraight();
	addSeqCurves();
	addDownHillToEnd();

	resetSprites();
	resetCars();

	segments[findSegment(playerZ)->getIndex() + 2].setColor(*pStartColor);
	segments[findSegment(playerZ)->getIndex() + 3].setColor(*pStartColor);
	for (int n = 0; n < rumbleLength; n++) {
		segments[segments.size() - 1 - n].setColor(*pFinishColor);
	}
	trackLength = segments.size() * segmentLength;
}

void HelloWorld::resetSprites() {
	size_t n, i;
	vector<double> v;
	v.push_back(-1.0);
	v.push_back(1.0);

	addSprite(20, "billboard07", -1);
	addSprite(40, "billboard06", -1);
	addSprite(60, "billboard08", -1);
	addSprite(80, "billboard09", -1);
	addSprite(100, "billboard01", -1);
	addSprite(120, "billboard02", -1);
	addSprite(140, "billboard03", -1);
	addSprite(160, "billboard04", -1);
	addSprite(180, "billboard05", -1);

	addSprite(240, "billboard07", -1.2);
	addSprite(240, "billboard06", 1.2);
	addSprite(segments.size() - 25, "billboard07", -1.2);
	addSprite(segments.size() - 25, "billboard06", 1.2);

	for (n = 10; n < 200; n += 4 + floor(n / 100)) {
		addSprite(n, "palm_tree", 0.5 + CCRANDOM_0_1()*0.5);
		addSprite(n, "palm_tree", 1 + CCRANDOM_0_1() * 2);
	}

	for (n = 250; n < 1000; n += 5) {
		addSprite(n, "column", 1.1);
		addSprite(n + ComUtil::randomInt(0, 5), "tree1", -1 - (CCRANDOM_0_1() * 2));
		addSprite(n + ComUtil::randomInt(0, 5), "tree2", -1 - (CCRANDOM_0_1() * 2));
	}

	for (n = 200; n < segments.size(); n += 3) {
		addSprite(n, ComUtil::randomChoice(vPlants), 
			ComUtil::randomChoice(v) * (2 + CCRANDOM_0_1() * 5));
	}

	double side, offset;
	string path;
	for (n = 1000; n < (segments.size() - 50); n += 100) {
		side = ComUtil::randomChoice(v);
		addSprite(n + ComUtil::randomInt(0, 50), ComUtil::randomChoice(vBillBoards), -side);
		for (i = 0; i < 20; i++) {
			path = ComUtil::randomChoice(vPlants);
			offset = side * (1.5 + CCRANDOM_0_1());
			addSprite(n + ComUtil::randomInt(0, 50), path, offset);
		}
	}
}

void HelloWorld::resetCars() {
	int n;
	double offset, z, speed, percent;
	string path;
	vector<double> v;
	v.push_back(-0.8);
	v.push_back(0.8);

	//var n, car, segment, offset, z, sprite, speed;
	for (n = 0; n < totalCars; n++) {
		offset = CCRANDOM_0_1() * ComUtil::randomChoice(v);
		z = floor(CCRANDOM_0_1() * segments.size()) * segmentLength;
		path = ComUtil::randomChoice(vCars);
		speed = maxSpeed / 4 + CCRANDOM_0_1() * maxSpeed / (path == "semi" ? 4 : 2);
		percent = ComUtil::percentRemaining(z, segmentLength);
		pTmpCar->setAll(n, path, offset, z, speed, percent);
		iterTmpSegment = findSegment(pTmpCar->getZ());
		segments[iterTmpSegment->getIndex()].cars.push_back(*pTmpCar);
		cars.push_back(*pTmpCar);
	}
}

vector<Segment>::iterator  HelloWorld::findSegment(int z) {
	int index = (int)floor(z / segmentLength) % segments.size();
	return segments.begin()+index;
}


void HelloWorld::update(float dt) {
	iterPlayerSegment = findSegment(position + playerZ);
	double playerW = playerWidth * playerSpriteScale;
	double speedPercent = speed / maxSpeed;
	double dx = dt * 2 * speedPercent;
	double startPostion = position;
	double carW, spriteW;

	updateCars(dt, playerW);
	position = ComUtil::increase(position, dt * speed, trackLength);

	if (keyLeft) {
		playerX = playerX - dx;
	}
	else if (keyRight) {
		playerX = playerX + dx;
	}

	playerX = playerX - (dx * speedPercent * iterPlayerSegment->getCurve() * centrifugal);

	if (keyFaster) {
		speed = ComUtil::accelerate(speed, accel, dt);
	}
	else if (keySlower) {
		speed = ComUtil::accelerate(speed, breaking, dt);
	}
	else {
		speed = ComUtil::accelerate(speed, decel, dt);
	}

	if ((playerX < -1) || (playerX > 1)) {
		if (speed > offRoadLimit) {
			speed = ComUtil::accelerate(speed, offRoadDecel, dt);
		}

		for (size_t n = 0; n < iterPlayerSegment->sprites.size(); n++) {
			iterTmpSprite = iterPlayerSegment->sprites.begin() + n;
			spriteW = MySprites::getInstance()->getWidth(iterTmpSprite->getPath()) * playerSpriteScale;
			if (ComUtil::overlap(playerX, playerW, iterTmpSprite->getOffset() + spriteW / 2 * (iterTmpSprite->getOffset() > 0 ? 1 : -1), spriteW)) {
				speed = maxSpeed / 5;
				position = ComUtil::increase(iterPlayerSegment->getPoint(0).getWorld().getZ(), -playerZ, trackLength); // stop in front of sprite (at front of segment)
				break;
			}
		}
	}

	for (size_t n = 0; n < iterPlayerSegment->cars.size(); n++) {
		iterTmpCar = iterPlayerSegment->cars.begin()+n;
		carW = MySprites::getInstance()->getWidth(iterTmpCar->getPath()) * playerSpriteScale;
		if (speed > iterTmpCar->getSpeed()) {
			if (ComUtil::overlap(playerX, playerW, iterTmpCar->getOffset(), carW, 0.8)) {
				speed = iterTmpCar->getSpeed() * (iterTmpCar->getSpeed() / speed);
				position = ComUtil::increase(iterTmpCar->getZ(), -playerZ, trackLength);
				break;
			}
		}
	}

	//don't ever let player go too far out of bounds
	playerX = ComUtil::limit(playerX, -2, 2);
	// or exceed maxSpeed
	speed = ComUtil::limit(speed, 0, maxSpeed);

	skyOffset = ComUtil::increase(skyOffset, skySpeed*iterPlayerSegment->getCurve()*speedPercent, 1);
	hillOffset = ComUtil::increase(hillOffset, hillSpeed*iterPlayerSegment->getCurve()*speedPercent, 1);
	treeOffset = ComUtil::increase(treeOffset, treeSpeed*iterPlayerSegment->getCurve()*speedPercent, 1);

	showCarsAndSprite();
	showPlayer();
}

void HelloWorld::updateCars(float dt, double playerW) {
	size_t n;
	//Segment oldSegment, newSegment;
	for (n = 0; n < cars.size(); n++) {
		iterTmpCar = cars.begin()+n;
		iterOldCarSegment = findSegment(iterTmpCar->getZ());
		iterTmpCar->setOffset(iterTmpCar->getOffset() + updateCarOffset(playerW));
		if (iterTmpCar->getOffset() > 1.0 || iterTmpCar->getOffset() < -1.0) {
			CCLOG("%f", iterTmpCar->getOffset());
		}
		iterTmpCar->setZ(ComUtil::increase(iterTmpCar->getZ(), dt * iterTmpCar->getSpeed(), trackLength));
		iterTmpCar->setPercent(ComUtil::percentRemaining(iterTmpCar->getZ(), segmentLength)); // useful for interpolation during rendering phase
		cars[n] = *iterTmpCar;
		iterNewCarSegment = findSegment(iterTmpCar->getZ());
		if (*iterOldCarSegment != *iterNewCarSegment) {
			vector<MyOneCar>::iterator it;
			for (it = iterOldCarSegment->cars.begin(); it != iterOldCarSegment->cars.end(); it++) {
				if (iterTmpCar->getIndex() == it->getIndex()) {
					iterOldCarSegment->cars.erase(it);
					break;
				}
			}
			iterNewCarSegment->cars.push_back(*iterTmpCar);
		}
	}
}

double HelloWorld::updateCarOffset(double playerW) {
	size_t i, j, lookahead = 20;
	double dir;
	//Segment segment;
	double otherCarW;
	double carW = MySprites::getInstance()->getWidth(iterTmpCar->getPath()) * playerSpriteScale;

	// optimization, don't bother steering around other cars when 'out of sight' of the player
	if ((iterOldCarSegment->getIndex() - iterPlayerSegment->getIndex()) > drawDistance)
		return 0;

	for (i = 1; i < lookahead; i++) {
		iterTmpSegment = segments.begin()+((iterOldCarSegment->getIndex() + i) % segments.size());

		if ((*iterTmpSegment == *iterPlayerSegment) && (iterTmpCar->getSpeed() > speed) && (ComUtil::overlap(playerX, playerW, iterTmpCar->getOffset(), carW, 1.2))) {
			if (playerX > 0.5)
				dir = -1;
			else if (playerX < -0.5)
				dir = 1;
			else
				dir = (iterTmpCar->getOffset() > playerX) ? 1 : -1;
			return dir * 1 / i * (iterTmpCar->getSpeed() - speed) / maxSpeed; // the closer the cars (smaller i) and the greater the speed ratio, the larger the offset
		}

		for (j = 0; j < iterTmpSegment->cars.size(); j++) {
			iterOtherCar = iterTmpSegment->cars.begin()+j;
			otherCarW = MySprites::getInstance()->getWidth(iterOtherCar->getPath()) * playerSpriteScale;
			if ((iterTmpCar->getSpeed() > iterOtherCar->getSpeed()) && ComUtil::overlap(iterTmpCar->getOffset(), carW, iterOtherCar->getOffset(), otherCarW, 1.2)) {
				if (iterOtherCar->getOffset() > 0.5)
					dir = -1;
				else if (iterOtherCar->getOffset() < -0.5)
					dir = 1;
				else
					dir = (iterTmpCar->getOffset() > iterOtherCar->getOffset()) ? 1 : -1;
				return dir * 1 / i * (iterTmpCar->getSpeed() - iterOtherCar->getSpeed()) / maxSpeed;
			}
		}
	}

	// if no cars ahead, but I have somehow ended up off road, then steer back on
	if (iterTmpCar->getOffset() < -0.9)
		return 0.1;
	else if (iterTmpCar->getOffset() > 0.9)
		return -0.1;
	else
		return 0;

}

void HelloWorld::draw() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //cocos2d默认混合模式，如果没有设置blend混合模式，默认没有透明度
	calcSlider();
	render();
}

void HelloWorld::render() {
	iterBaseSegment = findSegment(position);
	double basePercent = ComUtil::percentRemaining(position, segmentLength);
	iterPlayerSegment = findSegment(position + playerZ);
	double playerPercent = ComUtil::percentRemaining(position + playerZ, segmentLength);
	double playerY = ComUtil::interpolate(iterPlayerSegment->getPoint(0).getWorld().getY(),
		iterPlayerSegment->getPoint(1).getWorld().getY(), playerPercent);
	double maxy = height;
	//Segment segment;

	double x = 0;
	double dx = -(iterBaseSegment->getCurve()*basePercent);
	size_t n;

	//if (playerY != 0.0) {
	//	CCLOG("playerY=%6.6f, Y1=%6.6f, Y2=%6.6f, position=%6.6f", playerY,
	//		pPlayerSegment->getPoint(0).getWorld().getY(),
	//		pPlayerSegment->getPoint(1).getWorld().getY(),
	//		position);
	//}

	for (n = 0; n < drawDistance; n++) {
		iterTmpSegment = segments.begin()+((iterBaseSegment->getIndex() + n) % segments.size());
		iterTmpSegment->setLooped(iterTmpSegment->getIndex() < iterBaseSegment->getIndex());
		iterTmpSegment->setFog(ComUtil::exponentialFog(n / drawDistance, fogDensity));
		iterTmpSegment->setClip(maxy);

		iterTmpSegment->project((playerX * roadWidth), x, dx, playerY + cameraHeight,
			position - (iterTmpSegment->isLooped() ? trackLength : 0), cameraDepth, width, height, roadWidth);
		segments[iterTmpSegment->getIndex()] = *iterTmpSegment;
		x = x + dx;
		dx = dx + iterTmpSegment->getCurve();

		if ((iterTmpSegment->getPoint(0).getCamera().getZ() <= cameraDepth) || // behind us
			(iterTmpSegment->getPoint(1).getScreen().getY() >= iterTmpSegment->getPoint(0).getScreen().getY()) ||//back face cull
			(iterTmpSegment->getPoint(1).getScreen().getY() >= maxy)) {        // clip by (already rendered) segment
			continue;
		}

		iterTmpSegment->show(width, lanes);
		maxy = iterTmpSegment->getPoint(0).getScreen().getY();
	}
}

void HelloWorld::showCarsAndSprite() {
	//Segment baseSegment = findSegment(position);
	//Segment playerSegment = findSegment(position + playerZ);
	//Segment segment;
	double spriteScale, spriteX, spriteY;
	double scaleProject, scale;
	string path;
	size_t n, i;
	double originX, originY;
	double originW, originH;
	double destW, destH;
	double clipH, clipY;

	for (int j = 0; j < MAX_CAR_NUM; j++) {
		pOtherCarSprite[j]->setVisible(false);
	}
	for (int j = 0; j < MAX_SPRITE_NUM; j++) {
		pOtherTreeSprite[j]->setVisible(false);
	}
	for (n = (drawDistance - 1); n > 0; n--) {
		iterTmpSegment = segments.begin()+((iterBaseSegment->getIndex() + n) % segments.size());

		for (i = 0; i < iterTmpSegment->cars.size(); i++) {
			iterTmpCar = iterTmpSegment->cars.begin()+i;
			path = iterTmpSegment->cars[i].getPath();
			spriteScale = ComUtil::interpolate(iterTmpSegment->getPoint(0).getScreen().getScale(), iterTmpSegment->getPoint(1).getScreen().getScale(), iterTmpCar->getPercent());
			spriteX = ComUtil::interpolate(iterTmpSegment->getPoint(0).getScreen().getX(), iterTmpSegment->getPoint(1).getScreen().getX(), iterTmpCar->getPercent()) + (spriteScale * iterTmpCar->getOffset() * roadWidth * width / 2);
			spriteY = height - ComUtil::interpolate(iterTmpSegment->getPoint(0).getScreen().getY(), iterTmpSegment->getPoint(1).getScreen().getY(), iterTmpCar->getPercent());
			
			for (int j = 0; j < MAX_CAR_NUM; j++) {
				if (!pOtherCarSprite[j]->isVisible()) {
					MySprites::getInstance()->setDisplayFrame(pOtherCarSprite[j], path);
					scaleProject = spriteScale * width / 2.0;	//scale for projection
					scale = scaleProject * playerSpriteScale * roadWidth;
					originW = MySprites::getInstance()->getWidth(path);
					originH = MySprites::getInstance()->getHeight(path);
					destW = originW * scale;
					destH = originH * scale;
					clipY = height - iterTmpSegment->getClip();
					clipH = clipY ? ComUtil::maxValue(0, clipY - spriteY) : 0;
					if (clipH >= destH) {
						break;
					}

					pOtherCarSprite[j]->setRotation(0.0f);
					pOtherCarSprite[j]->setZOrder(drawDistance - n);
					pOtherCarSprite[j]->setPosition(ccp(spriteX, clipH <= 0 ? spriteY : spriteY + clipH));
					if (clipH > 0) {
						originX = MySprites::getInstance()->getOriginX(path);
						originY = MySprites::getInstance()->getOriginY(path);
						CCRect rect;
						if (MySprites::getInstance()->isRotated(path)) {
							rect.setRect(originX + clipH / scale, originY, (destH - clipH) / scale, originW);
							pOtherCarSprite[j]->setRotation(-90.0f);
							pOtherCarSprite[j]->setPosition(ccp(spriteX + destW / 2, spriteY + destH / 2));
						}
						else {
							rect.setRect(originX, originY, originW, (destH - clipH) / scale);
						}
						pOtherCarSprite[j]->setTextureRect(rect);
					}
					pOtherCarSprite[j]->setScale(scale);
					pOtherCarSprite[j]->setVisible(true);
					break;
				}
			}
		}

		for (i = 0; i < iterTmpSegment->sprites.size(); i++) {
			iterTmpSprite = iterTmpSegment->sprites.begin()+i;
			path = iterTmpSegment->sprites[i].getPath();
			spriteScale = iterTmpSegment->getPoint(0).getScreen().getScale();
			spriteX = iterTmpSegment->getPoint(0).getScreen().getX() + (spriteScale * iterTmpSprite->getOffset() * roadWidth * width / 2);
			spriteY = height - iterTmpSegment->getPoint(0).getScreen().getY();
			for (int j = 0; j < MAX_SPRITE_NUM; j++) {
				if (!pOtherTreeSprite[j]->isVisible()) {
					MySprites::getInstance()->setDisplayFrame(pOtherTreeSprite[j], path);
					scaleProject = spriteScale * width / 2.0;	//scale for projection
					scale = scaleProject * playerSpriteScale * roadWidth;
					originW = MySprites::getInstance()->getWidth(path);
					originH = MySprites::getInstance()->getHeight(path);
					destW = originW * scale;
					destH = originH * scale;
					clipY = height - iterTmpSegment->getClip();
					clipH = clipY ? ComUtil::maxValue(0, clipY - spriteY) : 0;
					if (clipH >= destH) {
						break;
					}

					pOtherTreeSprite[j]->setRotation(0.0f);
					pOtherTreeSprite[j]->setZOrder(drawDistance - n);
					pOtherTreeSprite[j]->setAnchorPoint(ccp((iterTmpSprite->getOffset() < 0 ? 1 : 0), 0));
					pOtherTreeSprite[j]->setPosition(ccp(spriteX, clipH <= 0 ? spriteY : spriteY + clipH));
					if (clipH > 0) {
						originX = MySprites::getInstance()->getOriginX(path);
						originY = MySprites::getInstance()->getOriginY(path);
						CCRect rect;
						if (MySprites::getInstance()->isRotated(path)) {
							rect.setRect(originX + clipH / scale, originY, (destH - clipH) / scale, originW);
							pOtherTreeSprite[j]->setRotation(-90.0f);
							if (iterTmpSprite->getOffset() < 0) {
								pOtherTreeSprite[j]->setPosition(
									ccp(spriteX, spriteY + destH));
							}
							else {
								pOtherTreeSprite[j]->setPosition(
									ccp(spriteX + destW, spriteY + clipH));
							}
						}
						else {
							rect.setRect(originX, originY, originW, (destH - clipH) / scale);
						}
						pOtherTreeSprite[j]->setTextureRect(rect);
					}
					pOtherTreeSprite[j]->setScale(scale);
					pOtherTreeSprite[j]->setVisible(true);
					break;
				}
			}
		}

		//if (segment == playerSegment) {
		//}
	}
}

void HelloWorld::showPlayer() {
	double speedPercent = speed / maxSpeed;
	vector<int> v;
	v.push_back(-1);
	v.push_back(1);
	double bounce = (1.5 * CCRANDOM_0_1() * speedPercent * resolution) * ComUtil::randomChoice(v);

	//Segment playerSegment = findSegment(position + playerZ);
	double playerPercent = ComUtil::percentRemaining(position + playerZ, segmentLength);
	
	double steer = speed * (keyLeft ? -1 : keyRight ? 1 : 0);
	string carSpriteFrameName;
	double updown = iterPlayerSegment->getPoint(1).getWorld().getY() - iterPlayerSegment->getPoint(0).getWorld().getY();//is climbing hill
	if (steer < 0) {
		carSpriteFrameName = (updown > 0) ? "player_uphill_left" :"player_left";
	}
	else if (steer > 0) {
		carSpriteFrameName = (updown > 0) ? "player_uphill_right" : "player_right";
	}
	else {
		carSpriteFrameName = (updown > 0) ? "player_uphill_straight" : "player_straight";
	}
	MySprites::getInstance()->setDisplayFrame(pMyCarSprite, carSpriteFrameName);
	pMyCarSprite->setPosition(ccp(width / 2, 0 + bounce));

	double scaleProject = cameraDepth / playerZ * width / 2.0;	//scale for projection
	double scale = scaleProject * playerSpriteScale * roadWidth;
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


#ifdef __ENABLE_DIRECTION__
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

void HelloWorld::registerWithTouchDispatcher() {
	//add touch pispatcher
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);
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
#endif

void HelloWorld::keyboardHook(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			keyLeft = true;
			break;
		case VK_RIGHT:
			keyRight = true;
			break;
		case VK_UP:
			keyFaster = true;
			break;
		case VK_DOWN:
			keySlower = true;
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			keyLeft = false;
			break;
		case VK_RIGHT:
			keyRight = false;
			break;
		case VK_UP:
			keyFaster = false;
			break;
		case VK_DOWN:
			keySlower = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
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
	addChild(ttfName, 500);

	(*slider) = new SliderCtrl();
	(*slider)->create(this, min, max, cur, 940, height, 0.75f);
	addChild((*slider)->getSlider(), 500);
}

void HelloWorld::calcSlider() {
	if (pRoadWidthSlider) {
		roadWidth = pRoadWidthSlider->getSlider()->getValue();
		playerSpriteScale = 0.3 * (1 / playerWidth);
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
}