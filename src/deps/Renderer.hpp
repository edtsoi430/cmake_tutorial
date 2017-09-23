#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <array>
#include <vector>

#include <epoxy/gl.h> // OpenGL function pointer manager
#include <glm/glm.hpp> // OpenGL math library

#include "Program.hpp"
#include "Vehicle.hpp"
#include "Roomba.hpp"
#include "Obstacle.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();
	/**
	* @brief Draws a scene to the offscreen framebuffer given current state of
	* the simulator objects
	*
	* @param INCLUDE ANY PARAMS NECESSARY FOR SIMULATOR OBJECTS
	* Pass in the vectors of Entity* to be rendered as well as the vehicle
	* as an Entity*
	*/
	void render(const std::vector<Vehicle>& vehicles,
		    const std::vector<Roomba>& roombas,
		    const std::vector<Obstacle>& obstacles);

	/**
	* @brief Blits the drawn scene from the offscreen framebuffer to the
	* currently bound framebuffer
	*/
	void blit();

	/**
	* @breif Updates the renderer to use a new framebuffer size
	*
	* @param size The new size to render with
	*/
	void resize(const glm::ivec2& size);

	Program* getProgram();

	void addRectangle(float xInit, float yInit, float yawInit, float radiusInit,
		const float *colorIn, Program *program, float widthIn, float heightIn);

private:
	// Color array when using changing colors
	float color[3] = {Constants::defaultChangeColors[0],Constants::defaultChangeColors[1],
		Constants::defaultChangeColors[2]};
	// updates the clear color if using the changing colors mode
	void updateColor();
	// Offscreen rendering objects
	GLuint OFB; // offscreen framebuffer
	GLuint ORB; //offscreen renderbuffers

	// Shader program class that holds the shader program and ids of certain things
	Program program;
	// The field composed of rectangles
	std::vector<Rectangle> field;

	// Current image size
	glm::ivec2 curSize;

	// Controls the chaning of the colors
	float colorChangers[3] = {Constants::changeColor[0],Constants::changeColor[1],
		Constants::changeColor[2]};
};

#endif
