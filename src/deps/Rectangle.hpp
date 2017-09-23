#ifndef _RECTANGLE_HPP_
#define _RECTANGLE_HPP_

#include "Entity.hpp"

class Rectangle : public Entity
{
public:
	Rectangle(float xInit, float yInit, float yawInit, float radiusInit,
		const float *colorIn, Program *program, float widthIn, float heightIn);
	virtual void update();
	virtual void render();
	float getWidth();
	float getHeight();
private:
	// Characteristics by which renderData is determined
	float width;
	float height;
	float color[3];
	// Data holding the opengl object ids needed to render
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	// Holds the vertices to be put into an EBO buffer
	unsigned int rectangleIndices[6] {0,3,2,2,3,1};
	// Holds the vertices to be rendered as well as the color to use
	float renderData[24];
};

#endif
