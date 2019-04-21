#ifndef _AC_DB_STUB_H_
#define _AC_DB_STUB_H_

#include "acdb.h"
#include "dbid.h"

class AcDbObject;
class AcDbEntity;


class AcDbStub
{
	friend struct AcDbStubPage;

private:
	AcDbStub();
	~AcDbStub();

	// No implementation, class copy forbidden
	AcDbStub(const AcDbStub&);
	AcDbStub& operator= (const AcDbStub&);

public:
	void mayHaveNonResidentSubspace(void);
	void space(void);
	void nextAndPreviousStubsInSpace(AcDbStub*&, AcDbStub*&, unsigned int);
	void openAcDbObject(AcDbObject *&, AcDb::OpenMode, bool, bool, bool);
	void openAcDbEntity(AcDbEntity *&, AcDb::OpenMode, bool, bool, bool);
	void objectOpenState(void);
	void canBeOpenedForRead(void);
	void slowObject(AcDbObject *&, AcDbObjectId);
	void readFromInputFile(long long, AcDbObject *&, AcDbObjectId);
	void unsetForwardingStub(AcDbObject *);
	void setForwardingStub(AcDbStub*, bool, bool);
	void removeExtension(bool);
	void changeForwardingStub(AcDbStub*);
	void setMayHaveSubspace(bool);
// 	void getIrec(irec *, AcDbEntity *);
// 	void setIrec(irec const&);
// 	void getExtents(iestruc &);
// 	void setExtents(iestruc const&);
	void swapObjects(AcDbStub*);
	void deleteObject(void);
	void prepareToDeleteSingleObject(bool);
	void vpVis(void);
	void slowDatabase(void);
	void slowDbHeader(void);
	void slowDocVars(void);
	void getClassIdLayerId(int &, AcDbObjectId &, AcDbDatabase *, bool);
	void readObjectLeftOnDisk(void);
	void objectClass(void);
};

#endif // _AC_DB_STUB_H_