#include "Location.h"

bool Location::isComeBackTimerTouchFired()
{
	//if (!returnTimer.isStopped() && (returnTimer.getSeconds() >= Params::comeBackHomeTime))
	//{
	//	returnTimer.stop();
	//	return true;	
	//}
	///*else if(returnTimer.isStopped())
	//{		
	//	returnTimer.start();		
	//}*/

	return false;
}

bool Location::isComeBackTimerKinectFired()
{	
	//if (!returnTimer.isStopped())
	//{
	//	if (isPeopleInFrame == true)
	//	{
	//		returnTimer.stop();
	//	}
	//	else if(returnTimer.getSeconds() >= Params::comeBackHomeTime)
	//	{
	//		returnTimer.stop();			
	//		return true;
	//	}
	//}
	//else if (isPeopleInFrame == false)
	//{
	//	if (returnTimer.isStopped())
	//		returnTimer.start();
	//}

	return false;
}

int Location::getSecondsToComeBack()
{
	return 10;//(int)(Params::comeBackHomeTime - returnTimer.getSeconds());
}

void Location::comeBackTimerStart()
{
	returnTimer.start();
}

void Location::comeBackTimerStop()
{
	if(!returnTimer.isStopped())
		returnTimer.stop();
}