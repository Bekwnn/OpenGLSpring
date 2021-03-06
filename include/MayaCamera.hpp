#ifndef LAB02_INCLUDE_MAYA_CAMERA_HPP
#define LAB02_INCLUDE_MAYA_CAMERA_HPP

#pragma once

#include <atlas/utils/Camera.hpp>
#include <atlas/utils/Time.hpp>
#include <memory>
#include <vector>

class MayaCamera : public atlas::utils::Camera
{
public:
    MayaCamera();
    ~MayaCamera();

	void updateCameraRail(atlas::utils::Time const & t);
    void mouseDown(atlas::math::Point2 const& point,
        CameraMovements movement) override;
    void mouseDrag(atlas::math::Point2 const& point) override;
    void mouseUp() override;
    void resetCamera() override;
    atlas::math::Matrix4 getCameraMatrix() override;
	atlas::math::Vector getHermitePos(int indexStartAt, float t, float tangentSize);

	std::vector<atlas::math::Vector> waypoints;
	std::vector<float> transitionDurations;

	bool bIsPlaying;

private:
    struct MayaCameraImpl;
    std::unique_ptr<MayaCameraImpl> mImpl;
	int waypointIndex;
	float curCurveTime;
};

#endif


