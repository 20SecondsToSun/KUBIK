#pragma once
#include "instagram/InstagramClient.h"
#include "ImageSequencer/ImageSequencer.h"

namespace instagram
{
	typedef std::shared_ptr<class InstagramViewer> InstagramViewerRef;

	class InstagramViewer
	{
		static const int MAX_PHOTO_CASHED = 100;
		static const int CASH_PHOTO_CLEAR_COUNT = 20;

		typedef ci::signals::signal<void(void)> SignalVoid;	

		kubik::ImageSequencerRef preloaderMain, preloaderMini;
		ci::gl::Texture noMaterials;
		ci::gl::Texture allLoaded;

		enum drawState{ IMAGES_DRAWING,
			PRELOADING, 
			MINI_PRELOADING,
			SHOW_NO_MORE_POPUP,
			NO_MATERIALS } state;

		void drawImages();
		void drawMainPreloader();
		void drawMiniPreloader();
		void drawNoMorePopup();
		void drawNoMaterialsPopup();		
		void noMorePopupAnimFinished();	
		void setState(const drawState& value);

		ci::Anim<float> noMorePopupAlpha;

	public:	
		InstagramViewer(InstagramClientRef client,
			kubik::ImageSequencerRef prel,
			kubik::ImageSequencerRef preloaderMini,
			const ci::gl::Texture& noMaterials,
			const ci::gl::Texture& allLoaded);

		void showMiniPreloader();
		void connect();
		void disconnect();
		void showPreloader();		
		void setPosition(float x, float y);

		void synchImages();
		void showNoMoreImagesMsg();
		void draw();
		void mouseDown(ci::app::MouseEvent event);
		void mouseUp(ci::app::MouseEvent event);
		void mouseDrag(ci::app::MouseEvent event);
		void getTouchedImage(const ci::Vec2f& pos);
		void animComplete();
		int getLastImageIndexTouched();
		void clear();
		ImageGraphic getImageGraphic();

		SignalVoid touchedEvent, reloadAllMedia, loadNextMedia;

	protected:
		InstagramClientRef client;	
		ci::Vec2f initPosition;
		ci::Vec2i currentMousePos, delta, futureCurrentPos;
		ci::Anim<ci::Vec2i> currentPos;

		bool blockDrag;
		bool connected;	
		bool wasDrag;

		int mainHeight;
		int oneImageWidth, oneImageHeight;
		int	countInRaw;		
		size_t lastImageIndexTouched;
		int sdvigX, sdvigY;

		float marginBottom, marginTop, animTime;
		float downSecond, touchDelta;

		ci::EaseFn animFunc;		

		ci::signals::scoped_connection	mouseDownCon, mouseUpCon, mouseDragCon;
		std::vector<ImageGraphic> images;
	};	
}