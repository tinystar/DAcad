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

#ifndef _rxiter_h
#define _rxiter_h 1

#include "rxobject.h"
#pragma pack (push, 8)

class AcRxIterator: public AcRxObject
{
public:
    ACRX_DECLARE_MEMBERS_EXPIMP(AcRxIterator, ACBASE_PORT);

    virtual Adesk::Boolean done() const = 0;		// 64//40h
    virtual Adesk::Boolean next() = 0;				// 72//48h
    virtual AcRxObject*    object() const = 0;		// 80//50h
};

#pragma pack (pop)
#endif
