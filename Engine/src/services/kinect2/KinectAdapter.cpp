#include "kinect2/KinectAdapter.h"

using namespace kubik;
using namespace std;
using namespace ci;

Kinect2::DeviceRef KinectAdapter::mDevice;

KinectAdapter& KinectAdapter::getInstance()
{
	static KinectAdapter kinect;
	return kinect;
}

void KinectAdapter::create()
{
	if (!mDevice)
	{
		mDevice = Kinect2::Device::create();
	}
}

void KinectAdapter::enableHandTracking()
{
	if (mDevice)
	{
		mDevice->enableHandTracking();
	}
}

void KinectAdapter::start()
{
	if (mDevice)
	{
		mDevice->start();
	}
}

void KinectAdapter::stop()
{
	if (mDevice)
	{
		mDevice->stop();
	}
}

bool KinectAdapter::deviceExist()
{
	return mDevice != nullptr;
}

Kinect2::DeviceRef KinectAdapter::getDevice()
{
	return mDevice;
}

void KinectAdapter::calcCenterBody(const Kinect2::BodyFrame& mBodyFrame, const ci::Vec2f& depthSize)
{
	auto bodies = mBodyFrame.getBodies();
	int bodyIndex = 0;
	float closestDistanceTocenter = 100000;

	filterBodies.clear();

	for (const Kinect2::Body& body : bodies)
	{
		if (body.isTracked())
		{
			auto jMap  = body.getJointMap();
			auto spine = jMap.at(JointType::JointType_SpineBase);
			auto state = spine.getTrackingState();

			if (state == TrackingState::TrackingState_Tracked)
			{
				auto position = Vec2f(kinect().getDevice()->mapCameraToDepth(spine.getPosition()));

				BodyFilter filter;
				filter.position			= Vec3f(position.x, position.y, spine.getPosition().z);
				filter.index			= bodyIndex;
				filter.isCentered		= false;
				filter.distanceZ		= float(spine.getPosition().z * 100) / 100;
				filter.distanceToCenter = position.distance(depthSize * 0.5);
				filter.body				= body;				

				if (filter.distanceToCenter < closestDistanceTocenter)
				{
					for (size_t i = 0; i < filterBodies.size(); i++)
					{
						filterBodies[i].isCentered = false;
					}
					filter.isCentered = true;

					centerBody = filter;
					closestDistanceTocenter = filter.distanceToCenter;
				}

				filterBodies.push_back(filter);
			}
		}
	}	
}

std::vector<KinectAdapter::BodyFilter> KinectAdapter::getFilterBodies() const
{
	return filterBodies;
}

KinectAdapter::BodyFilter KinectAdapter::getCenterBody() const
{
	return centerBody;
}

ci::Vec2f KinectAdapter::BodyFilter::getFitPosition(const ci::Vec2f& size, float shiftY)
{
	auto scale = 1920.0f / size.y;
	auto shift = Vec2f(0.5f * (1080.0f - scale * size.x), shiftY);
	return (Vec2f(position.x, position.y) * scale + shift);
}
