#include "PhotoInstruction.h"

using namespace std;
using namespace ci::signals;
using namespace kubik::games;
using namespace kubik;

PhotoInstruction::PhotoInstruction(PhotoboothSettingsRef settings)
{
	reset(settings);		
};

void PhotoInstruction::reset(PhotoboothSettingsRef set)
{
	settings = set;
	fonTex   = settings->getTexture("instrFon");
	titleTex =  settings->getTexture("instrTitle");
	titleTexPos = Vec2f(0.5 * (getWindowWidth() - titleTex.getWidth()), 565);				
}

void PhotoInstruction::start()
{
	console()<<"start Instruction"<<endl;
	connectEventHandler(&PhotoInstruction::nextLocationSignal, this);
}

void PhotoInstruction::stop()
{
	console()<<"stop Instruction"<<endl;
	disconnectEventHandler();
}

void PhotoInstruction::update() 
{

}

void PhotoInstruction::draw() 
{
	gl::draw(fonTex);
	gl::draw(titleTex, titleTexPos);
}	
