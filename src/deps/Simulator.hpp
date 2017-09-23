/**
* @file Simulator.hpp
*
* @author Ryan Wunderly (rywunder@umich.edu)
* @date 2017-08-22
*/

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>
#include <queue>

#include "Vehicle.hpp"

#include "Roomba.hpp"
#include "Obstacle.hpp"
#include "Constants.hpp"


class Simulator
{
public:
	Simulator();

	//Updates positions of objects, detects collisions, wins game
	//return true when the game ends, member function for if we won or lost
	bool simulate(const unsigned dt);

	// Adds Roomba to roombaList, the function pointer should be nothing by default
	void createRoomba(float xInit, float yInit, float angleInit, float radiusInit,
		Program* shaderProgramIdIn, float *color);

	// Adds Obstacle to obstacleList
	void createObstacle(float xInit, float yInit, float angleInit, float radiusInit,
		Program* shaderProgramIdIn, float *color);

	// Sets the function to call for updating roombas
	void setRoombaUpdateFunc(std::function<void(Roomba&)> func);

	// Sets the function to call for updating obstacles
	void setObstacleUpdateFunc(std::function<void(Obstacle&)> func);

	// Creates and intializes the vehicle on the environment
	void createVehicle(Program* prog);

	// Returns the vector of Roombas
	const std::vector<Roomba>& getRoombaList();

	// Returns the vector of Obstacles
	const std::vector<Obstacle>& getObstacleList();

	// Returns the vector of Vehicles
	const std::vector<Vehicle>& getVehicle();

	// Get score to update text
	int getScore();

	void addAction(std::function <void(Vehicle&)> action);

	void setGreenLinePosition(LinePosition newPos)
	{
		greenLinePosition = newPos;
	}

	void setRedLinePosition(LinePosition newPos)
	{
		redLinePosition = newPos;
	}

private:

	// Checks if two objects are colliding
	bool isCollision(const AnimatedEntity& aEnt1, const AnimatedEntity& aEnt2);

	//Objects collided so what happens to them?
	//Effects updates positions of the animated entities to point at collision
	//http://vobarian.com/collisions/2dcollisions2.pdf
	void physicsCollision(AnimatedEntity& aEnt1, AnimatedEntity& aEnt2, const unsigned dt);

	// Physics for a wall bounce
	// Effects updates yaw of entity to bounce off of the wall
	void physicsBounce(AnimatedEntity& aEnt, const unsigned dt);

	// Checks if a roomba is hitting the wall
	bool isWallCollision(const AnimatedEntity& aEnt);

	void speedDecay(AnimatedEntity& aEnt1, const unsigned dt);

	// Return 0: not in goal, 1: in goal, 2: in incorrect goal
	int roombaInGoal(const AnimatedEntity& roomba);

	// Function keeps roombas stationary
	std::function <void(Roomba&)> updateRoombaLocation;

	// Function defined by the student
	std::function<void(Obstacle&)> updateObstacleLocation;

	// Objects in the environment
	std::vector<Roomba> roombaList;
	std::vector<Obstacle> obstacleList;
	std::vector<Vehicle> vehicles;

	LinePosition redLinePosition;
	LinePosition greenLinePosition;

	int sizeEnvironment = 10; // Default 10X10m
	int score = 0;

	std::queue<std::function <void(Vehicle&)> > actionQueue;
};

#endif
