#include "PistonSpring.hpp"

#include <stdio.h>

PistonSpring::PistonSpring()
{ }

PistonSpring::PistonSpring(float restingLen, float springK, float damp) :
	restingLength(restingLen),
	springConstant(springK),
	damping(damp)
{ }

PistonSpring::~PistonSpring()
{ }

atlas::math::Vector PistonSpring::GetForce(atlas::math::Vector position, atlas::math::Vector anchorPosition,
		atlas::math::Vector velocity, atlas::math::Vector anchorVelocity)
{
	//TODO: F = -kd -bv
	//where: F is force
	//       k is spring constant
	//       d is delta distance from resting length
	//       b is damping
	//       v is relative velocity of the two bodies

	USING_ATLAS_MATH_NS;
	Vector posDiff = anchorPosition - position; //vec points from position to anchor

	//want to get the velocity magnitude in the direction from our object to the anchor
	//velDiffDot is positive when heading towards anchor
	float velDiffDot = dot((velocity - anchorVelocity), normalize(posDiff));

	// delta distance = (restingLength - posDiff magnitude) * (posDiff unit vec)
	// velocity relative to spring = (velDiffDot) * (posDiff unit vec)
	Vector part1 = (springConstant)*((restingLength - length(posDiff))*normalize(posDiff));
	Vector part2 = damping*(velDiffDot*normalize(posDiff));
	if (velDiffDot != 0.f)
	{
		/*
		printf("delta length: %f\n", restingLength - length(posDiff));
		printf("velocity: (%f, %f, %f)\n", velocity.x, velocity.y, velocity.z);
		printf("anchor velocity : (%f, %f, %f)\n", anchorVelocity.x, anchorVelocity.y, anchorVelocity.z);
		printf("veldiffdot %f\n", velDiffDot);
		printf("part1 (%f, %f, %f)\n", part1.x, part1.y, part1.z);
		printf("part2 (%f, %f, %f)\n", part2.x, part2.y, part2.z);

		printf("!");
		*/
	}
	return -part1 - part2;

}

