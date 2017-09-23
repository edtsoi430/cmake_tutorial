#include "Entity.hpp"

Entity::Entity(float xInit, float yInit, float yawInit, float radiusInit, Program *program) :
	x {xInit}, y {yInit}, yaw {yawInit}, radius {radiusInit},
	shaderProgramId {program->prog()}
{

}

void Entity::setPosition(float inX, float inY)
{
	x = inX;
	y = inY;
}

void Entity::setYaw(float inTheta)
{
	yaw = inTheta;
}

float Entity::getXPos() const
{
	return x;
}

float Entity::getYPos() const
{
	return y;
}

float Entity::getYaw() const
{
	return yaw;
}

float Entity::getRadius() const
{
	return radius;
}

void Entity::updateTheta(float &thetaVal, float incrementVal)
{
	thetaVal += incrementVal;
	if (thetaVal > 2.0f * 3.14159265f - 0.01f)
	{
		thetaVal = 0.0f;
	}
}

