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
	Vector posDiff = position - anchorPosition; //vec points from position to anchor

	//want to get the velocity magnitude in the direction from our object to the anchor
	//velDiffDot is positive when heading towards anchor
	float velDiffDot = 0.f;
	if (velocity - anchorVelocity != Vector(0.f, 0.f, 0.f))
		velDiffDot = dot((velocity - anchorVelocity), posDiff);

	// delta distance = (restingLength - posDiff magnitude) * (posDiff unit vec)
	// velocity relative to spring = (velDiffDot) * (posDiff unit vec)
	return (springConstant)*((restingLength - length(posDiff))*normalize(posDiff))
		- damping*(velDiffDot*normalize(posDiff));

}

