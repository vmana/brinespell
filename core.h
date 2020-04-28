#ifndef core_H
#define core_H

#include "config.h"
#include "soma.h"

class core
{
	soma *S;

	public:
		core();
		template <class C, typename... Args>
		void internal_send_all(C *object, void (C::*fn)(Args...), Args... args);

		template <class C, typename... Args>
		static std::function<void()> send_all(C *object, void (C::*fn)(Args...), Args... args);
};

template <class C, typename... Args>
void core::internal_send_all(C *object, void (C::*fn)(Args...), Args... args)
{
	(object->*fn)(args...);
}

template <class C, typename... Args>
std::function<void()> core::send_all(C *object, void (C::*fn)(Args...), Args... args)
{
	soma* p_soma = soma::application();
	if (!p_soma)
	{
		debug_line("p_soma error");
		return bind(fn, object, args...);
	}
	core* p_core = p_soma->C;
	if (!p_core)
	{
		debug_line("p_core error");
		return bind(fn, object, args...);
	}

	return bind(&core::internal_send_all<C, Args...>, p_core, object, fn, args...);
}
#endif
