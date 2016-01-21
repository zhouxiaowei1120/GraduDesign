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
	void setLon(double Lon);			//���뾭��
	double getLon();					//��������
	void setLat(double Lat);			//����γ��
	double getLat();					//����γ��
	void setAlt(double Alt);			//���뺣�θ߶�
	double getAlt();					//�������θ߶�
	void setArea(double Area);			//����¥�����
	double getArea();					//����¥�����
	void setHight(double Hight);		//����¥���߶�
	double getHight();					//����¥���߶�
	void setType(int Type);				//����¥����������
	int getType();						//����¥����������
	void setNumOfPeo(int NumOfPeo);		//�����˿�����
	int getNumOfPeo();					//�����˿�����

private:
	double lon;					//¥������
	double lat;					//¥��γ��
	double alt;					//¥�����θ߶�
	double area;				//¥�����
	double hight;				//¥���߶�
	int type;					//¥�����ܷ���   0:������ 1:ѧУ 2:�̳� 3:��԰ 4:ҽԺ 5:���� 6:���� 7:�칫����
	int NumOfPeople;			//���Ƶó����˿�����

};

#endif // !defined(AFX_FLOOR_H__55E00717_426A_45AC_9B39_61320080E524__INCLUDED_)
