#ifndef _CIRCLE_HPP_
#define _CIRCLE_HPP_

#include "Entity.hpp"

class Circle : public Entity
{
public:
	Circle(float xInit, float yInit, float angleInit, float radiusInit,
		const float colorIn[], Program *program);
	virtual void update();
	virtual void render();
private:
	float color[3];
	// Data holding the opengl object ids needed to render
	unsigned int VBO;
	unsigned int VAO;
	// Holds the vertices to be rendered as well as the color to use
	// Will contain 360 triangles to be rendered to make a circle
	float renderData[360 * 9 * 2];
};

#endif
