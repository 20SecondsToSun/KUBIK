#pragma once

class IResourceScreenSaver
{
public:	
	virtual void draw()  = 0;
	virtual void start() = 0;
	virtual void stop()  = 0;
};