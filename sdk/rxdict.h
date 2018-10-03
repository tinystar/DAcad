//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _rxdict_h
#define _rxdict_h 1

#include "rxobject.h"
#include "AdAChar.h"

#pragma pack (push, 8)

class AcRxDictionaryIterator;

class AcRxDictionary: public AcRxObject
{
public:
    ACRX_DECLARE_MEMBERS_EXPIMP(AcRxDictionary, ACBASE_PORT);

    virtual AcRxObject*    at      (const ACHAR* key) const = 0;					// 64//40h
    virtual AcRxObject*    at      (Adesk::UInt32 id) const = 0;					// 72//48h

    AcRxObject*            atPut   (const ACHAR* key, AcRxObject* pObject);
    virtual AcRxObject*    atPut   (const ACHAR* key, AcRxObject* pObject,
        Adesk::UInt32& retId) = 0;													// 80//50h
    virtual AcRxObject*    atPut   (Adesk::UInt32 id, AcRxObject* pObject) = 0;		// 88//58h

    virtual Adesk::Boolean resetKey(const ACHAR* oldKey, const ACHAR* newKey) = 0;	// 96//60h
    virtual Adesk::Boolean resetKey(Adesk::UInt32 id, const ACHAR* newKey) = 0;		// 104//68h

    virtual Adesk::Boolean atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id,
        AcRxObject* pObject) = 0;													// 112//70h

    virtual AcRxObject*    remove  (const ACHAR* key) = 0;							// 120//78h
    virtual AcRxObject*    remove  (Adesk::UInt32 id) = 0;							// 128//80h

    virtual Adesk::Boolean has     (const ACHAR* entryName) const = 0;				// 136//88h
    virtual Adesk::Boolean has     (Adesk::UInt32 id) const = 0;					// 144//90h

    virtual Adesk::UInt32  idAt    (const ACHAR* key) const = 0;					// 152//98h

    virtual const ACHAR*    keyAt   (Adesk::UInt32 id) const = 0;					// 160//A0h

    virtual Adesk::UInt32  numEntries() const = 0;									// 168//A8h

    virtual AcRxDictionaryIterator* newIterator(
        AcRx::DictIterType type = AcRx::kDictSorted) = 0;							// 176//B0h

    virtual Adesk::Boolean deletesObjects() const = 0;								// 184//B8h
    virtual Adesk::Boolean isCaseSensitive() const = 0;								// 192//C0h
    virtual Adesk::Boolean isSorted() const = 0;									// 200//C8h
};

extern "C" ACBASE_PORT AcRxDictionary* acrxSysRegistry();

inline AcRxObject* AcRxDictionary::atPut(const ACHAR* key, AcRxObject* pObj)
{ Adesk::UInt32 temp; return atPut(key, pObj, temp); }

#define acrxClassDictionary \
AcRxDictionary::cast(acrxSysRegistry()->at(ACRX_CLASS_DICTIONARY))

#define acrxServiceDictionary \
AcRxDictionary::cast(acrxSysRegistry()->at(ACRX_SERVICE_DICTIONARY))

#pragma pack (pop)
#endif
