#ifndef _VEHICLE_HPP_
#define _VEHICLE_HPP_

#include "Entity.hpp"
#include "AnimatedEntity.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include <cstdint>
#include <vector>

class Vehicle : public AnimatedEntity
{
public:
	Vehicle(float xInit, float yInit, float yawInit, float radiusInit,
		const float* colorIn, Program *program);
	virtual void update();
	virtual void render();
private:
	Rectangle body;
	std::vector<Rectangle> ring;
	Rectangle bar1;
	Rectangle bar2;
	Circle playerIdentifier;
	std::vector<Circle> propCaps;
	std::vector<Rectangle> props;
	int numRects = Constants::numRectsInRing;
	float thetaIncrement = 2 * PI / Constants::numRectsInRing;
	float propAngle = 0.0f;
};

#endif
