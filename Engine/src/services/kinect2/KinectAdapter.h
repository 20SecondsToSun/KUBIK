#pragma once

#include "Types.h"
#include "Kinect2.h"

namespace kubik
{
	class KinectAdapter
	{
	public:
		class BodyFilter
		{
		public:
			ci::Vec3f position;
			int index;
			bool isCentered;
			float distanceZ;
			float distanceToCenter;
			Kinect2::Body body;
			ci::Vec2f getFitPosition(const ci::Vec2f& size, float shiftY);
		};	

		static KinectAdapter& getInstance();

		void create();
		void start();
		void stop();
		void enableHandTracking();
		bool deviceExist();
		Kinect2::DeviceRef getDevice();
		void KinectAdapter::calcCenterBody(const Kinect2::BodyFrame& frame, const ci::Vec2f& depthSize);
		std::vector<KinectAdapter::BodyFilter> getFilterBodies() const;
		BodyFilter getCenterBody() const;

		std::vector<KinectAdapter::BodyFilter> boides;
		
	private:
		std::vector<BodyFilter> filterBodies;
		static Kinect2::DeviceRef mDevice;
		BodyFilter centerBody;
	};

	inline KinectAdapter& kinect() 
	{
		return KinectAdapter::getInstance();
	};
}