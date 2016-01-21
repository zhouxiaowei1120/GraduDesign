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
		lon = Lon;					//���뾭��
	}
	
	double getLon()
	{
		return lon;					//��������
	}
	
	void setLat(double Lat)
	{
		lat = Lat;					//����γ��
	}
	
	double getLat()
	{
		return lat;					//����γ��
	}
	
	void setAlt(double Alt)
	{
		alt = Alt;					//���뺣�θ߶�
	}
	
	double getAlt()
	{
		return alt;					//�������θ߶�
	}
	
	void setCap(int Cap)
	{
		capacity = Cap;				   //��������������ɵ�����
	}
	
	int getCap()
	{
		return capacity;				//���������������ɵ�����
	}
	

	void setType(int Type)
	{
		type = Type;				//�����������������
	}
	
	int getType()
	{
		return type;				//������������������
	}
		
	void setFlood(bool Flood)
	{
		flood = Flood;				//���ñ������Ƿ���û
	}

	bool getFlood()
	{
		return flood;				//�鿴�������Ƿ���û
	}
	
private:
	double lon;					//¥������
	double lat;					//¥��γ��
	double alt;					//¥�����θ߶�
	int capacity;				//¥�����������������ɶ�����
	int type;					//¥�����ܷ���   0:������ 1:ѧУ 2:�̳� 3:��԰ 4:ҽԺ 5:���� 6:���� 7:�칫���� 8:�羰�� 9:����ݣ�������
	bool flood;					//��true����ʾ����û����false����ʾû�б���û
};

#endif // !defined(AFX_SHELTER_H__41E5440F_44A8_499A_87DD_4DD2B22A79E5__INCLUDED_)
