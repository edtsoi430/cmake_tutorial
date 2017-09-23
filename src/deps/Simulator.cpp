/**
* @file Simulator.cpp
*
* @author Ryan Wunderly (rywunder@umich.edu)
* @date 2017-08-22
*/
#include "Simulator.hpp"
#include <glm/vec2.hpp>// glm::vec2
#include <glm/geometric.hpp>// glm::dot, glm::length
#include <cassert>
#include <iostream>
#include <algorithm>


using std::vector;
using std::queue;
using std::atan2;
using std::cos;
using std::sin;

using glm::length;
using glm::dot;
using glm::vec2;
using std::cout;

Simulator::Simulator() {  }

//Updates positions of objects, detects collisions, wins game,destroys obj
//Called at 16.7 ms
bool Simulator::simulate(const unsigned dt)
{
	vec2 vehicleInitPos(vehicles[0].getXPos(), vehicles[0].getYPos());

	//Apply Action which updates position of vehicle
	while(!actionQueue.empty())
	{
		actionQueue.front()(vehicles[0]);
		actionQueue.pop();
	}

	//Update position of all objects
	for(auto& obstacle: obstacleList)
	{
		updateObstacleLocation(obstacle);
		speedDecay(obstacle, dt);
	}

	for(auto& roomba: roombaList)
	{
		updateRoombaLocation(roomba);
		speedDecay(roomba, dt);
	}

	// Check for any collisions and update the positions until no overlapping
	// occurs
	bool collisionOccurred = true;

	while(collisionOccurred)
	{
		collisionOccurred = false; //First assume no collsions
		// Comparisons with roombas to check for collisions

		for(auto it = roombaList.begin(), end = roombaList.end();
			it != end; ++it)
		{
			// Compare all roombas to eachother
			for(auto it2 = it + 1; it2 != end; ++it2)
			{
				if(isCollision(*it, *it2))
				{
					collisionOccurred = true;
					physicsCollision(*it, *it2, dt);
				}
			}

			// Compare each roomba to each obstacle
			for(auto itO = obstacleList.begin(), endO = obstacleList.end();
				itO != endO; ++itO)
			{
				if(isCollision(*it, *itO))
				{
					collisionOccurred = true;
					physicsCollision(*it, *itO, dt);
				}
			}

			//std::cout << "For 2 \n";

			//Compare each roomba to the vehicle
			if(isCollision(*it, vehicles[0]))
			{
				cout << "Collided with roomba\n";
				collisionOccurred = true;
				physicsCollision(*it, vehicles[0], dt);
				vehicles[0].setPosition(vehicleInitPos[0], vehicleInitPos[1]);
			}

			
			/*// Check to see if roombas are against a wall
			if(isWallCollision(*it))
			{
				cout << "Collided with Walls\n";
				collisionOccurred = true;
				physicsBounce(*it, dt);
			}*/
		}

		// Comparisons with obstacles
		for(auto it = obstacleList.begin(), end = obstacleList.end();
			it != end; ++it)
		{
			// Compare all obstacles to eachother
			for(auto it2 = it + 1; it2 != end; ++it2)
			{
				if(isCollision(*it, *it2))
				{
					collisionOccurred = true;
					physicsCollision(*it, *it2, dt);
				}
			}

			//Compare each obstacle to the vehicle
			if(isCollision(*it, vehicles[0]))
			{
				collisionOccurred = true;
				//physicsCollision(*it, vehicle);
				vehicles[0].setPosition(vehicleInitPos[0], vehicleInitPos[1]);
				// We have lost the game!
				//cout << "Collsion with an obstacle! You LOSE\n";

				return true; // return that the game ended
			}
		}
	}

	//cout << "Stopped checking collisions\n";

	//check game logic (should we score any points?)-->destroy some roombas
	//Check did any roombas pass the goal line?
	for(size_t i = 0; i < roombaList.size(); ++i)
	{
		//int: 1 if correct goal 2 if wrong goal, 0 if not in any goal
		int inGoal = roombaInGoal(roombaList[i]);
		if(inGoal)
		{
			//swap and pop the roomba to destroy it.
			std::iter_swap(roombaList.begin() + i, roombaList.end() - 1);
			roombaList.pop_back(); //remove the roomba from the list

			//Update player score
			if(inGoal == 1)
			{
				score += 69;
			}
			else // The player put it in wrong goal
			{
				score -= 42;
			}
		}
	}

	//The game has ended
	if(roombaList.empty())
	{
		cout << "All roombas have been scored!\n";
		return true; // return that the game ended
	}

	for(auto& v : vehicles) {
		v.update();
	}

	for(auto& r : roombaList) {
		r.update();
	}

	for(auto& o : obstacleList) {
		o.update();
	}

	return false; //The game should continue

}

// Adds Roomba to roombaList
void Simulator::createRoomba(float xInit, float yInit, float angleInit,
	float radiusInit, Program* shaderProgramIdIn, float *color)
{
	roombaList.emplace_back(xInit, yInit, angleInit, radiusInit, color,
		shaderProgramIdIn);
}

// Adds Obstacle to obstacleList
void Simulator::createObstacle(float xInit, float yInit, float angleInit,
	float radiusInit, Program* shaderProgramIdIn, float *color)
{
	obstacleList.emplace_back(xInit, yInit, angleInit, radiusInit, color,
	 shaderProgramIdIn);
}

void Simulator::setRoombaUpdateFunc(std::function<void(Roomba&)> func)
{
	updateRoombaLocation = func;
}

void Simulator::setObstacleUpdateFunc(std::function<void(Obstacle&)> func)
{
	updateObstacleLocation = func;
}

void Simulator::createVehicle(Program* prog)
{
	if (vehicles.size() == 0)
		vehicles.emplace_back(0.f, 0.f, PI / 4.f, 1.f, Constants::playerOneColor, prog);
	else if (vehicles.size() == 1)
		vehicles.emplace_back(0.f, 0.f, PI / 4.f, 1.f, Constants::playerTwoColor, prog);
	else
		std::cerr << "Why are you making more than two vehicles????\n";
}

const std::vector<Roomba>& Simulator::getRoombaList()
{
	return roombaList;
}

const std::vector<Obstacle>& Simulator::getObstacleList()
{
	return obstacleList;
}

const std::vector<Vehicle>& Simulator::getVehicle()
{
	return vehicles;
}

// Get score to update text
int Simulator::getScore()
{
	return score;
}

void Simulator::addAction(std::function <void(Vehicle&)> action_in)
{
		actionQueue.push(action_in);
}

// Checks if two objects are colliding
bool Simulator::isCollision(const AnimatedEntity& aEnt1,
	const AnimatedEntity& aEnt2)
{
	// Get the positions of both objects
	vec2 aEnt1Pos(aEnt1.getXPos(), aEnt1.getYPos());
	vec2 aEnt2Pos(aEnt2.getXPos(), aEnt2.getYPos());

	//Get the distance between the objects
	double dist = length(aEnt1Pos - aEnt2Pos);

	//Check to see if radius between entities
	if(dist < (aEnt1.getRadius() +  aEnt2.getRadius()))
	{
		return true;
	}

	return false;
}

// Implement physics for a collision between entities
// Effects updates positions, yaw, and speed of the entities in collision
void Simulator::physicsCollision(AnimatedEntity& aEnt1, AnimatedEntity& aEnt2,
	const unsigned dt)
{
	int mass1 = aEnt1.getMass(), mass2 = aEnt2.getMass();

	// Get the positions of both objects
	vec2 aEnt1Pos(aEnt1.getXPos(), aEnt1.getYPos());
	vec2 aEnt2Pos(aEnt2.getXPos(), aEnt2.getYPos());

	// Unit normal vector and unit tangent vector to the objects
	vec2 unormal = (aEnt1Pos - aEnt2Pos) / length((aEnt1Pos - aEnt2Pos));
	vec2 utangent(-1 * unormal[1], unormal[0]);

	float aEnt1Yaw = -1 * atan2(unormal[1], unormal[0]);
	float aEnt2Yaw = atan2(unormal[1], unormal[0]);

	// Initial velocity vectors
	vec2 vi1(aEnt1.getSpeed()*cos(aEnt1Yaw),
		aEnt1.getSpeed()*sin(aEnt1Yaw));

	vec2 vi2(aEnt2.getSpeed()*cos(aEnt2Yaw),
		aEnt2.getSpeed()*sin(aEnt2Yaw));

	// Normal and tangential scalar velocities
	float s1norm = dot(unormal, vi1);
	float s1tan  = dot(utangent, vi1);
	float s2norm = dot(unormal, vi2);
	float s2tan  = dot(utangent, vi2);

	// Note: The tangential velocities are not affected by collision
	// Calculate the normal scalar velocities after the collsion
	float s1fnorm = (s1norm * (mass1 - mass2) + (2 * mass2 * s2norm)) /
		(mass1 + mass2);
	float s2fnorm = (s2norm * (mass2 - mass1) + (2 * mass1 * s1norm)) /
		(mass1 + mass2);

	// Convert the scalar velocities into vectors
	vec2 vf1norm = s1fnorm * unormal;
	vec2 vf1tan = s1tan * utangent;
	vec2 vf2norm = s2fnorm * unormal;
	vec2 vf2tan = s2tan * utangent;

	// Find final velocity vectors for each object
	vec2 v1final = vf1norm + vf1tan;
	vec2 v2final = vf2norm + vf2tan;

	// Set new positions of both objects to right before collision (overlap)
	vec2 aEnt1FPos = aEnt1Pos;
	vec2 aEnt2FPos = aEnt2Pos;
	float timestep = dt / 10.0;

	// Go back in time until there is no collision between objects
	while(isCollision(aEnt1, aEnt2))
	{
		aEnt1FPos = .001f * v1final * timestep + aEnt1FPos;
		//aEnt2FPos = -0.1f * vi2 * timestep + aEnt2FPos;

		//Set the new positions then check for the collision
		aEnt1.setPosition(aEnt1FPos[0], aEnt1FPos[1]);
		aEnt2.setPosition(aEnt2FPos[0], aEnt2FPos[1]);
	}

	assert(!isCollision(aEnt1, aEnt2)); // they aren't colliding

	// Set the new speed and yaw of the objects
	aEnt1.setSpeed(length(v1final));
	aEnt1.setYawPhysics(atan2(v1final[1], v1final[0]));
	aEnt2.setSpeed(length(v2final));
	aEnt2.setYawPhysics(atan2(v2final[1], v2final[0]));
}

// Return 0: not in goal, 1: in goal, 2: in incorrect goal
int Simulator::roombaInGoal(const AnimatedEntity& roomba)
{
	switch(greenLinePosition)
	{
		case LinePosition::top :
			if(roomba.getYPos() > sizeEnvironment)
				return 1;
			break;
		case LinePosition::bottom :
			if(roomba.getYPos() < 0)
				return 1;
			break;
		case LinePosition::left :
			if(roomba.getXPos() < 0)
				return 1;
			break;
		case LinePosition::right :
			if(roomba.getXPos() > sizeEnvironment)
				return 1;
			break;
	}

	switch(redLinePosition)
	{
		case LinePosition::top :
			if(roomba.getYPos() > sizeEnvironment)
				return 2;
			break;
		case LinePosition::bottom :
			if(roomba.getYPos() < 0)
				return 2;
			break;
		case LinePosition::left :
			if(roomba.getXPos() < 0)
				return 2;
			break;
		case LinePosition::right :
			if(roomba.getXPos() > sizeEnvironment)
				return 2;
			break;
	}

	return 0;
}

void Simulator::speedDecay(AnimatedEntity& aEnt, const unsigned dt)
{
	aEnt.setSpeed(std::max(aEnt.getSpeed() - (aEnt.getForceFriction() * dt), 0.f));
}

void Simulator::physicsBounce(AnimatedEntity& aEnt, const unsigned dt)
{

	// Which wall did we bounce off of?
	float x  = aEnt.getXPos(), y  = aEnt.getYPos(), r = aEnt.getRadius();

	vec2 vinitial(aEnt.getSpeed()*cos(aEnt.getYawPhysics()),
		aEnt.getSpeed()*sin(aEnt.getYawPhysics()));

	vec2 vfinal;

	if(x >= Constants::arenaSizeX - r || x <= r) // Right or Left wall
	{
		vec2 normalV = normalize(vec2(0, 10));
		vfinal = reflect(vinitial, normalV);
	}
	else // top or bottom wall
	{
		vec2 normalV = normalize(vec2(10, 0));
		vfinal = reflect(vinitial, normalV);
	}


	// Set the new speed and yaw of the objects
	aEnt.setSpeed(length(vfinal));
	aEnt.setYawPhysics(atan2(vfinal[1], vfinal[0]));

	float timestep = dt / 10.0;
	vec2 aEntFPos(x, y); 

	while(isWallCollision(aEnt))
	{
		aEntFPos = .001f * vfinal * timestep + aEntFPos;
	}
	
	aEnt.setPosition(aEntFPos[0], aEntFPos[1]);

}

bool Simulator::isWallCollision(const AnimatedEntity& aEnt)
{
	float x  = aEnt.getXPos(), y  = aEnt.getYPos(), r = aEnt.getRadius();
	if(roombaInGoal(aEnt) == 0)
	{
		if((y > Constants::arenaSizeY - r) || y < r){
			return true;
		}
		else if((x > Constants::arenaSizeX - r) || x < r){
			return true;
		}
	}

	return false;
}
