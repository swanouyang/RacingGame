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
	double curve;
	bool loop;
	double fog;

public:
	Segment();
	Segment(int n, ComPoint p1, ComPoint p2, ComColor color, bool loop);
	void setIndex(const int n);
	void setPoint(const ComPoint point, int index);
	void setColor(const ComColor color);
	void setCurve(const double curve);
	void setLooped(const bool loop);
	void setFog(const double fog);
	int getIndex() const;
	ComPoint getPoint(int index) const;
	ComColor getColors() const;
	double getCurve() const;
	bool isLooped() const;
	double getFog() const;

	void project(double cameraX, double x, double dx, double cameraY, double cameraZ, double cameraDepth,
		double width, double height, double roadWidth);
	void show(double width, double lanes);
	double getRumbleWidth(double projectedRoadWidth, double lanes);
	double getLaneMarkerWidth(double projectedRoadWidth, double lanes);
};

#endif	//__SEGMENT_H__