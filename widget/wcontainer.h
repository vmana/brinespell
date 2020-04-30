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
		void setHidden(bool hidden, const WAnimation& animation = WAnimation());
		~wcontainer();
};

#endif // wcontainer_H
