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
