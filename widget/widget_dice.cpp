#include "widget_dice.h"
#include "soma.h"

widget_dice::widget_dice() :
	wcontainer("dice"),
	signal_dice_results(this, "signal_dice_results"),
	signal_selector_click(this, "signal_selector_click")
{
	// load js lib if not loaded
	S->application()->require("js/cannon.min.js");
	S->application()->require("js/three.min.js");
	S->application()->require("js/dice.js");

	// dices area
	dices_area = bindNew<widget_template>("div_dices_area");
	dices_area->set_text("<div id=\"div_dices_area\" class=\"div_dices_area\"></div>");
	doJavaScript("init_dices_area('" + this->id() + "');");

	// secret dices area
	secret_dices_area = bindNew<widget_template>("div_secret_dices_area");
	secret_dices_area->set_text("<div id=\"div_secret_dices_area\" class=\"div_secret_dices_area\"></div>");
	doJavaScript("init_secret_dices_area('" + this->id() + "');");

	// animated d20
	animated_d20 = bindNew<widget_template>("div_animated_d20");
	animated_d20->set_text("<div id=\"div_animated_d20\" class=\"div_animated_d20\"></div>");
	doJavaScript("init_animated_d20();");

	draw_selector();

	// signal binding
	animated_d20->clicked().connect(this, &widget_dice::on_animated_d20_click);
	button_left->clicked().connect(this, &widget_dice::on_secret_throw_click);
	button_right->clicked().connect(this, &widget_dice::on_throw_click);
	signal_dice_results.connect(this, &widget_dice::dice_results_callback);
	signal_selector_click.connect(this, &widget_dice::selector_click_callback);
}

void widget_dice::draw_selector()
{
	// selector
	dice_selector = bindNew<wtemplate>("div_dice_selector", "selector");
	dice_selector->setStyleClass("div_dice_selector hidden");
	/* dice_selector->setStyleClass("div_dice_selector"); */

	text_notation = dice_selector->bindNew<WText>("text_notation");
	text_notation->setStyleClass("selector_notation_text");
	doJavaScript("init_animated_selector('" + this->id() + "');");

	// selector buttons

	button_left = dice_selector->bindNew<WText>("button_left");
	button_left->setStyleClass("selector_button_left");

	button_middle = dice_selector->bindNew<WText>("button_middle");
	button_middle->setStyleClass("selector_button_middle");

	button_right = dice_selector->bindNew<WText>("button_right");
	button_right->setStyleClass("selector_button_right");

	canvas_decoration_left = dice_selector->bindNew<widget_template>("canvas_decoration_left");
	canvas_decoration_left->set_text("<canvas id=\"canvas_decoration_left\">");

	canvas_decoration_right = dice_selector->bindNew<widget_template>("canvas_decoration_right");
	canvas_decoration_right->set_text("<canvas id=\"canvas_decoration_right\">");

	doJavaScript("init_selector_buttons();");
}

void widget_dice::on_animated_d20_click()
{
	if (dice_selector->hasStyleClass("animate_show"))
	{
		dice_selector->removeStyleClass("hidden");
		dice_selector->setStyleClass("div_dice_selector animate_hide");
	}
	else
	{
		clear_notation();
		dice_selector->setStyleClass("div_dice_selector animate_show");
	}
}

void widget_dice::clear_notation()
{
	for (auto &d : selector_pool)
	{
		auto &[d_type, value] = d;
		selector_pool[d_type] = 0;
	}
	selector_notation = "";
	text_notation->setText("Choose your dice");
}

void widget_dice::on_throw_click()
{
	if (selector_notation == "") return; // no dice to throw

	// throw_initialized_dice_event, so everyone can see the throw
	rand_init = "";
	for (int i = 0; i < 100; i++)
		rand_init += "0." + rand_int_string(4) + ",";
	rand_init = substr(rand_init, 0, -1); // remove last ,

	throw_dice_event.emit(selector_notation, rand_init);
	throw_dice(selector_notation, rand_init);
}

void widget_dice::on_secret_throw_click()
{
	if (selector_notation == "") return; // no dice to throw
	// hide selector
	dice_selector->setStyleClass("div_dice_selector animate_hide");

	doJavaScript("throw_secret_dices_area('" + selector_notation + "');");
}

void widget_dice::throw_dice(string notation)
{
	rand_init = "";
	for (int i = 0; i < 100; i++)
		rand_init += "0." + rand_int_string(4) + ",";
	rand_init = substr(rand_init, 0, -1); // remove last ,
	throw_dice(notation, rand_init);
}

void widget_dice::throw_dice(string notation, string rand_init)
{
	this->rand_init = rand_init;

	// hide selector
	dice_selector->setStyleClass("div_dice_selector animate_hide");

	doJavaScript("throw_initialized_dices_area('" + notation + "', [" + rand_init + "]);");
}

void widget_dice::throw_dice_nocallback(string notation, string rand_init)
{
	this->rand_init = rand_init;

	// hide selector
	dice_selector->setStyleClass("div_dice_selector animate_hide");

	doJavaScript("throw_initialized_dices_area_nocallback('" + notation + "', [" + rand_init + "]);");
}

void widget_dice::dice_results_callback(string value, bool is_secret)
{
	auto values = explode(" ", value);
	int sum = 0;
	for (auto &v : values)
		sum += convert::string_int(v);

	if (!is_secret)
	{
		dice_results_event.emit("<i>Dice results " + value + " (<span class=\"widget_chat_dice_total\">" + convert::int_string(sum) + "</span>)</i>");
	}
	else
	{
		dice_secret_results_event.emit("<i>Secret dice results " + value + " (<span class=\"widget_chat_dice_total\">" + convert::int_string(sum) + "</span>)</i>");
	}
}

void widget_dice::selector_click_callback(int dice_type, int count)
{
	/* debug_line(dice_type << ":" << count); */
	if (count != -1 && count != 1) return;
	try
	{
		selector_pool[dice_type] += count;
		if (selector_pool.at(dice_type) < 0) selector_pool[dice_type] = 0;
	} catch(const out_of_range &e) {}

	selector_notation = "";
	for (auto &d : selector_pool)
	{
		auto &[d_type, value] = d;
		if (value == 0) continue;
		selector_notation += convert::int_string(value) + "d" + convert::int_string(d_type) + " + ";
	}
	// remove last " + "
	selector_notation = substr(selector_notation, 0, -3);

	if (selector_notation != "")
	{
		text_notation->setText(selector_notation);
	}
	else
	{
		text_notation->setText("Choose your dice");
	}
}
