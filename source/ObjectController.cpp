#include "ObjectController.hpp"

ObjectController::ObjectController()
{
	bIsStopped = false;

	accel = atlas::math::Vector(0.f, 0.f, 0.f);
	velocity = atlas::math::Vector(0.f, 0.f, 0.f);
	deltaMove = atlas::math::Vector(0.f, 0.f, 0.f);
}

ObjectController::~ObjectController()
{

}

atlas::math::Vector ObjectController::updateController(atlas::utils::Time const& t)
{
	velocity = velocity + accel * t.deltaTime;
	atlas::math::Vector tempDeltaMove = deltaMove; // deltaMove ignores deltaTime
	resetMoves();
	return velocity * t.deltaTime + tempDeltaMove;
}

void ObjectController::addMove(const atlas::math::Vector& move)
{
	deltaMove = deltaMove + move;
}

void ObjectController::addVelocity(const atlas::math::Vector& vel)
{
	velocity = velocity + vel;
}

void ObjectController::addAccel(const atlas::math::Vector& acc)
{
	accel = accel + acc;
}

void ObjectController::resetMoves()
{
	deltaMove = atlas::math::Vector(0.f, 0.f, 0.f);
}

void ObjectController::resetVelocity()
{
	velocity = atlas::math::Vector(0.f, 0.f, 0.f);
}

void ObjectController::resetAccel()
{
	accel = atlas::math::Vector(0.f, 0.f, 0.f);
}
