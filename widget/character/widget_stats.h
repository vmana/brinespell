#ifndef widget_stats_H
#define widget_stats_H

#include "widget/wcontainer.h"
#include <Wt/WLineEdit.h>
#include <Wt/WImage.h>

class widget_stats : public wcontainer
{
	public:

		WText *mod_str;
		WText *mod_dex;
		WText *mod_con;
		WText *mod_int;
		WText *mod_wis;
		WText *mod_cha;

		string current_stat = "";
		wtemplate *details_stat;
		WText *close_stat;
		WText *base_stat;
		WText *bonus_stat;
		WLineEdit *tmp_stat;
		WText *mod_stat;
		WText *total_stat;

		wtemplate *details_skill;

		// hold skills data
		// dbo::ptr, proficiency, total
		typedef tuple<dbo::ptr<skill>, WImage*, WLineEdit*> skill_line;
		vector<skill_line> vskill;

		widget_stats();

		string notation(int value); // return notation value, ie -2, +3 ...
		void update_stats();
		void prepare_details(string name);
		void update_details();
		void on_tmp_stat_changed();
		void update_skills();
		void update_skill(skill_line &line);
		void on_skill_changed(skill_line &line);
		void on_proficiency_click(skill_line &line);
		void switch_details_visibility();
};

#endif // widget_stats_H

