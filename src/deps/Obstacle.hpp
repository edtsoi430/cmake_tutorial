#ifndef _OBSTACLE_HPP_
#define _OBSTACLE_HPP_

#include "Entity.hpp"
#include "AnimatedEntity.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include <cstdint>
#include <vector>
#include <functional>

class Obstacle : public AnimatedEntity
{
public:
	Obstacle(float xInit, float yInit, float yawInit, float radiusInit,
		const float *colorIn, Program *program);
	virtual void render();
	virtual void update();
	float getSpeed();
	void setSpeed(float newSpeed);
	std::function<void(Obstacle&)> updateLocation;
private:
	Circle border;
	Circle roombaBody;
	Circle pillar;
	Circle outerBorder;
	Circle innerBorder;
	Circle insidePillar;
};

#endif
