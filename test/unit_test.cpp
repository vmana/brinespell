#include "unit_test.h"
#include "db/user.h"
#include "db/campaign.h"

bool unit_test::test_user()
{
	return true;
}

bool unit_test::test_campaign()
{
	return true;
}

bool unit_test::test_all()
{
	bool ret = true;
	ret = ret && test_user();
	ret = ret && test_campaign();
	return ret;
}
