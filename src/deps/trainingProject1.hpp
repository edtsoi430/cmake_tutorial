#include <iostream>
#include <cstdint>
#include <vector>
#include <epoxy/gl.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cmath>
#include "Shape.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Triangle.hpp"

class arenaSimulator
{
public:
private:
	std::vector<Shape*> shapes;
	// Processes key inputs, really just makes escape close the window
	static void process_input(GLFWwindow* window);
	// Called when the window is resized
	static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
	// Helper function used to help with drawing circles
	static void updateTheta(float &thetaVal, float incrementVal);
	// Prepares for rendering and begins render loop
	static void render(std::atomic<bool> *run, arenaSimulator *arenaSim);
};
