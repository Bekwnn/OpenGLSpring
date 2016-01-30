#ifndef PISTON_SPRING_HPP
#define PISTON_SPRING_HPP

#include <atlas/math/Vector.hpp>

class PistonSpring
{
public:
	PistonSpring();
	PistonSpring(float restingLen, float springK, float damp);
	~PistonSpring();

	float restingLength;
	float springConstant;
	float damping;

	atlas::math::Vector GetForce(atlas::math::Vector position, atlas::math::Vector anchorPosition,
			atlas::math::Vector velocity, atlas::math::Vector anchorVelocity);
};

#endif
