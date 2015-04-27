#pragma once

#include "Resources.h"
#include "IScreen.h"
#include "FileTools.h"

namespace kubik
{
	typedef std::shared_ptr<class IMovie> IMovieRef;

	class IMovie
	{
	public:
		enum Type
		{
			VIDEO,
			IMAGES_SEQUENCE
		};

		void setPosition(const ci::Vec2f& vec);
		virtual void draw() = 0;

	protected:
		ci::Vec2f position;
	};
}