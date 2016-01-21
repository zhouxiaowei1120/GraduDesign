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
	lon = Lon;					//���뾭��
}

double CFloor::getLon()
{
	return lon;					//��������
}

void CFloor::setLat(double Lat)
{
	lat = Lat;					//����γ��
}

double CFloor::getLat()
{
	return lat;					//����γ��
}

void CFloor::setAlt(double Alt)
{
	alt = Alt;					//���뺣�θ߶�
}

double CFloor::getAlt()
{
	return alt;					//�������θ߶�
}

void CFloor::setArea(double Area)
{
	area = Area;				//����¥�����
}

double CFloor::getArea()
{
	return area;				//����¥�����
}

void CFloor::setHight(double Hight)
{
	hight = Hight;				//����¥���߶�
}

double CFloor::getHight()
{
	return hight;				//����¥���߶�
}

void CFloor::setType(int Type)
{
	type = Type;				//����¥����������
}

int CFloor::getType()
{
	return type;				//����¥����������
}

void CFloor::setNumOfPeo(int NumOfPeo)
{
	NumOfPeople = NumOfPeo;				//�����˿�����
}

int CFloor::getNumOfPeo()
{
	return NumOfPeople;				//�����˿�����
}