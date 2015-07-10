#pragma once

#include "Types.h"
#include "Kinect2.h"

namespace kubik
{
	class KinectAdapter
	{
	public:
		static KinectAdapter& getInstance();

		void create();
		void start();
		void stop();
		void enableHandTracking();
		bool deviceExist();
		Kinect2::DeviceRef getDevice();
		
	private:
		static Kinect2::DeviceRef	mDevice;
	};

	inline KinectAdapter&	kinect() { return KinectAdapter::getInstance();};
}