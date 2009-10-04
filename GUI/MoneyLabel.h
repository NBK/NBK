#ifndef MONEY_LABEL_H
#define MONEY_LABEL_H

#include "AbstractGUIItem.h"

namespace DK_GUI
{
	class CMoneyLabel: public GUI::CAbstractGUIItem
	{
	public:
		CMoneyLabel();
		~CMoneyLabel();

		virtual GLvoid draw();
		virtual GLvoid set_extent(EXTENT extent);

		GLvoid set_value(GLint amount);
		GLvoid update(GLint amount);

		GLvoid set_check_updated_value(bool check_updated_value);

	private:
		GLuint number_texture[10];
		GLint value;

		char *string_value;
		GLint count;

		bool check_updated_value;
	};
}

#endif // MONEY_LABEL_H