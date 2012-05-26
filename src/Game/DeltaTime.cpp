#include "DeltaTime.h"

#define MAX_DELTA	2000

namespace control
{
	CDeltaTime::CDeltaTime()
	{
		dwLastFrame = timeGetTime();

		dwDelta=1;
	}

	CDeltaTime::~CDeltaTime()
	{
	}

	void CDeltaTime::beforeUpdate()
	{
		dwCurrent = timeGetTime();
		dwDelta = dwCurrent - dwLastFrame;

		dwDelta=(dwDelta<=0||dwDelta>MAX_DELTA)?1:dwDelta;
	}

	void CDeltaTime::afterUpdate()
	{
		dwLastFrame = dwCurrent;
	}

	float CDeltaTime::getDelta()
	{
		return (float)dwDelta;
	}
}