#include "background.h"
#include "soma.h"

background::background()
{
}

// thread to perform tasks periodically
// started from main.cpp, stopped and deleted by main destructor
void background::main()
{
	/* soma_database D; */
	/* // try to create database. It will fail if the schema isn't empty */
	/* if (global::try_database_creation) D.create_database(); */
	/* // try to create some default values */
	/* D.create_default(); */

	started = true;
	int ctime = 0;

	while (started)
	{
		/* if (ctime % 3600 == 0) */
		/* { */
		/* } */
		mana::system::sleep(1000);
		ctime++;
	}
}

background::~background()
{
}

