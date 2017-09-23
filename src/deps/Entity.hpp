#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <cmath>
#include <iostream>
#include <cstdint>
#include <epoxy/gl.h>
#include "Program.hpp"
#include "Constants.hpp"

constexpr static float ARENASIZEX = Constants::arenaSizeX;
constexpr static float ARENASIZEY = Constants::arenaSizeY;
constexpr static float PI = Constants::pi;

enum class LinePosition {top, bottom, left, right};

class Entity
{
public:
	// Constructor, creates entity, pass in all initial conditions and shader program id
	Entity(float xInit, float yInit, float yawInit, float radiusInit, Program *program);
	// Set the new position
	virtual void setPosition(float inX, float inY);
	// Set the new yaw in Radians
	virtual void setYaw(float inTheta);
	// Update the position of sub objects based on new position
	virtual void update() = 0;
	// Puts render data into openGL buffer and  renders this object
	virtual void render() = 0;
	// return current x coordinate
	virtual float getXPos() const;
	// return current y coordinate
	virtual float getYPos() const;
	// return current yaw
	virtual float getYaw() const;
	// Defines the collision radius
	virtual float getRadius() const;
protected:
	// Position, orientation, color data, as well as the shader program to use
	float x;
	float y;
	float yaw;
	float radius;
	GLuint shaderProgramId;
	// Constant expressions to
	constexpr static float ARENASIZEX = Constants::arenaSizeX;
	constexpr static float ARENASIZEY = Constants::arenaSizeY;
	constexpr static float PI = Constants::pi;
	static void updateTheta(float &thetaVal, float incrementVal);
};

#endif
