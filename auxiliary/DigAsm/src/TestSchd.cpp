#include "TestSchd.h"


TestScheduler* TestScheduler::_s_pScheduler = NULL;

TestScheduler* TestScheduler::scheduler()
{
	if (NULL == _s_pScheduler)
	{
		_s_pScheduler = new TestScheduler();
		_s_pScheduler->init();
	}

	return _s_pScheduler;
}

void TestScheduler::destroy()
{
	if (_s_pScheduler != NULL)
	{
		delete _s_pScheduler;
		_s_pScheduler = NULL;
	}
}

void TestScheduler::run()
{

}

void TestScheduler::help()
{

}

void TestScheduler::init()
{

}

void TestScheduler::registerTests()
{

}

TestScheduler::TestScheduler()
{

}

TestScheduler::~TestScheduler()
{

}
