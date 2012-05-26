#ifndef DELTA_TIME_H
#define DELTA_TIME_H

#include "system.h"

namespace control
{
	class CDeltaTime
	{
	public:
		CDeltaTime();
		~CDeltaTime();

		void	beforeUpdate();
		void	afterUpdate();

		float	getDelta();

	private:
		DWORD	dwLastFrame,
				dwCurrent,
				dwDelta;

	};
};

#endif // DELTA_TIME_H
