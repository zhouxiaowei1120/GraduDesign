// Shelter.h: interface for the CShelter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELTER_H__41E5440F_44A8_499A_87DD_4DD2B22A79E5__INCLUDED_)
#define AFX_SHELTER_H__41E5440F_44A8_499A_87DD_4DD2B22A79E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShelter  
{
public:
	CShelter();
	virtual ~CShelter();

	void setLon(double Lon)
	{
		lon = Lon;					//读入经度
	}
	
	double getLon()
	{
		return lon;					//读出经度
	}
	
	void setLat(double Lat)
	{
		lat = Lat;					//读入纬度
	}
	
	double getLat()
	{
		return lat;					//读出纬度
	}
	
	void setAlt(double Alt)
	{
		alt = Alt;					//读入海拔高度
	}
	
	double getAlt()
	{
		return alt;					//读出海拔高度
	}
	
	void setCap(int Cap)
	{
		capacity = Cap;				   //读入避难所可容纳的人数
	}
	
	int getCap()
	{
		return capacity;				//读出避难所可容纳的人数
	}
	

	void setType(int Type)
	{
		type = Type;				//读入避难所功能类型
	}
	
	int getType()
	{
		return type;				//读出避难所功能类型
	}
		
	void setFlood(bool Flood)
	{
		flood = Flood;				//设置避难所是否被淹没
	}

	bool getFlood()
	{
		return flood;				//查看避难所是否被淹没
	}
	
private:
	double lon;					//楼房经度
	double lat;					//楼房纬度
	double alt;					//楼房海拔高度
	int capacity;				//楼房容纳力，即可容纳多少人
	int type;					//楼房功能分类   0:居民区 1:学校 2:商场 3:公园 4:医院 5:宾馆 6:工厂 7:办公场所 8:风景区 9:博物馆，大厅等
	bool flood;					//“true”表示被淹没，“false”表示没有被淹没
};

#endif // !defined(AFX_SHELTER_H__41E5440F_44A8_499A_87DD_4DD2B22A79E5__INCLUDED_)
