#include "FinalPhotoTemplate.h"
using namespace kubik::games::photobooth;

FinalPhotoTemplate::FinalPhotoTemplate()
	:templateWidth(424.0f),
	animTime(0.6f)
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
	delaycall(bind(&FinalPhotoTemplate::changePhoto, this), animTime);
}

void FinalPhotoTemplate::stopAnimate()
{
	clearDelaycall();
}

void FinalPhotoTemplate::changePhoto()
{			
	if (++index >= MAX_PHOTOS) index = 0;
	renderTexture();	
	startAnimate();
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
}