#include "Segment.h"

Segment::Segment() {

}

Segment::Segment(int n, ComPoint p1, ComPoint p2, ComColor color, bool loop) {
	this->index = n;
	this->p[0] = p1;
	this->p[1] = p2;
	this->color = color;
	this->loop = loop;
}

void Segment::setIndex(int n) {
	this->index = n;
}

void Segment::setPoint(const ComPoint point, int index) {
	this->p[index] = point;
}

void Segment::setColor(const ComColor color) {
	this->color = color;
}

void Segment::setLooped(const bool loop) {
	this->loop = loop;
}

void Segment::setFog(const float fog) {
	this->fog = fog;
}

int Segment::getIndex() const {
	return this->index;
}

ComPoint Segment::getPoint(int index) const {
	return this->p[index];
}

ComColor Segment::getColors() const {
	return this->color;
}

bool Segment::isLooped() const {
	return this->loop;
}

float Segment::getFog() const {
	return this->fog;
}

void Segment::project(float cameraX, float cameraY, float cameraZ, float cameraDepth,
	float width, float height, float roadWidth) {
	for (int n = 0; n < MAX_POINTS; n++) {
		p[n].getCamera().setX((p[n].getWorld().getX()/* || 0*/) - cameraX);
		p[n].getCamera().setY((p[n].getWorld().getY()/* || 0*/) - cameraY);
		p[n].getCamera().setZ((p[n].getWorld().getZ()/* || 0*/) - cameraZ);
		p[n].getScreen().setScale(cameraDepth / p[n].getCamera().getZ());
		p[n].getScreen().setX(round((width / 2) + (p[n].getScreen().getScale() * p[n].getCamera().getX() * width / 2)));
		p[n].getScreen().setY(round((height / 2) - (p[n].getScreen().getScale() * p[n].getCamera().getY() * height / 2)));
		p[n].getScreen().setW(round((p[n].getScreen().getScale() * roadWidth * width / 2)));
	}
}

void Segment::show(float width, int lanes) {
	float x1 = p[0].getScreen().getX();
	float y1 = p[0].getScreen().getY();
	float w1 = p[0].getScreen().getW();
	float x2 = p[1].getScreen().getX();
	float y2 = p[1].getScreen().getY();
	float w2 = p[1].getScreen().getW();
	
	float r1 = getRumbleWidth(w1, lanes),
		r2 = getRumbleWidth(w2, lanes),
		l1 = getLaneMarkerWidth(w1, lanes),
		l2 = getLaneMarkerWidth(w2, lanes),
		lanew1, lanew2, lanex1, lanex2, lane;

	DRAW_SOLID_RECT(0.f, y2, width, y1 - y2, color.getGrass(), 1.f);
	
	CCPoint points[] = { 
		ccp(x1 - w1 - r1, SCREEN_HEIGHT - y1),
		ccp(x1 - w1, SCREEN_HEIGHT - y1),
		ccp(x2 - w2, SCREEN_HEIGHT - y2),
		ccp(x2 - w2 - r2, SCREEN_HEIGHT - y2)
	};
	DRAW_SOLID_POLY(points, 4, color.getRumble());
	points[0].x = x1 + w1 + r1;
	points[1].x = x1 + w1;
	points[2].x = x2 + w2;
	points[3].x = x2 + w2 + r2;
	DRAW_SOLID_POLY(points, 4, color.getRumble());
	points[0].x = x1 - w1;
	points[3].x = x2 - w2;
	DRAW_SOLID_POLY(points, 4, color.getRoad());
	//Render.polygon(ctx, x1 - w1 - r1, y1, x1 - w1, y1, x2 - w2, y2, x2 - w2 - r2, y2, color.rumble);
	//Render.polygon(ctx, x1 + w1 + r1, y1, x1 + w1, y1, x2 + w2, y2, x2 + w2 + r2, y2, color.rumble);
	//Render.polygon(ctx, x1 - w1, y1, x1 + w1, y1, x2 + w2, y2, x2 - w2, y2, color.road);

	if (!color.getLane().empty()) {
		lanew1 = w1 * 2 / lanes;
		lanew2 = w2 * 2 / lanes;
		lanex1 = x1 - w1 + lanew1;
		lanex2 = x2 - w2 + lanew2;
		for (lane = 1; lane < lanes; lanex1 += lanew1, lanex2 += lanew2, lane++) {
			points[0].x = lanex1 - l1 / 2;
			points[1].x = lanex1 + l1 / 2;
			points[2].x = lanex2 + l2 / 2;
			points[3].x = lanex2 - l2 / 2;
			DRAW_SOLID_POLY(points, 4, color.getLane());
			//Render.polygon(ctx, lanex1 - l1 / 2, y1, lanex1 + l1 / 2, y1, lanex2 + l2 / 2, y2, lanex2 - l2 / 2, y2, color.lane);
		}
	}

	DRAW_SOLID_RECT(0.f, y2, width, y1 - y2, COLOR_FOG, 1-fog);
	//Render.fog(ctx, 0, y1, width, y2 - y1, fog);
}

float Segment::getRumbleWidth(float projectedRoadWidth, int lanes) {
	return projectedRoadWidth / fmaxf(6, 2 * lanes);
}

float Segment::getLaneMarkerWidth(float projectedRoadWidth, int lanes) {
	return projectedRoadWidth / fmaxf(32, 8 * lanes);
}