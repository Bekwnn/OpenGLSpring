#ifndef OBJECT_CONTROLLER_HPP
#define OBJECT_CONTROLLER_HPP

#pragma once

#include <atlas/math/Vector.hpp>
#include <atlas/utils/Time.hpp>

// Keeps track of velocity and acceleration vectors and sums them for each time step
class ObjectController {
public:
	ObjectController();
	~ObjectController();

	bool bIsStopped;

	atlas::math::Vector updateController(atlas::utils::Time const& t);

	void addMove(const atlas::math::Vector& move);
	void addVelocity(const atlas::math::Vector& vel);
	void addAccel(const atlas::math::Vector& acc);

	void resetMoves();
	void resetAccel();
	void resetVelocity();

	const atlas::math::Vector getMovement() { return deltaMove; }
	const atlas::math::Vector getAccel() { return accel; }
	const atlas::math::Vector getVelocity() { return velocity; }

protected:
	atlas::math::Vector velocity;
	atlas::math::Vector accel;
	atlas::math::Vector deltaMove;
};

#endif
