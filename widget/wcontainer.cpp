#include "wcontainer.h"
#include "soma.h"

wtemplate::wtemplate()
{
}

wtemplate::wtemplate(string filename)
{
	load(filename);
}

wtemplate wtemplate::operator+=(std::unique_ptr<wtemplate> &T)
{
	string new_id = mana::randstring(20);
	this->setTemplateText(this->templateText() + "${" + new_id + "}");
	this->bindWidget(new_id, move(T));
	return *this;
}

void wtemplate::load(string filename)
{
	if (strpos(filename, ".html") == string::npos) filename += ".html";
	string tpl;
	if (file::read_content(global::template_path + filename, &tpl))
	{
		set_text(tpl);
	}
	else
	{
		debug_line("filename " + filename + " not found");
	}
}

wcontainer::wcontainer()
{
	this->S = soma::application();
}

wcontainer::wcontainer(string filename) : wcontainer()
{
	load(filename);
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
