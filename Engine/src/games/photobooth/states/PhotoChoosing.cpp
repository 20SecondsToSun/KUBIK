#include "PhotoChoosing.h"

using namespace std;
using namespace kubik;
using namespace ci::signals;
using namespace ci::app;
using namespace kubik::games;

PhotoChoosing::PhotoChoosing(PhotoboothSettingsRef settings, PhotoStorageRef photoStorage):photoStorage(photoStorage)
{			
	reset(settings);
}		

void PhotoChoosing::start()
{
	//console()<<"start PhotoChoosing"<<endl;			

	//canSelectCount = 0;//settings->getData().photoNum;			
	//nowSelectCount = canSelectCount;			
	//thumbs = photoStorage->getChoosingThumbs();
	//
	//photoBtns.clear();

	//float shift = 0.0f;
	//for (size_t i = 0; i < thumbs.size(); i++)
	//{			
	//	createPhotoButton(i, Vec2f(shift, 200.0f));
	//	shift += thumbs[i].getWidth() + 20;

	//	bool value = (i < (size_t)canSelectCount);
	//	photoBtns[i]->setSelection(value);
	//}	

	//lastSelectedPhotoButton = photoBtns[canSelectCount - 1].get();		
}

void PhotoChoosing::createPhotoButton(int id, Vec2f vec)
{
	//PhotoButtonRef button = PhotoButtonRef(new PhotoButton(id, thumbs[id], vec));	
	//connect_once(button->mouseUpSignal, bind(&PhotoChoosing::mouseUpListener, this, placeholders::_1));
	//photoBtns.push_back(button);
}

void PhotoChoosing::reset(PhotoboothSettingsRef _settings) 
{
	settings = _settings;

	auto title1   = settings->getTextItem(PhtTextID::PRELOAD_TEXT1);
	auto title2   = settings->getTextItem(PhtTextID::PRELOAD_TEXT2);

	tex1 = textTools().getTextField(title1);
	tex2 = textTools().getTextField(title2);

	title1Pos = Vec2f(0.5f * (getWindowWidth() - tex1.getWidth()), 172.0f);
	title2Pos = Vec2f(0.5f * (getWindowWidth() - tex2.getWidth()), 258.0f);
	
	Vec2f position, size;
}	

void PhotoChoosing::update()
{

}

void PhotoChoosing::draw()
{
	gl::draw(tex1, title1Pos);
	gl::draw(tex2, title2Pos);
}

//void mouseUpHandler(Vec2i vec) override
//{
//	for (auto btn : photoBtns)		
//		btn->mouseUpHandler(vec);

//	//nextButton->mouseUpHandler(vec);
//}

/*void mouseUpNextListener(MenuButton& button)
{
storeSelectedItems();

if(photoStorage->isChoosedRightCount(canSelectCount))
nextLocationSignal();
}*/

/*void storeSelectedItems()
{
photoStorage->clearPhotoChosenIds();

for (size_t i = 0; i < photoBtns.size(); i++)			
if(photoBtns[i]->isSelect())
photoStorage->setPhotosChoosenIds(i);			
}*/
