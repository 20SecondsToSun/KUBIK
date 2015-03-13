#pragma once
#include "shaders/ShaderTool.h"

using namespace shaders::imagefilters;

namespace kubik
{	
	class FinalPhotoTemplate
	{
		ci::gl::Texture photoTemplate, photo;
		float photoTemplateScale;

	public:
		
		void setData(PhotoStorageRef photoStorage)
		{
			/*using namespace shaders::imagefilters;

			auto fID = photoStorage->getSelectedFilter();
			auto shader = shadertool().get((ShaderTool::FilterType)fID);
			auto templates = photoStorage->getPhotoTemplates();
			auto tex = templates[0][FormatID::FORMAT2_BIG];

			gl::Fbo fbo = gl::Fbo(tex.getWidth(), tex.getHeight());

			Utils::drawGraphicsToFBO(fbo, [&]()
			{
				shader->render(tex);
			});

			photo = fbo.getTexture();
			Utils::clearFBO(fbo);	*/
		}

		void setTemplate(const ci::gl::Texture& texture)
		{
			photoTemplate = texture;
			photoTemplateScale = 424.0f / photoTemplate.getWidth();	
		}

		void draw()
		{
			gl::pushMatrices();
			gl::scale(photoTemplateScale, photoTemplateScale);				
			gl::draw(photoTemplate);	
			if(photo)
				gl::draw(photo);
			gl::popMatrices();
		}
	};
}