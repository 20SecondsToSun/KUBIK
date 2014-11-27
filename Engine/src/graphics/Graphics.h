#pragma once
#include "cinder/app/AppNative.h"
#include <boost/algorithm/string.hpp>
#include "cinder/Json.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class Graphics
{

public:

	enum gamesTexturesID
	{
		PERMANENT_IN_MEMORY_TEXTURES
	};

	static Graphics& getInstance() { static Graphics graph; return graph; };

	void loadTextures(int id)
	{
		try	
		{
		}
		catch(...)
		{
	
		}
		
		completeHandler();

	}

	void addCompleteListener(const std::function<void(void)>& handler)
	{
		completeHandler = handler;
	}

	void addErrorListener(const std::function<void(void)>& handler)
	{
		errorHandler = handler;
	}

	void uloadLastGame()
	{

	}

	

	private:
		 std::function<void(void)> completeHandler;
		 std::function<void(void)> errorHandler;

};

inline Graphics&	graphics() { return Graphics::getInstance(); };