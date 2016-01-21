// Floor.cpp: implementation of the CFloor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraduDesign.h"
#include "Floor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CFloor::CFloor()
{
	lon = 0;
	lat = 0;
	alt = 0;
	area = 0;
	hight = 0;
	type = 0;
	NumOfPeople = 0;
}

CFloor::~CFloor()
{

}

void CFloor::setLon(double Lon)
{
	lon = Lon;					//读入经度
}

double CFloor::getLon()
{
	return lon;					//读出经度
}

void CFloor::setLat(double Lat)
{
	lat = Lat;					//读入纬度
}

double CFloor::getLat()
{
	return lat;					//读出纬度
}

void CFloor::setAlt(double Alt)
{
	alt = Alt;					//读入海拔高度
}

double CFloor::getAlt()
{
	return alt;					//读出海拔高度
}

void CFloor::setArea(double Area)
{
	area = Area;				//读入楼房面积
}

double CFloor::getArea()
{
	return area;				//读出楼房面积
}

void CFloor::setHight(double Hight)
{
	hight = Hight;				//读入楼房高度
}

double CFloor::getHight()
{
	return hight;				//读出楼房高度
}

void CFloor::setType(int Type)
{
	type = Type;				//读入楼房功能类型
}

int CFloor::getType()
{
	return type;				//读出楼房功能类型
}

void CFloor::setNumOfPeo(int NumOfPeo)
{
	NumOfPeople = NumOfPeo;				//读入人口数量
}

int CFloor::getNumOfPeo()
{
	return NumOfPeople;				//读出人口数量
}