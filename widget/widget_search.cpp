#include "widget_search.h"

widget_search::widget_search() : wcontainer("widget_search")
{
	edit_search = bindNew<WLineEdit>("edit_search");
	edit_search->setStyleClass("widget_search_edit");
	suggestions = bindNew<widget_template>("suggestions");

	edit_search->textInput().connect(this, &widget_search::on_key_pressed);
}

void widget_search::set_data(const vector<string> &data)
{
	F.set_data(data);
}

void widget_search::on_key_pressed()
{
	suggestions->setTemplateText("");
	string pattern = edit_search->text().toUTF8();

	// update suggestions
	auto res = F.search_all(pattern);
	int limit_shown = 10;
	for (auto &suggestion : res)
	{
		// change color for every char matches
		string value;
		if (suggestion.match_start > 0) value = substr(suggestion.value, 0, suggestion.match_start);
		int pos = suggestion.match_start;
		for (int pos_pattern = 0; pos < suggestion.match_end && pos_pattern < pattern.length(); pos++)
		{
			if (tolower(suggestion.value[pos]) == tolower(pattern[pos_pattern]))
			{
				value += string("<span class=\"widget_search_match\">") + suggestion.value[pos] + string("</span>");
				pos_pattern++;
			}
			else
			{
				// no match, add it with default color
				value += suggestion.value[pos];
			}
		}
		// add the end
		value += substr(suggestion.value, suggestion.match_end);

		string line = "<div class=\"widget_search_line_suggestion\">" + value + "</div>";
		auto new_suggestion = make_unique<widget_template>(line);

		// signal binding
		new_suggestion->clicked().connect(bind(&widget_search::on_select_choice, this, suggestion.value));

		// combine templates
		*suggestions += new_suggestion;

		if (--limit_shown <= 0) break;
	}

	edit_search->setFocus(true);
}

void widget_search::on_select_choice(string value)
{
	debug_line(value);
}
