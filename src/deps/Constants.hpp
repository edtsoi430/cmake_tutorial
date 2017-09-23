#ifndef _CONSTANTS_HPP_PROJECT_0_
#define _CONSTANTS_HPP_PROJECT_0_

namespace Constants
{
	constexpr float roombaMass = 5.0f;
	constexpr float vehicleMass = 50.0f;
	constexpr float obstacleMass = 15.0f;
	constexpr float defaultSpeed = 0.01f;
	constexpr float arenaSizeX = 10.0f;
	constexpr float arenaSizeY = 10.0f;
	constexpr float pi = 3.14159265358979f;
	constexpr float white[3] = {0.85f,0.8f,0.65f};
	constexpr float black[3] = {0.0f,0.0f,0.0f};
	constexpr float pillarWhite[3] = {white[0] + 0.3f, white[1] + 0.3f, white[2] + 0.8f};
	constexpr float inPillar[3] = {white[0] - 0.4f, white[1] - 0.4f, white[2] - 0.4f};
	constexpr int numRectsInRing = 360;
	
	// Vehicle bars connecting to ring and prop caps
	constexpr float barColor[3] = {0.6f,0.6f,0.6f};
	constexpr float propColor[3] = {0.3f,0.3f,0.3f};
	constexpr float bodyColor[3] = {0.4f,0.4f,0.4f};
	constexpr float ringColor[3] = {0.4f,0.4f,0.4f};
	constexpr float propCapColor[3] = {0.65f,0.65f,0.65f};
	
	// Prop speed in radians
	constexpr float propSpeed = 0.3;

	// Player identifier colors
	constexpr float playerOneColor[3] = {0.7f,0.7f,0.0f};
	constexpr float playerTwoColor[3] = {0.6f,0.0f,0.7f};
	constexpr float coeffFriction = 0.00005; 
	constexpr bool changeColors = false;
	constexpr float clearColor[3] = {0.5f,0.6f,0.8f};
	constexpr float changeColor[3] = {0.05f,-0.025f,0.025f};
	constexpr float defaultChangeColors[3] = {0.0f,1.0f,0.0f};
}

#endif
