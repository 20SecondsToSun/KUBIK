#pragma once
#include "IDispatcher.h"
#include "TextTools.h"
#include "ConfigSettings.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class NotPurchasedGamesBlock> NotPurchasedGamesBlockRef;

		class NotPurchasedGamesBlock: public IDispatcher
		{
		public:
			NotPurchasedGamesBlock(ConfigSettingsRef configSett, vector<GamesInfo> games)
				:titleColor(Color::hex(0x939eb0))
			{

				titleText = configSett->getNotInstallGamesText();
				titleFont = configSett->getFont("introLight36");
			}

			virtual void draw()
			{
				gl::color(Color::white());
				textTools().textFieldDraw(titleText, &titleFont, titleColor, Vec2f(position.x - 8.0f, position.y + 56.0f));
			}

			virtual void setPosition(ci::Vec2i position)		
			{			
				IDrawable::setPosition(position);
			}		

		private:	
			string titleText;
			Color titleColor;
			Font titleFont;
		};
	}
}