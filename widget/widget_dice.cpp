#include "widget_dice.h"
#include "soma.h"

widget_dice::widget_dice() :
	wcontainer("widget_dice"),
	signal_dice_results(this, "signal_dice_results"),
	signal_selector_click(this, "signal_selector_click")
{
	animated_d20 = bindNew<widget_template>("div_animated_d20");
	animated_d20->set_text("<div id=\"div_animated_d20\" class=\"div_animated_d20\"/>");
	doJavaScript("init_animated_d20();");
	doJavaScript("init_animated_selector('" + this->id() + "');");

	text_notation = bindNew<widget_template>("text_notation");

	dices_area = bindNew<widget_template>("div_dices_area");
	dices_area->set_text("<div id=\"div_dices_area\" class=\"div_dices_area\"/>");
	doJavaScript("init_dices_area('" + this->id() + "');");

	animated_d20->clicked().connect([&](){ throw_dice(selector_notation); });

	// signal binding
	signal_dice_results.connect(this, &widget_dice::dice_results_callback);
	signal_selector_click.connect(this, &widget_dice::selector_click_callback);
}

void widget_dice::throw_dice(string notation)
{
	rand_init = "";
	for (int i = 0; i < 100; i++)
		rand_init += "0." + rand_int_string(4) + ",";
	rand_init = substr(rand_init, 0, -1); // remove last ,
	throw_initialized_dice(notation, rand_init);
}

void widget_dice::throw_initialized_dice(string notation, string rand_init)
{
	this->rand_init = rand_init;
	doJavaScript("thow_initialized_dices_area('" + notation+ "', [" + rand_init + "]);");
}

void widget_dice::dice_results_callback(string value)
{
	debug_line(value);
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
	text_notation->set_text(selector_notation);
	/* debug_line(selector_notation); */
}
