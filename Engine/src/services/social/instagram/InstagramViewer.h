#pragma once

#include "instagram/InstagramClient.h"
#include "ImageSequencer/ImageSequencer.h"
#include "SettingsFactory.h"

namespace instagram
{
	typedef std::shared_ptr<class InstagramViewer> InstagramViewerRef;

	class InstagramViewer
	{
		static const int MAX_PHOTO_CASHED		= 60;
		static const int CASH_PHOTO_CLEAR_COUNT = 18;
		static const int PHOTO_BLOCK_COUNT		= 18;
		
		typedef ci::signals::signal<void(void)> SignalVoid;	

		kubik::IMovieRef preloaderMain, preloaderMini;
		ci::gl::Texture noMaterials;
		ci::gl::Texture allLoaded;
		ci::gl::Texture privateUser;
		ci::gl::Texture dragToReload;
		ci::gl::Texture notExistUser;
		ci::gl::Texture notPhotosUser;
		ci::ColorA preloaderToneColor;

		ci::Anim<float> noMorePopupAlpha, alphaDragToReload;

		size_t showingCount;

		enum drawState
		{	IMAGES_DRAWING,
			PRELOADING, 
			MINI_PRELOADING,
			SHOW_NO_MORE_POPUP,
			SHOW_PRIVATE_USER,
			SHOW_NOT_EXIST_USER,
			SHOW_USER_NOT_HAVE_PHOTOS,
			SHOW_NO_HASHTAG_PHOTOS,
			NO_MATERIALS 
		} 
		state;

		void drawImages();
		void drawMainPreloader();
		void drawMiniPreloader();
		void drawNoMorePopup();
		void drawNoMaterialsPopup();
		void drawNotExistUser();
		void drawUserNotHavePhotos();		
		void drawPrivateUser();	
		void drawCenteredInfoImage(const ci::gl::Texture& image, float y);
		void noMorePopupAnimFinished();
		void setState(const drawState& value);	

	public:	
		InstagramViewer(InstagramClientRef client,			
			const ci::gl::Texture& noMaterials,
			const ci::gl::Texture& allLoaded,
			const ci::gl::Texture& privateUser,	
			const ci::gl::Texture& notExistUser,
			const ci::gl::Texture& notPhotosUser,
			const ci::gl::Texture& dragToReload);

		void setDesignElements(const gl::Texture& noMaterials,
			const ci::gl::Texture& allLoaded,
			const ci::gl::Texture& privateUser,
			const ci::gl::Texture& notExistUser,
			const ci::gl::Texture& notPhotosUser,
			const ci::gl::Texture& dragToReload);

		void setPreloaderToneColor(const ci::ColorA& color);
		void showMiniPreloader();
		void connect();
		void disconnect();
		void showPreloader();		
		void setPosition(float x, float y);
		void animatePositionTo(float x, float y);		
		void synchImages();
		void showNoMoreImagesMsg();
		void showPrivateUserState();
		void showNotExistUser();
		void showUserNotHavePhotos();
		void showNoHashtagPhotos();
		void draw();		
		void mouseDown(ci::app::MouseEvent event);
		void mouseUp(ci::app::MouseEvent event);
		void mouseDrag(ci::app::MouseEvent event);
		void getTouchedImage(const ci::Vec2f& pos);
		void animComplete();
		int getLastImageIndexTouched();
		void clear();
		void setTopDragVisible(bool value);

		ImageGraphic getImageGraphic();
		SignalVoid touchedEvent, reloadAllMedia, loadNextMedia, touchedDownEvent;

	protected:
		InstagramClientRef client;	
		ci::Anim<ci::Vec2f> initPosition;
		ci::Vec2i currentMousePos, delta, futureCurrentPos;
		ci::Anim<ci::Vec2i> currentPos;

		bool blockDrag;
		bool connected;	
		bool wasDrag;
		bool showAlphaDrag;
		bool AllLoaded;

		int mainHeight;
		int oneImageWidth, oneImageHeight;
		int	countInRaw;		
		size_t lastImageIndexTouched;
		int sdvigX, sdvigY;

		float marginBottom, marginTop, animTime, marginToShowUpdate;
		float downSecond, touchDelta;

		ci::EaseFn animFunc;		

		ci::signals::scoped_connection	mouseDownCon, mouseUpCon, mouseDragCon;
		std::vector<ImageGraphic> images;
	};	
}