#include "widget_search.h"

widget_search::widget_search() : wcontainer("widget_search")
{
	search = bindNew<WLineEdit>("edit_search");
}
