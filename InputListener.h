#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

/*
	An abstract class used to register input events.
*/

namespace control
{
	class CInputListener
	{
	public:
		CInputListener();
		~CInputListener();

		virtual void onKeyDown(int key)=0;
		virtual void onKeyUp(int key)=0;
		virtual void onMouseClicked(int button)=0;
	};
};

#endif // INPUT_LISTENER_H