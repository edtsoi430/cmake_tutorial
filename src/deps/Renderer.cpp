#include "Renderer.hpp"
#include "Constants.hpp"
#include <iostream>

using std::cerr;
using std::vector;

Renderer::Renderer()
{
	auto cc = Constants::clearColor;
	glClearColor(cc[0], cc[1], cc[2], 0.f);
	// set up offscreen  rendering
	glGenFramebuffers(1, &OFB);
	glBindFramebuffer(GL_FRAMEBUFFER, OFB);
	glGenRenderbuffers(1, &ORB);
	glBindRenderbuffer(GL_RENDERBUFFER, ORB);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_RGB,800,600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ORB);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer()
{
	glDeleteRenderbuffers(1, &ORB);
	glDeleteFramebuffers(1, &OFB);
}

void Renderer::render(const std::vector<Vehicle>& vehicles,
		      const std::vector<Roomba>& roombas,
		      const std::vector<Obstacle>& obstacles)
{
	//recreateRenderBuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, OFB);
	// Check the frame buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cerr << "FRAMEBUFFER ISSUES DETECTED!\n";
		cerr << "Your error is: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";
		cerr << "Codes are:\n";
		cerr << "\t" << GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT << " incomplete attachment\n";
		cerr << "\t" << GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS << " incomplete dimensions\n";
		cerr << "\t" << GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT << " incomplete missing attachment\n";
		cerr << "\t" << GL_FRAMEBUFFER_UNSUPPORTED << " unsupported\n";
		exit(-1);
	}
	if (Constants::changeColors)
	{
		glClearColor(color[0],color[1],color[2],1.0f);
		updateColor();
	}
	else
	{
		glClearColor(Constants::clearColor[0],Constants::clearColor[1],
			Constants::clearColor[2],1.0f);
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto rectangle : field)
	{
		rectangle.render();
	}
	for (auto roomba : roombas)
	{
		roomba.update();
		roomba.render();
	}
	for (auto obstacle : obstacles)
	{
		obstacle.update();
		obstacle.render();
	}
	// Renders the vehicle last
	for (auto vehicle : vehicles)
	{
		vehicle.update();
		vehicle.render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Renderer::updateColor()
{
	for (int i {0}; i < 3; ++i)
	{
		color[i] += colorChangers[i];
		if (color[i] < 0.f || color[i] > 1.f)
			colorChangers[i] *= -1;
	}
}

Program* Renderer::getProgram()
{
	return &program;
}

void Renderer::blit()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, OFB);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, curSize.x, curSize.y, 0, 0, curSize.x, curSize.y,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
}

void Renderer::resize(const glm::ivec2& size)
{
	curSize = size;
	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, OFB);
	glBindRenderbuffer(GL_RENDERBUFFER, ORB);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB565, size.x, size.y);
}

void Renderer::addRectangle(float xInit, float yInit, float yawInit, float radiusInit,
	const float *colorIn, Program *program, float widthIn, float heightIn)
{
	field.emplace_back(xInit,yInit,yawInit,radiusInit,colorIn,program,widthIn,heightIn);
	field.back().setYaw(yawInit);
	field.back().update();
}
