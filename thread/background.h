#ifndef background_H
#define background_H

#include "mana.h"

using namespace mana;

class soma;
class soma_database;

class background
{
	public:
		bool started = false;

		background();
		void main();
		~background();
};

#endif // background_H
