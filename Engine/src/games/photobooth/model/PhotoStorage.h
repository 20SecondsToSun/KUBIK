#pragma once
#include "Utils.h"

namespace kubik
{
	namespace games
	{
		namespace photobooth
		{
			enum FormatID
			{
				FORMAT1_SMALL,
				FORMAT2_SMALL,
				FORMAT2_MIDDLE,
				FORMAT2_BIG,
				FORMAT3_SMALL,
				FORMAT3_BIG,		
				FORMAT4_SMALL,
				FORMAT4_BIG
			};

			class Format
			{
				float width;
				float height;
				float top;

			public:	
				friend class PhotoStorage;
			};

			class PhotoItem
			{
				ci::gl::Texture mainPhoto;
				ci::gl::Texture choosingPreview;

				ci::gl::Texture format1, format2, format3, format4;
				ci::gl::Texture format2Preview;

				bool screenshot;
				bool selected;

			public:
				PhotoItem():screenshot(false), selected(false){};
				friend class PhotoStorage;
			};

			typedef std::map<FormatID, ci::gl::Texture> PhotoTemplates;
			class PhotoStorage
			{
				std::vector<PhotoItem> loadedPhotoTexVec;
				Format format1, format2, format3, format4;
				PhotoTemplates photoTemplates;
				std::vector<PhotoTemplates> photoTemplatesVec;
				int selectedFilterID;
				gl::Texture getScaledTex(int width, int height, float scale, const gl::Texture& tex);

			public:
				PhotoStorage();
				void clear();
				void createChoosingPreview();
				std::vector<ci::gl::Texture> getChoosingPreview();
				void setNextPhoto(const ci::gl::Texture& tex);

				ci::gl::Texture loadDownloadedPhoto(const string& path);

				void createPhotoTemplates();
				vector<PhotoTemplates> getPhotoTemplates();

				void setSelectedID(int i);
				ci::gl::Texture createFormatTemplate(const Format& format, const PhotoItem& tex);

				void setSelectedFilter(int id);
				int getSelectedFilter();
			};

			typedef std::shared_ptr<PhotoStorage> PhotoStorageRef;
		}
	}
}