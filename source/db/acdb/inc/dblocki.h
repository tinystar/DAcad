#ifndef _AC_DB_LOCKI_H_
#define _AC_DB_LOCKI_H_

class AcDbImpLock
{
public:
	AcDbImpLock();
	~AcDbImpLock();

public:
// 	void slowIsMyLock(void);
// 	void setLocked(AcAp::DocLockMode, wchar_t const*, bool);
// 	void removeAutoLock(AcDbImpLock::LockItem *, AcAp::DocLockMode);
// 	void sendLockModeWillChange(AcAp::DocLockMode, AcAp::DocLockMode, AcAp::DocLockMode, wchar_t const*);
// 	void sendLockModeChanged(AcAp::DocLockMode, AcAp::DocLockMode, AcAp::DocLockMode, wchar_t const*);
// 	void sendLockModeChangeVetoed(wchar_t const*);
// 	void stripAutoLock(AcDbImpLock::LockItem *);
// 	void setUnlocked(void);
// 	void lockMode(bool);
// 	void myLockMode(bool);
// 	void transferLock(void *, void *);
// 	void addReactor(AcDbLockReactor *);
// 	void removeReactor(AcDbLockReactor *);
// 	void addAssocReactor(AcDbLockReactor *);
// 	void removeAssocReactor(AcDbLockReactor *);
};

#endif // _AC_DB_LOCKI_H_