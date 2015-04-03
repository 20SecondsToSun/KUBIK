#pragma once
#include "instagram/InstagramClient.h"

namespace instagram
{
	typedef std::shared_ptr<class InstagramViewer> InstagramViewerRef;

	class InstagramViewer
	{
		typedef ci::signals::signal<void(void)> SignalVoid;	

	public:	
		InstagramViewer(InstagramClientRef client);

		void connect();
		void disconnect();
		void setPosition(float x, float y);

		void update();
		void draw();
		void mouseDown(ci::app::MouseEvent event);
		void mouseUp(ci::app::MouseEvent event);
		void mouseDrag(ci::app::MouseEvent event);
		void getTouchedImage(const ci::Vec2f& pos);
		void animComplete();
		int getLastImageIndexTouched();
		void clear();

		SignalVoid touchedEvent;

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

		ci::signals::scoped_connection	mouseDownCon, mouseUpCon, mouseDragCon, updateCon;	
	};	
}