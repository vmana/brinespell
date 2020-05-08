#ifndef broadcast_H
#define broadcast_H

#include "config.h"
#include "soma.h"
#include <Wt/WServer.h>

class broadcast
{

	private:
		template <typename... Args>
		static void internal_all(int campaign_id, void (*fn)(Args...), Args... args);

	public:

		template <typename... Args>
		/* static std::function<void()> all(void (*fn)(Args...), Args... args); */
		static void all(void (*fn)(Args...), Args... args);
};

template <typename... Args>
void broadcast::internal_all(int campaign_id, void (*fn)(Args...), Args... args)
{
	// compare campaign_id with S->p_campaign
	// if it's the same, execute fn, else don't care since it's not broadcasted for us
	soma* p_soma = soma::application();
	if (p_soma && p_soma->p_campaign)
	{
		if (p_soma->p_campaign.id() == campaign_id)
		{
			(fn)(args...);
		}
	}
}

template <typename... Args>
void broadcast::all(void (*fn)(Args...), Args... args)
{
	soma* p_soma = soma::application();
	if (!(p_soma && p_soma->p_campaign)) return;
	int campaign_id = p_soma->p_campaign.id();

	WServer *wt_server = WServer::instance();
	if (wt_server)
	{
		wt_server->postAll(std::bind(&broadcast::internal_all<Args...>, campaign_id, fn, args...));
	}
}

#endif
