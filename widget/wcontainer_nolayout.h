#ifndef wcontainer_nolayout_H
#define wcontainer_nolayout_H

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WImage.h>

#include "mana.h"
#include "soma_database.h"

using namespace mana;
using namespace Wt;

class soma;

class wcontainer_nolayout : public WContainerWidget
{
	protected:
		Wt::Signal<bool> signal_on_visible_change;

	public:
		soma *S;
		shared_ptr<soma_database> D;

		// signals
		Wt::Signal<bool>& on_visible_change();

		wcontainer_nolayout();
		void setHidden(bool hidden, const WAnimation& animation = WAnimation());
		~wcontainer_nolayout();
};

#endif // wcontainer_nolayout_H
