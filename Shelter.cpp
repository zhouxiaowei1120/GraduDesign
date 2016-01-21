// Shelter.cpp: implementation of the CShelter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraduDesign.h"
#include "Shelter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShelter::CShelter()
{
	lon = 0;
	lat = 0;
	alt = 0;
	capacity = 0;
	type = 0;
	flood = TRUE;
}

CShelter::~CShelter()
{

}
