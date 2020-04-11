#include "unit_test.h"
#include "user.h"

bool unit_test::test_user()
{
	if (!user::load())
	{
		debug_line("failed to load user.yml");
		return false;
	}
	if (! user::check_password("unit_test", "unit_test_pass"))
	{
		debug_line("failed to log user");
		return false;
	}
	if (user::check_password("unit_test", "xxx"))
	{
		debug_line("log user with wrong password");
		return false;
	}
	if (! user::check_password("unit_test_sha1", "870B15CF8CFDADBAA555C5CAAE18CAE6EDE92259"))
	{
		debug_line("failed to log user with sha1 password");
		return false;
	}
	return true;
}

bool unit_test::test_all()
{
	bool ret = true;
	ret = ret && test_user();
	return ret;
}
