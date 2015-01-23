#pragma once

#include "IDrawable.h"
#include "ToolRoundBtn.h"
#include "settings/SavePhotobootnConfigEvent.h"

namespace kubik
{
	namespace config
	{
		class SaveBtn : public ToolRoundBtn
		{
		public:	
			SaveBtn(Rectf rect, string text, Font font)
				:ToolRoundBtn(rect, text, font, Color::hex(0xffff00))
			{
				event = SavePhotobootnConfigEventRef(new SavePhotobootnConfigEvent());
			}
		};
		typedef std::shared_ptr<SaveBtn> SaveBtnRef;
	}
}