#ifndef notes_H
#define notes_H

#include "config.h"

class player;

class notes
{
	public:
		dbo::ptr<player> p_player;

		string my_notes;

		template<class Action>
		void persist(Action &a)
		{
			dbo::belongsTo(a, p_player, dbo::OnDeleteCascade);
			dbo::field(a, my_notes, "my_notes");
		}

		notes() {}
};

#endif

