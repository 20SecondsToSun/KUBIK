#pragma once

#include "TextTools.h"
#include "gui/SimpleSpriteButton.h"
#include "DesignData.h"

namespace kubik
{
	namespace config
	{
		typedef std::shared_ptr<class ImageQuadroButton> ImageQuadroButtonRef;

		class SixButtonsInitObject
		{
			DesignData designdata;
			int userDesignID, activeID;
			std::string path;
			ci::gl::Texture overTexture;
			ci::Color backgroundTextColor;
			float startX, startY;

		public:
			SixButtonsInitObject(const DesignData& designdata,
				int activeID,
				int userDesignID,
				const std::string& path,
				const ci::Color& backgroundTextColor,
				const ci::gl::Texture& overTexture = ci::gl::Texture(),
				float startX = 106,
				float startY = 354)
				:designdata(designdata),
				userDesignID(userDesignID),
				activeID(activeID),
				path(path),
				backgroundTextColor(backgroundTextColor),
				overTexture(overTexture),
				startX(startX),
				startY(startY)
			{

			}

			DesignData getDesignData() const
			{
				return designdata;
			}

			ci::Color getBgTextColor() const
			{
				return backgroundTextColor;
			}

			ci::gl::Texture	getOverTexture() const
			{
				return overTexture;
			}

			int	getActiveID() const
			{
				return activeID;
			}

			int	getUserDesignID() const
			{
				return userDesignID;
			}

			std::string	getPath() const
			{
				return path;
			}

			ci::Vec2f getStartVec() const
			{
				return Vec2f(startX, startY);
			}
		};

		class SixButtonOneData
		{
			OneDesignItem designItem;
			ci::gl::Texture overTexture;
			ci::Color backgroundTextColor;
			ci::Vec2f position;

		public:
			SixButtonOneData(const OneDesignItem& designItem,
				const ci::Color& backgroundTextColor,
				const ci::gl::Texture& overTexture = ci::gl::Texture())
				:designItem(designItem),
				overTexture(overTexture),
				backgroundTextColor(backgroundTextColor)
			{

			}

			void calculateOffset(const ci::Vec2f& startVec, const ci::Vec2f& shiftVec, int index)
			{
				position.x = startVec.x + (designItem.getIcon().getWidth() + shiftVec.x) * (index % 3);
				position.y = startVec.y + (designItem.getIcon().getWidth() + shiftVec.y) * (index / 3);
			}

			OneDesignItem getDesignItem() const
			{
				return designItem;
			}

			ci::Color getBgTextColor() const
			{
				return backgroundTextColor;
			}

			ci::gl::Texture	getOverTexture() const
			{
				return overTexture;
			}

			ci::Vec2f getPosition() const
			{
				return position;
			}
		};

		class ImageQuadroButton: public SimpleSpriteButton
		{
		public:
			ImageQuadroButton(const SixButtonOneData& data);

			virtual void drawLayout() override;
			void setAlpha(float alpha);
			void setSelection(bool value);
			void swapSelection();
			OneDesignItem getItem() const;

		private:
			OneDesignItem item;
			bool selection;
			gl::Texture overtex;			
			ci::Color backgroundTextColor;
			ci::Vec2f textPosition, textBoxSize;
		};	
	}
}