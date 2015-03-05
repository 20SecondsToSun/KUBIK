#pragma once
#include "IPhotoboothLocation.h"
#include "PhotoboothSettings.h"
#include "model/PhotoStorage.h"
#include "TextTools.h"

namespace kubik
{
	namespace games
	{
		typedef	shared_ptr<class PhotoChoosing> PhotoChoosingRef;

		class PhotoChoosing: public IPhotoboothLocation
		{
			ci::gl::Texture tex1, tex2;
			ci::Vec2f title1Pos, title2Pos;

			int canSelectCount;
			int nowSelectCount;

			PhotoStorageRef photoStorage;
			std::vector<Surface> thumbs;

		public:
			PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage);

			void start();
			void draw();
			void createPhotoButton(int id, Vec2f vec);
			void reset(PhotoboothSettingsRef _settings);		
			void update();	
		};	
	}
}