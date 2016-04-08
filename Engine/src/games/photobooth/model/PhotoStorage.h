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
				FORMAT1_PRINT,
				FORMAT2_SMALL,
				FORMAT2_MIDDLE,
				FORMAT2_BIG,
				FORMAT2_PRINT,
				FORMAT3_SMALL,
				FORMAT3_BIG,
				FORMAT3_PRINT_1,
				FORMAT3_PRINT_2,
				FORMAT4_SMALL,
				FORMAT4_BIG,
				FORMAT4_PRINT,
				FORMAT5_PRINT_1,
				FORMAT5_PRINT_2,
				FORMAT5_PRINT_3
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
				ci::gl::Texture getScaledTex(int width, int height, float scale, const ci::gl::Texture& tex);

				ci::gl::Texture lastScreenshot;

			public:
				PhotoStorage();
				~PhotoStorage();
				void clear();
				bool empty() const;
				void createChoosingPreview();
				std::vector<ci::gl::Texture> getChoosingPreview();
				void setNextPhoto(const ci::gl::Texture& tex);

				ci::gl::Texture loadDownloadedPhoto(const std::string& path);

				void createPhotoTemplates();
				ci::gl::Texture createFormat2Template(const ci::gl::Texture& tex);

				std::vector<PhotoTemplates> getPhotoTemplates();

				void setSelectedID(int i);
				ci::gl::Texture createFormatTemplate(const Format& format, const PhotoItem& tex);

				void setSelectedFilter(int id);
				int getSelectedFilter() const;

				void setLastScreenShot(const ci::gl::Texture& tex);
				ci::gl::Texture getLastScreenShot() const;
			};

			typedef std::shared_ptr<PhotoStorage> PhotoStorageRef;
		}
	}
}