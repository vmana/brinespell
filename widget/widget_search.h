#ifndef widget_search_H
#define widget_search_H

#include "wcontainer.h"
#include <Wt/WLineEdit.h>

class widget_search : public wcontainer
{
	public:

		WLineEdit *search;
		widget_search();
};

#endif // widget_search_H
