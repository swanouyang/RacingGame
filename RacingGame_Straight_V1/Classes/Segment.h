#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include "Common.h"
using namespace std;

const int MAX_POINTS = 2;

class Segment {
private:
	int index;
	ComPoint p[MAX_POINTS];
	ComColor color;
	bool loop;
	float fog;

public:
	Segment();
	Segment(int n, ComPoint p1, ComPoint p2, ComColor color, bool loop);
	void setIndex(const int n);
	void setPoint(const ComPoint point, int index);
	void setColor(const ComColor color);
	void setLooped(const bool loop);
	void setFog(const float fog);
	int getIndex() const;
	ComPoint getPoint(int index) const;
	ComColor getColors() const;
	bool isLooped() const;
	float getFog() const;

	void project(float cameraX, float cameraY, float cameraZ, float cameraDepth, 
		float width, float height, float roadWidth);
	void show(float width, int lanes);
	float getRumbleWidth(float projectedRoadWidth, int lanes);
	float getLaneMarkerWidth(float projectedRoadWidth, int lanes);
};

#endif	//__SEGMENT_H__