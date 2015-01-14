#pragma once

#include "IDrawable.h"

namespace kubik
{
	namespace config
	{
		class GamesBlock:public IDrawable
		{
		public:	
			virtual void draw()
			{
				gl::pushMatrices();
				gl::translate(position);
				gl::popMatrices();
			}
		};

		typedef std::shared_ptr<GamesBlock> GamesBlockRef;
	}
}