#include "TestSchd.h"
#include <iostream>

using namespace std;

#define DEF_TEST_ENTER(testid, description, entrypoint)	\
	extern void entrypoint(void);

#include "TestsDef.h"

#undef DEF_TEST_ENTER

//------------------------------------------------------------
//
//------------------------------------------------------------
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
	help();

	string sInput;
	while (true)
	{
		cin >> sInput;

		if ("exit" == sInput)
			break;

		if ("help" == sInput)
		{
			help();
			continue;
		}

		int id = atoi(sInput.c_str());
		TestIdToEntryMap::iterator itr = m_id2Entry.find(id);
		if (itr != m_id2Entry.end())
			itr->second._entrypoint();
		else
			cout << "Invalid input!" << endl;
	}
}

void TestScheduler::help()
{
	cout << "****************************************" << endl;
	cout << "* exit:	exit tests					 " << endl;
	cout << "* help:	print help					 " << endl;
	
	for (TestIdToEntryMap::iterator itr = m_id2Entry.begin();
		itr != m_id2Entry.end();
		++itr)
	{
		cout << "* " << itr->first << ":\t" << itr->second._description << endl;
	}

	cout << "****************************************" << endl;
}

void TestScheduler::init()
{
	registerTests();
}

void TestScheduler::registerTests()
{
#define DEF_TEST_ENTER(testid, description, entrypoint)	\
	m_id2Entry.insert(std::make_pair(testid, TestEntry(description, entrypoint)));

#include "TestsDef.h"

#undef DEF_TEST_ENTER
}

TestScheduler::TestScheduler()
{

}

TestScheduler::~TestScheduler()
{

}
