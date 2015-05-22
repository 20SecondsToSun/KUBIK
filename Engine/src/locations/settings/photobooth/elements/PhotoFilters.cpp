#include "PhotoFilters.h"

using namespace kubik;
using namespace kubik::config;

PhotoFilters::PhotoFilters(PhotoboothSettingsRef phbSettings, const ci::Color& color, int index)
	:IPhotoboothItem(phbSettings, PhtTextID::FILTERS, color, index)
{
	DesignData designdata = settings->getPhotoFiltersPreview();				

	ci::Vec2f pos = ci::Vec2f::zero();
	int i = 0;
	float shiftX = 53, shiftY = 130;
	float startX = 106, startY= 354;
	auto over6 = phbSettings->getTexture("over6");

	for (auto it : designdata)
	{
		pos.x = startX + (it.getIcon().getWidth() + shiftX) * (i % 3);
		pos.y = startY + (it.getIcon().getWidth() + shiftY) * (i / 3);	

		SixButtonOneData data(it, color, over6);
		ImageQuadroButtonRef imageQuadroButton = settingsFactory().createPhotoFilterPreviewButton(data);

		btns[it.getID()] = imageQuadroButton;
		addChild(imageQuadroButton);	
		i++;					
	}

	auto filters = settings->getFilters();	

	for (auto item : filters)
		if(item.isActive())
			btns[item.getID()]->setSelection(true);				
}

void PhotoFilters::activateListeners()
{
	for (auto it : btns)
		it.second->connectEventHandler(&PhotoFilters::buttonClicked, this);

	IPhotoboothItem::activateListeners();
}

void PhotoFilters::unActivateListeners()
{
	for (auto it : btns)
		it.second->disconnectEventHandler();	

	IPhotoboothItem::unActivateListeners();
}

void PhotoFilters::buttonClicked(EventGUIRef& event)
{
	EventGUI *ev = event.get();

	if(typeid(*ev) == typeid(ChangePhotoFilterPreviewActiveEvent))
	{			
		int id = static_pointer_cast<ChangePhotoFilterPreviewActiveEvent>(event)->getItem().getID();
		btns[id]->swapSelection();					
	}
}	
