#include "widget_dice.h"
#include "soma.h"

widget_dice::widget_dice() :
	wcontainer("widget_dice"),
	signal_dice_results(this, "signal_dice_results"),
	signal_selector_click(this, "signal_selector_click")
{
	// dices area
	dices_area = bindNew<widget_template>("div_dices_area");
	dices_area->set_text("<div id=\"div_dices_area\" class=\"div_dices_area\"></div>");
	doJavaScript("init_dices_area('" + this->id() + "');");

	// animated d20
	animated_d20 = bindNew<widget_template>("div_animated_d20");
	animated_d20->set_text("<div id=\"div_animated_d20\" class=\"div_animated_d20\"></div>");
	doJavaScript("init_animated_d20();");

	// selector
	dice_selector = bindNew<wtemplate>("div_dice_selector", "widget_selector");
	dice_selector->setStyleClass("div_dice_selector hidden");
	text_notation = dice_selector->bindNew<WText>("text_notation");
	text_notation->setStyleClass("selector_notation_text");
	doJavaScript("init_animated_selector('" + this->id() + "');");
	// selector buttons
	button_clear = dice_selector->bindNew<WText>("button_clear");
	button_clear->setStyleClass("selector_buttons selector_button_clear");
	button_clear->setText("CLEAR");
	button_throw = dice_selector->bindNew<WText>("button_throw");
	button_throw->setStyleClass("selector_buttons selector_button_throw");
	button_throw->setText("THROW");

	// signal binding
	animated_d20->clicked().connect(this, &widget_dice::on_animated_d20_click);
	button_clear->clicked().connect(this, &widget_dice::on_clear_click);
	button_throw->clicked().connect(this, &widget_dice::on_throw_click);
	signal_dice_results.connect(this, &widget_dice::dice_results_callback);
	signal_selector_click.connect(this, &widget_dice::selector_click_callback);
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
		dice_selector->setStyleClass("div_dice_selector animate_show");
	}
}

void widget_dice::on_clear_click()
{
	for (auto &d : selector_pool)
	{
		auto &[d_type, value] = d;
		selector_pool[d_type] = 0;
	}
	selector_notation = "";
	text_notation->setText(selector_notation);
	text_notation->setPadding(0, Side::Left|Side::Right);
}

void widget_dice::on_throw_click()
{
	// hide selector
	dice_selector->setStyleClass("div_dice_selector animate_hide");
	// show dices area
	dices_area->removeStyleClass("div_dices_area_hide");
	throw_dice(selector_notation);
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
	// hide dices area after a delay
	dices_area->addStyleClass("div_dices_area_hide");
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
	text_notation->setText(selector_notation);

	// adjust padding if needed
	text_notation->setPadding(20, Side::Left|Side::Right);
	if (selector_notation == "") text_notation->setPadding(0, Side::Left|Side::Right);

	/* debug_line(selector_notation); */
}
