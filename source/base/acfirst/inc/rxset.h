#ifndef _AC_RX_SET_H_
#define _AC_RX_SET_H_

#include "rxobject.h"

class AcRxIterator;

class AcRxSet : public AcRxObject
{
public:
	ACRX_DECLARE_MEMBERS(AcRxSet);

	virtual void add(AcRxObject*) = 0;					// 64//40h
	virtual void remove(const AcRxObject*) = 0;			// 72//48h
	virtual void removeAllElements(void) = 0;			// 80//50h
	virtual bool has(const AcRxObject*) const = 0;		// 88//58h
	virtual unsigned int nbrOfElements(void) = 0;		// 96//60h
	virtual void intersectWith(const AcRxSet*) = 0;		// 104//68h
	virtual void unionWith(const AcRxSet*) = 0;			// 112//70h
	virtual AcRxIterator* newIterator(void) = 0;		// 120//78h
};


AcRxSet* newAcRxSet(void);

#endif // _AC_RX_SET_H_