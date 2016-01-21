// Floor.h: interface for the CFloor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLOOR_H__55E00717_426A_45AC_9B39_61320080E524__INCLUDED_)
#define AFX_FLOOR_H__55E00717_426A_45AC_9B39_61320080E524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFloor  
{
public:
	CFloor();
	virtual ~CFloor();
	void setLon(double Lon);			//读入经度
	double getLon();					//读出经度
	void setLat(double Lat);			//读入纬度
	double getLat();					//读出纬度
	void setAlt(double Alt);			//读入海拔高度
	double getAlt();					//读出海拔高度
	void setArea(double Area);			//读入楼房面积
	double getArea();					//读出楼房面积
	void setHight(double Hight);		//读入楼房高度
	double getHight();					//读出楼房高度
	void setType(int Type);				//读入楼房功能类型
	int getType();						//读出楼房功能类型
	void setNumOfPeo(int NumOfPeo);		//读入人口数量
	int getNumOfPeo();					//读出人口数量

private:
	double lon;					//楼房经度
	double lat;					//楼房纬度
	double alt;					//楼房海拔高度
	double area;				//楼房面积
	double hight;				//楼房高度
	int type;					//楼房功能分类   0:居民区 1:学校 2:商场 3:公园 4:医院 5:宾馆 6:工厂 7:办公场所
	int NumOfPeople;			//估计得出的人口数量

};

#endif // !defined(AFX_FLOOR_H__55E00717_426A_45AC_9B39_61320080E524__INCLUDED_)
