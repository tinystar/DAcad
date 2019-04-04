#ifndef __TEST_SCHD_H__
#define __TEST_SCHD_H__

#include <string>
#include <map>

class TestScheduler
{
public:
	static TestScheduler* scheduler();
	static void destroy();

public:
	void run();
	void help();

protected:
	void init();
	void registerTests();

private:
	TestScheduler();
	~TestScheduler();

	static TestScheduler* _s_pScheduler;

private:
	typedef void (*TestEntrypoint)(void);

	struct TestEntry
	{
		std::string		_description;
		TestEntrypoint	_entrypoint;

		TestEntry(const char* pszDesc, TestEntrypoint method)
			: _entrypoint(method)
			, _description(pszDesc)
		{}
	};

	typedef std::map<int, TestEntry> TestIdToEntryMap;

	TestIdToEntryMap	m_id2Entry;
};

#endif // __TEST_SCHD_H__