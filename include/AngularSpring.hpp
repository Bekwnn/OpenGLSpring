#ifndef ANGULAR_SPRING_HPP
#define ANGULAR_SPRING_HPP

#include <atlas/math/Vector.hpp>

class AngularSpring
{
public:
	AngularSpring();
	AngularSpring(atlas::math::Vector restingDir, float springK, float damp);
	~AngularSpring();

	atlas::math::Vector restingDirection;
	float springConstant;
	float damping;

	atlas::math::Vector GetForce(atlas::math::Vector position, atlas::math::Vector anchorPosition,
		atlas::math::Vector velocity);
};

#endif
