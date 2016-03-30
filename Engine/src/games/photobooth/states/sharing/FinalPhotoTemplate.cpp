#include "FinalPhotoTemplate.h"
using namespace kubik::games::photobooth;

FinalPhotoTemplate::FinalPhotoTemplate()
	:templateWidth(424.0f),
	animTime(0.6f),
	index(0),
	animate(false)
{

}

void FinalPhotoTemplate::setData(PhotoStorageRef photoStorage)
{
	using namespace shaders::imagefilters;
	index = 0;

	auto filterID = photoStorage->getSelectedFilter();
	shader = shadertool().get((ShaderTool::FilterType)filterID);

	templates = photoStorage->getPhotoTemplates();	
	renderTexture();		
}

void FinalPhotoTemplate::renderTexture()
{	
	auto texture = templates[index][FormatID::FORMAT2_BIG];
	
	photo = Utils::drawGraphicsToFBO(texture.getSize(), [&]()
	{
		shader->render(texture);
	});
	
	photoScale = templateWidth / photo.getWidth();	
}

void FinalPhotoTemplate::startAnimate()
{
	animate = true;
}

void FinalPhotoTemplate::stopAnimate()
{
	animate = false;
}

void FinalPhotoTemplate::changePhoto()
{			
	if (++index >= MAX_PHOTOS)
	{
		index = 0;
	}

	renderTexture();	
}

void FinalPhotoTemplate::setTemplate(const ci::gl::Texture& texture)
{
	photoTemplate = texture;
	photoTemplateScale = templateWidth / photoTemplate.getWidth();	
}

void FinalPhotoTemplate::draw()
{
	gl::pushMatrices();
	gl::scale(photoTemplateScale, photoTemplateScale);				
	gl::draw(photoTemplate);				
	gl::popMatrices();

	gl::pushMatrices();
	gl::scale(photoScale, photoScale);			
	gl::draw(photo);
	gl::popMatrices();

	if (getElapsedFrames() % 30 == 0 && animate)
	{
		changePhoto();
	}		
}