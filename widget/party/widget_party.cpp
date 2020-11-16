#include "widget_party.h"
#include "soma.h"
#include "db/player.h"
#include "db/campaign.h"

widget_party::widget_party() : wcontainer("party/party")
{
	S->application()->useStyleSheet("css/party.css");

	setStyleClass("widget_party");

	// signal binding

	// update values
	/* details_hp->update_hit_point(); */
}
