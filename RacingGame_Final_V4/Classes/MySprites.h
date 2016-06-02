#ifndef __MY_SPITES_H__
#define __MY_SPITES_H__

#include "Common.h"
using namespace std;

const map<string, string> g_SpriteMap = {
	{ "player_left",				"player_left.png" },
	{ "player_right",				"player_right.png" },
	{ "player_straight",			"player_straight.png" },
	{ "player_uphill_left",			"player_uphill_left.png" },
	{ "player_uphill_right",		"player_uphill_right.png" },
	{ "player_uphill_straight",		"player_uphill_straight.png" },

	{ "directions",					"Directions.png" },

	{ "sliderProgress2", "sliderProgress2.png" },
	{ "sliderTrack2", "sliderTrack2.png" },
	{ "sliderThumb", "sliderThumb.png" },

	{ "billboard01", "billboard01.png" },
	{ "billboard02", "billboard02.png" },
	{ "billboard03", "billboard03.png" },
	{ "billboard04", "billboard04.png" },
	{ "billboard05", "billboard05.png" },
	{ "billboard06", "billboard06.png" },
	{ "billboard07", "billboard07.png" },
	{ "billboard08", "billboard08.png" },
	{ "billboard09", "billboard09.png" },

	{ "boulder1", "boulder1.png" },
	{ "boulder2", "boulder2.png" },
	{ "boulder3", "boulder3.png" },

	{ "bush1", "bush1.png" },
	{ "bush2", "bush2.png" },

	{ "cactus", "cactus.png" },

	{ "car01", "car01.png" },
	{ "car02", "car02.png" },
	{ "car03", "car03.png" },
	{ "car04", "car04.png" },

	{ "column", "column.png" },

	{ "dead_tree1", "dead_tree1.png" },
	{ "dead_tree2", "dead_tree2.png" },

	{ "palm_tree", "palm_tree.png" },

	{ "semi", "semi.png" },
	{ "stump", "stump.png" },
	{ "tree1", "tree1.png" },
	{ "tree2", "tree2.png" },
	{ "truck", "truck.png" },
};

const int g_BillBoardsNum = 9;
const string g_BillBoardsName[g_BillBoardsNum] = {
	"billboard01",
	"billboard02",
	"billboard03",
	"billboard04",
	"billboard05",

	"billboard06",
	"billboard07",
	"billboard08",
	"billboard09",
};
const int g_PlantsNum = 12;
const string g_PlantsName[g_PlantsNum] = {
	"tree1",
	"tree2",
	"dead_tree1",
	"dead_tree2",
	"palm_tree",

	"bush1",
	"bush2",
	"cactus",
	"stump",
	"boulder1",

	"boulder2",
	"boulder3",
};
const int g_CarsNum = 6;
const string g_CarsName[g_CarsNum] = {
	"car01",
	"car02",
	"car03",
	"car04",
	"semi",
	"truck",
};

class MySprites : public CCNode {
private:
	CCSpriteFrameCache *frameCache;
	MySprites();

	static MySprites* s_MySprites;
public:
	
	static MySprites* getInstance();
	static void destroy();
	bool init();
	void setDisplayFrame(CCSprite * sprite, string name);
	static double getWidth(CCSprite* sprite);//获取sprite本身的 width
	static double getHeight(CCSprite* sprite);//获取sprite本身的 height
	double getWidth(string name);//获取name.png本身的 width
	double getHeight(string name);//获取name.png本身的 height
	double getOriginX(string name);
	double getOriginY(string name);
	bool isRotated(string name) const;
	
	
	static void getWidthAndHeight(CCSprite* sprite, double& width, double& height);
	void getWidthAndHeight(string name, double& width, double& height);
	static double getPicWidth(CCSprite* sprite);//获取sprite所在大图的 width
	static double getPicHeight(CCSprite* sprite);//获取sprite所在大图的 height
	static void getPicWidthAndHeight(CCSprite* sprite, double& width, double& height);
};

class MyOneSprite {
protected:
	string path;
	double offset;
public :
	MyOneSprite();
	MyOneSprite(string path, double offset);
	void setPath(string path);
	void setOffset(double offset);
	string getPath() const;
	double getOffset() const;
};

class MyOneCar : public MyOneSprite {
private:
	int index;
	double z;
	double speed;
	double percent;
public:
	MyOneCar();
	bool operator==(MyOneCar car);
	void setIndex(int index);
	void setZ(double z);
	double getZ() const;
	void setSpeed(double speed);
	int getIndex() const;
	double getSpeed() const;
	void setPercent(double percent);
	double getPercent() const;
	void setAll(int index, string path, double offset, double z, double speed, double percent);
};

#endif //__MY_SPITES_H__