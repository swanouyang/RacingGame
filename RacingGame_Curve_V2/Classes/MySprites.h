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
};

#endif