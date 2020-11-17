#ifndef wcontainer_H
#define wcontainer_H

#include <Wt/WApplication.h>

#include "soma_database.h"
#include "wt/generic/widget_template.h"
#include "broadcast.h"

class soma;

class wtemplate : public widget_template
{
	public:
		wtemplate();
		wtemplate(string filename); // filename located in template/, with or without .html
		void load(string filename); // filename located in template/, with or without .html
		wtemplate operator+=(std::unique_ptr<wtemplate> &T);
};

class wcontainer : public wtemplate
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
		void setHidden(bool hidden, const WAnimation& animation = WAnimation());
};

#endif // wcontainer_H
