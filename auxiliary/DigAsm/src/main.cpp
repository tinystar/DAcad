#include "TestSchd.h"


int main()
{
	TestScheduler::scheduler()->run();
	TestScheduler::destroy();

	return 0;
}