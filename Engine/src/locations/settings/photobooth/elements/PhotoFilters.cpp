#include "PhotoFilters.h"

using namespace std;
using namespace ci;
using namespace kubik;
using namespace kubik::config;

PhotoFilters::PhotoFilters(PhotoboothSettingsRef phbSettings, const ci::Color& color, int index)
	:IPhotoboothItem(phbSettings, PhtTextID::FILTERS, color, index)
{
	DesignData designdata = settings->getPhotoFiltersPreview();				
	auto over6 = phbSettings->getTexture("over6");

	int i = 0;
	Vec2f shiftVec(53, 130);
	Vec2f startVec(106, 354);		

	for (auto it : designdata)
	{
		SixButtonOneData data(it, color, over6);
		data.calculateOffset(startVec, shiftVec, i++);
		ImageQuadroButtonRef imageQuadroButton = settingsFactory().createPhotoFilterPreviewButton(data);

		btns[it.getID()] = imageQuadroButton;
		addChild(imageQuadroButton);					
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