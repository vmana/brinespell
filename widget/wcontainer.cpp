#include "wcontainer.h"
#include "soma.h"

wcontainer::wcontainer()
{
	this->S = soma::application();
	this->D = S->D;
}

wcontainer::wcontainer(string filename) :
	wcontainer()
{
	if (strpos(filename, ".html") == string::npos) filename += ".html";
	string tpl;
	if (file::read_content(global::template_path + filename, &tpl))
	{
		setTemplateText(tpl, TextFormat::UnsafeXHTML);
	}
	else
	{
		debug_line("filename " + filename + " not found");
	}
}

void wcontainer::setHidden(bool hidden, const WAnimation& animation)
{
	widget_template::setHidden(hidden, animation);
	signal_on_visible_change.emit(hidden);
}

Wt::Signal<bool>& wcontainer::on_visible_change()
{
	return signal_on_visible_change;
}

wcontainer::~wcontainer()
{
}

