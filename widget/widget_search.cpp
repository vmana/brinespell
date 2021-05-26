#include "widget_search.h"

widget_search::widget_search() : wcontainer("search")
{
	setToolTip("Global HotKey : Enter");
	edit_search = bindNew<WLineEdit>("edit_search");
	edit_search->setStyleClass("widget_search_edit");
	suggestions = bindNew<widget_template>("suggestions");

	edit_search->textInput().connect(this, &widget_search::on_input_changed);
	edit_search->keyWentDown().connect(this, &widget_search::on_key_pressed);
}

void widget_search::set_data(const vector<string> &data)
{
	F.set_data(data);
}

void widget_search::on_input_changed()
{
	string pattern = edit_search->text().toUTF8();

	// clear previous choices
	wresults.clear();
	suggestions->set_text("");

	results = F.search_all(pattern, 10);

	// auto select first item, or none if no results
	if (results.size() > 0) selected = 0;
	else selected = -1;

	// create template text
	string template_text = "";
	for (int i = 0; i < results.size(); i++)
	{
		string new_id = "search_line_" + convert::int_string(i);
		template_text += "${" + new_id + "}\n";
	}
	suggestions->set_text(template_text);

	// update suggestions
	for (int i = 0; i < results.size(); i++)
	{
		auto &suggestion = results[i];
		// change color for every char matches
		string text;
		if (suggestion.match_start > 0) text = substr(suggestion.text, 0, suggestion.match_start);
		int pos = suggestion.match_start;
		for (int pos_pattern = 0; pos < suggestion.match_end && pos_pattern < pattern.length(); pos++)
		{
			if (tolower(suggestion.text[pos]) == tolower(pattern[pos_pattern]))
			{
				text += string("<span class=\"widget_search_match\">") + suggestion.text[pos] + string("</span>");
				pos_pattern++;
			}
			else
			{
				// no match, add it with default color
				text += suggestion.text[pos];
			}
		}
		// add the end
		text += substr(suggestion.text, suggestion.match_end);

		// set css class, with a visual difference if it's the current selection
		string css_class = "widget_search_line_suggestion";
		if (i == selected) css_class += " widget_search_selected";

		/* string line = "<div class=\"" + css_class + "\">" + text + "</div>"; */
		auto new_suggestion = make_unique<widget_template>(text);
		new_suggestion->setStyleClass(css_class);

		// signal binding
		new_suggestion->clicked().connect(bind(&widget_search::on_select_choice, this, suggestion.text));

		// combine templates
		string new_id = "search_line_" + convert::int_string(i);
		auto p_new_suggestion = suggestions->bindWidget(new_id, move(new_suggestion));
		wresults.push_back(p_new_suggestion);
	}

	edit_search->setFocus(true);
}

void widget_search::on_key_pressed(const WKeyEvent &event)
{
	if (event.key() == Key::Up)
	{
		if (selected > 0) selected--;
		update_selected();
	}
	else if (event.key() == Key::Down)
	{
		if (selected < results.size() - 1) selected++;
		update_selected();
	}
	else if (event.key() == Key::Enter)
	{
		on_enter_pressed();
	}
	else if (event.key() == Key::Escape)
	{
		// remove focus if escape is pressed on an empty input
		if (edit_search->text() == "") remove_focus_event.emit();
		selected = -1;
		suggestions->set_text("");
		edit_search->setText("");
	}
}

void widget_search::on_enter_pressed()
{
	if (selected >= 0 && selected < results.size())
	{
		on_select_choice(results[selected].text);
		remove_focus_event.emit();
	}
	else if (edit_search->text() == "")
	{
		// remove focus if enter is pressed on an empty input
		remove_focus_event.emit();
	}
}

void widget_search::on_select_choice(string text)
{
	on_select_event.emit(text);

	selected = -1;
	suggestions->set_text("");
	edit_search->setText("");
}

void widget_search::update_selected()
{
	for (int i = 0; i < wresults.size(); i++)
	{
		if (i == selected)
		{
			// add selected style class
			wresults[i]->addStyleClass("widget_search_selected");
		}
		else
		{
			// remove style class of present
			wresults[i]->removeStyleClass("widget_search_selected");
		}
	}
}
