#ifndef TYPE_H
#define TYPE_H

#include "fstream.h"
#include "iomanip.h"
#include "stdafx.h"

struct LineNodes // ÿ��·�����ݽṹ
{
	double S_lon;
	double S_lat;
	double E_lon;
	double E_lat;
	double S_alt;
	double E_alt;
	double length;
	double width;					//��·���
	int code;
	int Max_Edge_Cap;				//�洢�ߵ����������
	int Available_Edge_Cap;			//��ǰʱ�̿��õ����������õ�·��ǰ��t�ڿ�����������˽����·��
	double WalkTime;				//��������Ҫ��ʱ��
	int NumofTimeQuantum;			//ͨ��������·����Ҫ�Ħ�t������
};

struct Vertexs
{
	double lon;
	double lat;
	int code;
	double alt;						//�洢����
	int Max_Code_Cap;				//�洢������������
	int Available_Code_Cap;			//��ǰʱ�̣���Ŀ���������
	int StartOrEnd;					//����ɢ��㡢��������־  ����ɢ���Ϊ0���������յ�Ϊ2��������Ϊ1
};

struct structNodes					// ����ÿ���߽ڵ�����ݽṹ
{
	int num;						// �˱���ָ��ĵ�����
	LineNodes _LineNodes;			// �ߵĽṹ
	structNodes *next;				// ָ����һ���ߵ�ָ��
};

//structNodes AdjList[200];
struct vLineNodes					// �ڽӱ��е�ڵ�Ľṹ
{
	Vertexs data;					//  ��Ľṹ
	structNodes *first;				// ָ��߱��ָ��
};

struct ALGraph						//ͼ�Ľṹ
{
	vLineNodes _structNodes[1000];
	int vertexnum;					//ͼ�е����Ŀ	
	int arcnum;						//ͼ�бߵ���Ŀ
};

void gainData(CString filename);					//���ļ���ȡ����
int GetCodeOfArcPointTo(LineNodes temp, int k);
int CaculateVertexofGraph(CString filepath);		//�ҵ�·���е����н��
BOOL TheVertexIN(double lon, double lat, int sum);
void CreateGraph(ALGraph &s,CString filepath);		//����ͼ
void printGraph(ALGraph &s,CString filepath);		//���ͼ


struct ShortPathEdge 
{
	Vertexs *data;			//�洢���·���е����Ϣ
	ShortPathEdge *next;			//ָ�����·���е���һ����
};

struct ShortPathCode		//�洢���·��������ĵ�ڵ�����ݽṹ 
{
	bool VisitFlag;					//��ʼ���б߶�û�б����ʹ�
	int NumofCode;					//�������·���бߵ�������n0 ~ nk
	int dist;						//��¼�����·����Ȩֵ
	ShortPathEdge *first;			//ָ������·���ĵ�һ����
};

struct ShortPath			//�洢���·��������
{
	ShortPathCode Short_Paht_Code[3000];	//�洢��ڵ������
	int NumofShortestPath;					//�洢���·������Ŀ
};

#endif
