#include "PhotoStorage.h"

using namespace std;
using namespace kubik;
using namespace kubik::games::photobooth;

PhotoStorage::PhotoStorage()
{
	format2.width	=	1181; 
	format2.height	=	1521;
	format2.top		=	281;

	format1.width	 = 1151;
	format1.height	 = 1026;
	format1.top		 = 360;

	format3.width	=	1181; 
	format3.height	=	1181;
	format3.top		=	281;

	format4.width	=	1270; 
	format4.height	=	2226;
	format4.top		=	0;

	selectedFilterID = 1;
}

void PhotoStorage::clear()
{
	loadedPhotoTexVec.clear();
	photoTemplates.clear();
}

ci::gl::Texture PhotoStorage::loadDownloadedPhoto(const string& path)
{
	gl::Texture loadedTex = gl::Texture(loadImage( ci::loadFile( path ) ));

	//rotate
	gl::Fbo fbo = gl::Fbo(loadedTex.getHeight(), loadedTex.getWidth());
	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		gl::pushMatrices();			
		gl::translate(loadedTex.getHeight(), 0.0f);
		gl::rotate(90);			
		gl::draw(loadedTex);
		gl::popMatrices();
	});

	gl::Texture finalTex = fbo.getTexture();
	Utils::clearFBO(fbo);

	//flip
	gl::Fbo fbo1 = gl::Fbo(loadedTex.getHeight(), loadedTex.getWidth());
	Utils::drawGraphicsToFBO(fbo1, [&]()
	{
		gl::pushMatrices();
		gl::translate(loadedTex.getHeight(), 0.0f);
		gl::scale(-1.0f, 1.0f);					
		gl::draw(finalTex);
		gl::popMatrices();
	});

	PhotoItem item;
	item.mainPhoto =  fbo1.getTexture();	

	Utils::clearFBO(fbo1);

	//item.format2 = createFormatTemplate(format2, item);
	loadedPhotoTexVec.push_back(item);

	return createFormatTemplate(format2, item);
}

void PhotoStorage::setNextPhoto(const ci::gl::Texture& tex)
{
	PhotoItem item;
	item.mainPhoto = tex;
	item.screenshot = true;
	loadedPhotoTexVec.push_back(item);
}

void PhotoStorage::createChoosingPreview()
{
	float prWidth = 272.0f, prHeight = 351.0f;

	for (unsigned int i = 0; i < loadedPhotoTexVec.size(); i++)	
	{
		auto tex = loadedPhotoTexVec[i];
		tex.format2 = createFormatTemplate(format2, tex);

		gl::Fbo fbo1 = gl::Fbo(prWidth, prHeight);
		Utils::drawGraphicsToFBO(fbo1, [&]()
		{
			float scale = prWidth / tex.format2.getWidth();
			gl::pushMatrices();			
			gl::scale(scale, scale);
			gl::draw(tex.format2);
			gl::popMatrices();
		});

		tex.format2Preview = fbo1.getTexture();
		Utils::clearFBO(fbo1);

		loadedPhotoTexVec[i] = tex;
	}
}

ci::gl::Texture PhotoStorage::createFormatTemplate(const Format& format, const PhotoItem& tex)
{
	gl::Fbo fbo = gl::Fbo(format.width, format.height);

	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		gl::pushMatrices();			
		gl::translate(-0.5 * (tex.mainPhoto.getWidth() - format.width), -format.top);		
		gl::draw(tex.mainPhoto);
		gl::popMatrices();
	});

	auto finalTex = fbo.getTexture();
	Utils::clearFBO(fbo);

	return finalTex;
}

std::vector<ci::gl::Texture> PhotoStorage::getChoosingPreview()
{
	std::vector<ci::gl::Texture> texs;

	for (unsigned int i = 0; i < loadedPhotoTexVec.size(); i++)
		texs.push_back(loadedPhotoTexVec[i].format2Preview);	

	return texs;
}

void PhotoStorage::createPhotoTemplates()
{
	for (unsigned int i = 0; i < loadedPhotoTexVec.size(); i++)
	{
		auto tex = loadedPhotoTexVec[i];
		tex.format1 = createFormatTemplate(format1, tex);
		tex.format3 = createFormatTemplate(format3, tex);
		tex.format4 = createFormatTemplate(format4, tex);
		loadedPhotoTexVec[i] = tex;
	}

	photoTemplatesVec.clear();

	for (unsigned int i = 0; i < loadedPhotoTexVec.size(); i++)
	{
		if(!loadedPhotoTexVec[i].selected) continue;

		/////////////////////////////////////////////////////////////////////
		//format 2;
		auto tex	 = loadedPhotoTexVec[i].format2;
		float _width = 303.0f;
		float _scale = _width / tex.getWidth();	
		photoTemplates[FORMAT2_BIG]   = getScaledTex(_width, _scale * format2.height, _scale, tex);

		_width = 72.0f;
		_scale = _width / tex.getWidth();			
		photoTemplates[FORMAT2_SMALL] = getScaledTex(_width, _scale * format2.height, _scale, tex);	

		_width = 157.0f;
		_scale = _width / tex.getWidth();			
		photoTemplates[FORMAT2_MIDDLE] = getScaledTex(_width, _scale * format2.height, _scale, tex);

		/////////////////////////////////////////////////////////////////////	
		//format1
		tex	   = loadedPhotoTexVec[i].format1;
		_width = 218.0f;
		_scale = _width / tex.getWidth();			
		photoTemplates[FORMAT1_SMALL] = getScaledTex(_width, _scale * format1.height, _scale, tex);
		
		/////////////////////////////////////////////////////////////////////
		//format3
		tex	   = loadedPhotoTexVec[i].format3;
		_width = 202.0f;
		_scale = _width / tex.getWidth();	

		photoTemplates[FORMAT3_BIG]     = getScaledTex(_width, _scale * format3.height, _scale, tex);

		_width = 100.0f;
		_scale = _width / tex.getWidth();	
		photoTemplates[FORMAT3_SMALL]   = getScaledTex(_width, _scale * format3.height, _scale, tex);

		/////////////////////////////////////////////////////////////////////
		//format4
		tex	   = loadedPhotoTexVec[i].format4;
		_width = 100.0f;
		_scale = _width / tex.getWidth();	
		photoTemplates[FORMAT4_BIG] = getScaledTex(_width, _scale * format4.height, _scale, tex);		

		photoTemplatesVec.push_back(photoTemplates);
	}
}

gl::Texture PhotoStorage::getScaledTex(int width, int height, float scale, const gl::Texture& tex)
{
	gl::Fbo fbo = gl::Fbo(width, height);

	Utils::drawGraphicsToFBO(fbo, [&]()
	{
		gl::pushMatrices();			
		gl::scale(scale, scale);
		gl::draw(tex);
		gl::popMatrices();
	});

	auto finalTex = fbo.getTexture();
	Utils::clearFBO(fbo);

	return finalTex;
}

vector<PhotoTemplates> PhotoStorage::getPhotoTemplates()
{
	return photoTemplatesVec;
}

void PhotoStorage::setSelectedID(int index)
{
	loadedPhotoTexVec[index].selected = true;
}

void PhotoStorage::setSelectedFilter(int id)
{
	selectedFilterID = id;
}

int PhotoStorage::getSelectedFilter()
{
	return selectedFilterID;
}

void PhotoStorage::setLastScreenShot(const gl::Texture& tex)
{
	lastScreenshot = tex;
}

gl::Texture PhotoStorage::getLastScreenShot()
{
	return lastScreenshot;
}
