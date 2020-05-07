#ifndef wcontainer_H
#define wcontainer_H

#include <Wt/WApplication.h>

#include "mana.h"
#include "broadcast.h"
#include "soma_database.h"
#include "wt/generic/widget_template.h"

class soma;

class wcontainer : public widget_template
{
	protected:
		Wt::Signal<bool> signal_on_visible_change;

	public:
		soma *S;
		shared_ptr<soma_database> D;

		// signals
		Wt::Signal<bool>& on_visible_change();

		wcontainer();
		wcontainer(string filename); // filename located in template/, with or without .html
		void load(string filename); // filename located in template/, with or without .html
		void setHidden(bool hidden, const WAnimation& animation = WAnimation());
};

class wtemplate : public widget_template
{
	public:
		wtemplate(string filename); // filename located in template/, with or without .html
};

#endif // wcontainer_H
