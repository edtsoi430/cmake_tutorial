#include "trainingProject1.hpp"

void arenaSimulator::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0,0,width,height);
}


void arenaSimulator::process_input(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window,true);
	}
}


void arenaSimulator::updateTheta(float &thetaVal, float incrementVal)
{
	thetaVal += incrementVal;
	if (thetaVal > 2.0f * 3.14159265f - 0.01f)
	{
		thetaVal = 0.0f;
	}
}

void arenaSimulator::render(std::atomic<bool> *run, arenaSimulator *arenaSim)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800,800,"arenaSim",NULL,NULL);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	if (window == NULL)
	{
		std::cout << "Failed to initialize window... Closing...\n";
		glfwTerminate();
		*run = false;
		return;
	}
	glfwMakeContextCurrent(window);
	glViewport(0,0,800,800);
	// Initialize renderers (for circles and such)

	// Initialize array of floats that control clear color
	float colors[3] {1.0f,1.0f,1.0f};
	while(*run && !glfwWindowShouldClose(window))
	{
		// Call setup functions for each object to be rendered

		// Process keypresses
		process_input(window);
		// Rendering commands below
		glClearColor(colors[0], colors[1], colors[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Call render functions of objects

		// Swap the buffers
		glfwSwapBuffers(window);
		// Sleep to take load off of my crappy vm TODO TODO TODO
		std::this_thread::sleep_for(std::chrono::milliseconds(60));
		// Poll for events
		glfwPollEvents();
	}
	if (glfwWindowShouldClose(window))
	{
		*run = false;
	}
	glfwTerminate();
}
