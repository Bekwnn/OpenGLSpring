#include "AngularSpring.hpp"

#include <stdio.h>

AngularSpring::AngularSpring()
{ }

AngularSpring::AngularSpring(atlas::math::Vector restingDir, float springK, float damp) :
	restingDirection(restingDir),
	springConstant(springK),
	damping(damp)
{ }

AngularSpring::~AngularSpring()
{ }

atlas::math::Vector AngularSpring::GetForce(atlas::math::Vector position, atlas::math::Vector anchorPosition,
	atlas::math::Vector velocity)
{
	//TODO: Torque = k(theta(t)-thetad) - v(theta'(t)-theta'd)
	USING_ATLAS_MATH_NS;
	//we want to get the vector normal to the plane represented by the resting pos and our object's pos
	Vector orthoPlaneNorm;
	if (abs(dot(restingDirection, normalize(anchorPosition - position))) != 1.f)
		orthoPlaneNorm = cross(restingDirection, position - anchorPosition);
	else return Vector(0.f, 0.f, 0.f); //at resting position, apply no force

	//cross product that with our relative position to get a velocity direction
	//orthogonal to our relative position
	Vector velDirection = -normalize(cross(orthoPlaneNorm, position - anchorPosition));

	//multiply velDirection with torque magnitude to get force vector
	Vector normRestingDir = normalize(restingDirection);
	Vector normRelativePos = normalize(position - anchorPosition);

	//testing whether finalForce should be reversed ( the above cross product stuff causes inconsistent direction )
	Vector finalForce = velDirection *
		(springConstant * cosf(dot(normRelativePos, normRestingDir)) -
			damping*dot(normalize(velocity), velDirection)*length(velocity) * cosf(dot(normRelativePos, normRestingDir)));
	if (dot(normalize(finalForce), normRestingDir) < 0.f) finalForce = -finalForce;

	return finalForce;
}

