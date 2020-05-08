#ifndef wt_dbo_version_h
#define wt_dbo_version_h
#include <Wt/Dbo/Dbo.h>

class user;
class player;
class campaign;

namespace dbo = Wt::Dbo;

namespace Wt
{
	namespace Dbo
	{
		template<> struct dbo_traits<user> : public dbo_default_traits { static const char *versionField() { return 0; } };
		template<> struct dbo_traits<player> : public dbo_default_traits { static const char *versionField() { return 0; } };
		template<> struct dbo_traits<campaign> : public dbo_default_traits { static const char *versionField() { return 0; } };
	}
}

#endif
