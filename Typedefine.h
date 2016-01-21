#ifndef TYPE_H
#define TYPE_H

#include "fstream.h"
#include "iomanip.h"
#include "stdafx.h"

struct LineNodes // 每段路的数据结构
{
	double S_lon;
	double S_lat;
	double E_lon;
	double E_lat;
	double S_alt;
	double E_alt;
	double length;
	double width;					//道路宽度
	int code;
	int Max_Edge_Cap;				//存储边的最大容纳力
	int Available_Edge_Cap;			//当前时刻可用的容纳力即该道路当前Δt内可以允许多少人进入道路。
	double WalkTime;				//行走所需要的时间
	int NumofTimeQuantum;			//通过该条道路所需要的Δt的数量
};

struct Vertexs
{
	double lon;
	double lat;
	int code;
	double alt;						//存储海拔
	int Max_Code_Cap;				//存储点的最大容纳力
	int Available_Code_Cap;			//当前时刻，点的可用容纳力
	int StartOrEnd;					//待疏散起点、避难所标志  待疏散起点为0，避难所终点为2，其他点为1
};

struct structNodes					// 链表每个边节点的数据结构
{
	int num;						// 此边所指向的点的序号
	LineNodes _LineNodes;			// 边的结构
	structNodes *next;				// 指向下一条边的指针
};

//structNodes AdjList[200];
struct vLineNodes					// 邻接表中点节点的结构
{
	Vertexs data;					//  点的结构
	structNodes *first;				// 指向边表的指针
};

struct ALGraph						//图的结构
{
	vLineNodes _structNodes[1000];
	int vertexnum;					//图中点的数目	
	int arcnum;						//图中边的数目
};

void gainData(CString filename);					//从文件读取数据
int GetCodeOfArcPointTo(LineNodes temp, int k);
int CaculateVertexofGraph(CString filepath);		//找到路网中的所有结点
BOOL TheVertexIN(double lon, double lat, int sum);
void CreateGraph(ALGraph &s,CString filepath);		//创建图
void printGraph(ALGraph &s,CString filepath);		//输出图


struct ShortPathEdge 
{
	Vertexs *data;			//存储最短路径中点的信息
	ShortPathEdge *next;			//指向最短路径中的下一个点
};

struct ShortPathCode		//存储最短路径的链表的点节点的数据结构 
{
	bool VisitFlag;					//开始所有边都没有被访问过
	int NumofCode;					//该条最短路径中边的条数，n0 ~ nk
	int dist;						//记录该最短路径的权值
	ShortPathEdge *first;			//指向该最短路径的第一个点
};

struct ShortPath			//存储最短路径的链表
{
	ShortPathCode Short_Paht_Code[3000];	//存储点节点的数组
	int NumofShortestPath;					//存储最短路径的数目
};

#endif
