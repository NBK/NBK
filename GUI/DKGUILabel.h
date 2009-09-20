#ifndef DK_GUI_LABEL_H
#define DK_GUI_LABEL_H

#include "GUILabel.h"

using namespace GUI;

namespace DK_GUI
{
	class CDKGUILabel: public CGUILabel
	{
	public:
		CDKGUILabel(char *caption);
		~CDKGUILabel();

		virtual GLvoid update();

		/* updates the integer value of the label */
		GLvoid int_update(GLint x=0);

		/* sets the integer value of the label */
		GLvoid int_set(GLint x=0);

	};
}

#endif // DK_GUI_LABEL_H