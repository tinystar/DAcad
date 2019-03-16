#include "dbmain.h"
#include "acutmacro.h"


AcCmColor::AcCmColor()
	: mColorName(NULL)
	, mBookName(NULL)
	, mNameFlags(0)
{
	mRGBM.whole = 0;

	AC_ASSERT_NOT_IMPLEMENTED();
}

AcCmColor::AcCmColor(const AcCmColor&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

AcCmColor& AcCmColor::operator = (const AcCmColor& inputColor)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return *this;
}

AcCmColor::~AcCmColor()
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

AcCmEntityColor::ColorMethod AcCmColor::colorMethod() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return AcCmEntityColor::kNone;
}

Acad::ErrorStatus AcCmColor::setColorMethod(AcCmEntityColor::ColorMethod eColorMethod)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

bool AcCmColor::isByColor() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool AcCmColor::isByLayer() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool AcCmColor::isByBlock() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool AcCmColor::isByACI() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool AcCmColor::isByPen() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool AcCmColor::isForeground() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

Adesk::UInt32     AcCmColor::color() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

Acad::ErrorStatus AcCmColor::setColor(Adesk::UInt32 color)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcCmColor::setRGB(Adesk::UInt8 red, Adesk::UInt8 green, Adesk::UInt8 blue)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcCmColor::setRed(Adesk::UInt8 red)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcCmColor::setGreen(Adesk::UInt8 green)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcCmColor::setBlue(Adesk::UInt8 blue)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Adesk::UInt8      AcCmColor::red() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

Adesk::UInt8      AcCmColor::green() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

Adesk::UInt8      AcCmColor::blue() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}


Adesk::UInt16     AcCmColor::colorIndex() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

Acad::ErrorStatus AcCmColor::setColorIndex(Adesk::UInt16 colorIndex)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Adesk::UInt16     AcCmColor::penIndex() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

Acad::ErrorStatus AcCmColor::setPenIndex(Adesk::UInt16 penIndex)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcCmColor::setNames(const ACHAR *colorName, const ACHAR *bookName /*= NULL*/)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

const ACHAR *     AcCmColor::colorName(void) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR *     AcCmColor::bookName(void) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR *     AcCmColor::colorNameForDisplay(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

bool              AcCmColor::hasColorName(void) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool              AcCmColor::hasBookName(void) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}
