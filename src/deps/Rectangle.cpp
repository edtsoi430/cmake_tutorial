#include "Rectangle.hpp"

Rectangle::Rectangle(float xInit, float yInit, float angleInit, float yawInit,
	const float *colorIn, Program *program, float widthIn, float heightIn) :
	Entity(xInit,yInit,yawInit,yawInit,program),
	width {widthIn}, height {heightIn}
{
	// Initialize color
	color[0] = colorIn[0];
	color[1] = colorIn[1];
	color[2] = colorIn[2];
	// Creates required openGL buffers
	glGenBuffers(1,&VBO);
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&EBO);
	// Binds buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	// Create attrib pointers
	glVertexAttribPointer(program->vert(),3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)0);
	glEnableVertexAttribArray(program->vert());
	glVertexAttribPointer(program->color(),3,GL_FLOAT,GL_FALSE,6 * sizeof(float),
		(void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(program->color());
	// Create Element buffer data and store it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(rectangleIndices),rectangleIndices,
		GL_STATIC_DRAW);
	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	// Sets the color in Render data, as well as depth (z)
	// Only needs to be set once
	for (int i {3}; i < 24; i += 6)
	{
		for (int ii {0}; ii < 3; ++ii)
		{
			renderData[i + ii] = color[ii];
		}
		renderData[i - 1] = 0.0f;
	}
	// Initializes the vertices
	update();
}

void Rectangle::update()
{
	float theta = atan(height/width);
	float thetaGlobal = theta + yaw - PI / 2;
	float thetaGlobal2 = thetaGlobal + (PI / 2 - theta) * 2;
	float hDiv2 = height / 2;
	float wDiv2 = width / 2;
	float distToVertex = sqrt(hDiv2 * hDiv2 + wDiv2 * wDiv2);
	renderData[0] = cos(thetaGlobal) * distToVertex + x;
	renderData[0] = (renderData[0] - ARENASIZEX / 2) / (ARENASIZEX / 2);
	renderData[1] = sin(thetaGlobal) * distToVertex + y;
	renderData[1] = (renderData[1] - ARENASIZEY / 2) / (ARENASIZEY / 2);
	renderData[6] = -cos(thetaGlobal) * distToVertex + x;
	renderData[6] = (renderData[6] - ARENASIZEX / 2) / (ARENASIZEX / 2);
	renderData[7] = -sin(thetaGlobal) * distToVertex + y;
	renderData[7] = (renderData[7] - ARENASIZEY / 2) / (ARENASIZEY / 2);
	renderData[12] = cos(thetaGlobal2) * distToVertex + x;
	renderData[12] = (renderData[12] - ARENASIZEX / 2) / (ARENASIZEX / 2);
	renderData[13] = sin(thetaGlobal2) * distToVertex + y;
	renderData[13] = (renderData[13] - ARENASIZEY / 2) / (ARENASIZEY / 2);
	renderData[18] = -cos(thetaGlobal2) * distToVertex + x;
	renderData[18] = (renderData[18] - ARENASIZEX / 2) / (ARENASIZEX / 2);
	renderData[19] = -sin(thetaGlobal2) * distToVertex + y;
	renderData[19] = (renderData[19] - ARENASIZEY / 2) / (ARENASIZEY / 2);
}

void Rectangle::render()
{
	glUseProgram(shaderProgramId);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(renderData),renderData,GL_STREAM_DRAW);
	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

float Rectangle::getWidth()
{
	return width;
}

float Rectangle::getHeight()
{
	return height;
}
