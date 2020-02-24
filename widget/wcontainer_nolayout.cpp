#include "wcontainer_nolayout.h"
#include "soma.h"

wcontainer_nolayout::wcontainer_nolayout()
{
	this->S = soma::application();
	this->D = S->D;
	this->setStyleClass("container-background");
}

void wcontainer_nolayout::setHidden(bool hidden, const WAnimation& animation)
{
	WContainerWidget::setHidden(hidden, animation);
	signal_on_visible_change.emit(hidden);
}

Wt::Signal<bool>& wcontainer_nolayout::on_visible_change()
{
	return signal_on_visible_change;
}

wcontainer_nolayout::~wcontainer_nolayout()
{
}

