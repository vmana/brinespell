#ifndef campaign_H
#define campaign_H

#include "config.h"

class campaign
{
	public:
		string name;

		campaign();
		bool load(string name);
};

#endif
