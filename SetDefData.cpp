// SetDefData.cpp : implementation file
//

#include "stdafx.h"
#include "GraduDesign.h"
#include "SetDefData.h"
#include "Floor.h"
#include "Shelter.h"
#include "fstream.h"
#include "Typedefine.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PRECISION_Start_End 0.000001			//用于求待疏散起点和避难所在哪条道路上
#define PRECISION_Flood 0.0002					//用于求待疏散起点和避难所所在地是否被淹没
#define OlderSpeed 0.8							//老人步行疏散的速度0.8m/s
#define MAX_NUMOFROAD 1000						//道路段数，最多限定为1000条
#define INF 1000000								//将1000000定义为无穷大
#define FLOODPRECISION 0.0002					//用于判断淹没的经纬度误差范围，经纬度误差小于0.0002，即可认为是同一个点
#define TIMEQUANTUM 180							//Δt的大小，此处设定为180秒
#define RESIDENT 0
#define SCHOOL 1
#define SHOP 2
#define PARK 3									//不同功楼房的代码
#define HOSPITAL 4	
#define HOTEL 5
#define FACTORY 6
#define WORKPLACE 7
/////////////////////////////////////////////////////////////////////////////
// CSetDefData dialog

ofstream f;
CFloor *Floor;							//声明指向楼房对象数组的指针
CShelter *Shelter;						//声明指向避难所对象数组的指针
int FloorRows = 1000;					//记录楼房数据文件的行数
int ShelterRows = 1000;					//记录避难所文件的行数
int WaterDepRows = 100;					//记录水深数据文件的行数
Point * PointArry;						//声明指向水深结构体数组的指针
extern ALGraph _Graph;					//引用全局变量 _Graph
extern int NUMOFELEMENT;				//引用全局变量，路网中边的数量
bool ReadFloorFlag = false,ReadShelterFlag = false,ReadWaterFlag = true;      //标记是否读入楼房和避难所信息
ShortPath ShortestPath;					//声明存储最短路径的链表
int ShortPathCount = 0;					//用于计数

CSetDefData::CSetDefData(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDefData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDefData)
	m_factory = 0.0;
	m_hospital = 0.0;
	m_hotel = 0.0;
	m_park = 0.0;
	m_resident = 0.0;
	m_school = 0.0;
	m_shop = 0.0;
	m_work = 0.0;
	//}}AFX_DATA_INIT
}


void CSetDefData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDefData)
	DDX_Control(pDX, IDC_BTN_ReadRoadData, m_ReadRoadBTN);
	DDX_Control(pDX, IDC_RADIONight, m_RadioNight);
	DDX_Control(pDX, IDC_RADIODay, m_RadioDay);
	DDX_Text(pDX, IDC_EDITFactory, m_factory);
	DDV_MinMaxDouble(pDX, m_factory, 0., 1000.);
	DDX_Text(pDX, IDC_EDITHospital, m_hospital);
	DDV_MinMaxDouble(pDX, m_hospital, 0., 100.);
	DDX_Text(pDX, IDC_EDITHotel, m_hotel);
	DDV_MinMaxDouble(pDX, m_hotel, 0., 10000.);
	DDX_Text(pDX, IDC_EDITPark, m_park);
	DDV_MinMaxDouble(pDX, m_park, 0., 10000.);
	DDX_Text(pDX, IDC_EDITResident, m_resident);
	DDV_MinMaxDouble(pDX, m_resident, 0., 1000.);
	DDX_Text(pDX, IDC_EDITSchool, m_school);
	DDV_MinMaxDouble(pDX, m_school, 0., 1000.);
	DDX_Text(pDX, IDC_EDITShop, m_shop);
	DDV_MinMaxDouble(pDX, m_shop, 0., 1000.);
	DDX_Text(pDX, IDC_EDITWorkPlace, m_work);
	DDV_MinMaxDouble(pDX, m_work, 0., 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDefData, CDialog)
	//{{AFX_MSG_MAP(CSetDefData)
	ON_BN_CLICKED(IDC_BTN_ReadTsuData, OnBTNReadTsuData)
	ON_BN_CLICKED(IDC_BTN_ReadRoadData, OnBTNReadRoadData)
	ON_BN_CLICKED(IDC_BTN_ReadBuildData, OnBTNReadBuildData)
	ON_BN_CLICKED(IDC_BTN_ReadshelterData, OnBTNReadshelterData)
	ON_BN_CLICKED(IDC_RADIODay, OnRADIODay)
	ON_BN_CLICKED(IDC_RADIONight, OnRADIONight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDefData message handlers

void CSetDefData::OnBTNReadTsuData()  
{
	//读取水深数据文件，用于显示淹没范围
	// TODO: Add your control notification handler code here

	CString filename;//保存路径
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("文本文档(*.txt)|*.txt|数据文件(*.xyz,*.dep,*.dat)|*.xyz;*.dep;*.dat||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();					//选择海啸仿真数据文件，读入经纬度数据
	} 

	CStdioFile  FileIn;
	CFileException except;							//定义异常对象
	
	CString StrTemp;										//临时变量
	
	if(!FileIn.Open(filename,CFile::modeReadWrite,&except))			//打开文件
	{
		switch(except.m_cause){
		case CFileException::fileNotFound:
			MessageBox("文件未找到，请确认文件已经放到指定文件夹下！","错误提示",MB_OKCANCEL);
			break;
		case CFileException::endOfFile:
			MessageBox("文件结束！","错误提示",MB_OKCANCEL);
		default:
			MessageBox("文件读取异常！","错误提示",MB_OKCANCEL);
		}
	}		
	else
	{
		WaterDepRows = 0;
		while (FileIn.ReadString(StrTemp))
		{
			WaterDepRows ++;				//计算水深文件的行数，即计算有多少水深数据点
		}

		PointArry =	(Point *) malloc(sizeof(Point) * WaterDepRows);   		//动态分配用于存储水深信息的空间
		
		int i = 0;					//水深数组下标

		FileIn.SeekToBegin();		//将光标定位到文件开头，以便从头开始读取数据
		while (FileIn.ReadString(StrTemp))
		{
			sscanf(StrTemp,"%lf %lf %lf",&PointArry[i].lon,&PointArry[i].lat,&PointArry[i].dep);   //读取水深数据到内存中
							
		/*	if (i%1000 == 0 && i!= 0 && WaterDepRows > 1000)
			{
				printf("%f,%f,%f\n",PointArry[i].lon, PointArry[i].lat, PointArry[i].dep);
			}*/
			i ++;
		}
		FileIn.Close();

	/*	if (opendlg.DoModal()==IDOK)
		{
			filename=opendlg.GetPathName();			//选择海啸仿真数据文件，读入与经纬度相对应的水深数据，用于显示淹没范围
		} 
		
		ifstream f;									//读入与经纬度相对应的水深数据
		f.open(filename);
		
		for (i = 0; i < WaterDepRows; i++)
		{
			f >> PointArry[i].dep;
		}
		
		f.close();
		printf("%f,%f,%f\n",PointArry[0].dep, PointArry[1].dep, PointArry[2].dep);*/
	}	
	ReadWaterFlag = true;
	if (ReadShelterFlag && ReadFloorFlag)
	{
		m_ReadRoadBTN.EnableWindow(TRUE);		//使“读入路网数据”按钮正常使用
	}
}

void MarkStartEnd()				
{
	//标记待疏散起点，避难所终点在图中的位置

	int j;
	structNodes *p1, *p2;
	int num = 0;     //纪录该点与几条路相交
	for (int i = 0;i < _Graph.vertexnum; i ++)
	{
		_Graph._structNodes[i].data.StartOrEnd = 1;  //标记该点为其他点

		for (j = 0;j < FloorRows; j++)
		{

			if (fabs(_Graph._structNodes[i].data.lon - Floor[j].getLon()) < PRECISION_Start_End)  //经度相同
			{
				if (fabs(_Graph._structNodes[i].data.lat - Floor[j].getLat()) < PRECISION_Start_End)   //纬度相同
				{
					_Graph._structNodes[i].data.StartOrEnd = 0;  //标记该点为起点所在
					_Graph._structNodes[i].data.Max_Code_Cap = Floor[j].getNumOfPeo();   //将起点的待疏散总人数纪录到节点的最大容纳人数变量中
					_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap; 
											//初始时刻，起点剩余的人数等于起点的待疏散总人数
				}
			}

		}

		for (j = 0; j < ShelterRows;j++)
		{
			if (_Graph._structNodes[i].data.StartOrEnd != 0)
			{
			
				if (fabs(_Graph._structNodes[i].data.lon - Shelter[j].getLon()) < PRECISION_Start_End)  //经度相同
				{
			
					if (fabs(_Graph._structNodes[i].data.lat - Shelter[j].getLat()) < PRECISION_Start_End)   //纬度相同
					{
						_Graph._structNodes[i].data.StartOrEnd = 2;  //标记该点为终点所在
						_Graph._structNodes[i].data.Max_Code_Cap = Shelter[j].getCap();   //设置终点最大的可容纳人数为避难所的容纳力
						_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap; 
											//初始时刻，终点的可容纳人数等于终点的最大可容纳人数
					}
				}

			}
		
		}

		if (_Graph._structNodes[i].data.StartOrEnd == 1)
		{
			p1 = _Graph._structNodes[i].first;
			for (p2 = p1; p2 != NULL; p2 = p2->next)
			{
				num ++;
			}
			if (num == 2)
			{
				_Graph._structNodes[i].data.Max_Code_Cap = p1->_LineNodes.Max_Edge_Cap;  //该点与两条路相连，为一条道路的中间点，该点的容纳力等于道路容纳力
				_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap;
			}
			else
			{
				_Graph._structNodes[i].data.Max_Code_Cap =0.7 * TIMEQUANTUM * 0.5 * p1->_LineNodes.width * num / 4;
								//估算道路中的节点的容纳力，也是按照Δt的时间段进行估算的
				_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap;
			}
		}
	}
	
	f.open("C:\\Users\\zxw\\Desktop\\顶点的容纳力信息.txt");
	f << "顶点序号\t" << "顶点最大容纳力\t" << "顶点当前可容纳人数\t" << "起止点标志\n";
		
	for (i = 0;i < _Graph.vertexnum; i ++)
	{

		f << _Graph._structNodes[i].data.code << "\t" << _Graph._structNodes[i].data.Max_Code_Cap << "\t" 
		  << _Graph._structNodes[i].data.Available_Code_Cap << "\t" << _Graph._structNodes[i].data.StartOrEnd << "\n";
		printf("%d ",_Graph._structNodes[i].data.StartOrEnd);
		if (i %10 == 0 && i != 0)
		{
			printf("\n");
		}
	}
	printf("\n");
	f.close();
}


double SpeedCal(double S_alt,double E_alt,double Length)
{
	double slope;				//记录坡度的大小
	double speed = OlderSpeed;
	slope = fabs(S_alt - E_alt)/(Length * 1000) *100;		//计算坡度，求坡度的大小;length * 1000,将千米转换为米，再乘以100，坡度模型要求
	if (slope >= 0 && slope <= 3)
		speed = OlderSpeed;		
	else if (slope > 3 && slope <= 6)
	{
		speed = OlderSpeed * 0.85;
	}
	else if (slope > 6 && slope <= 9)
	{
		speed = OlderSpeed * 0.7;
	}
	else if (slope > 9 && slope <= 12)
	{
		speed = OlderSpeed * 0.55;
	}
	else if (slope > 12 && slope <= 15)				/*根据坡度的大小，设置速度系数，求解实际的疏散速度*/
	{
		speed = OlderSpeed * 0.45;
	}
	else if (slope > 15 && slope <= 18)
	{
		speed = OlderSpeed * 0.4;
	}
	else if (slope > 18 && slope <= 21)
	{
		speed = OlderSpeed * 0.35;
	}
	else if (slope > 21 && slope <= 24)
	{
		speed = OlderSpeed * 0.3;
	}
	else if (slope > 24 && slope <= 27)
	{
		speed = OlderSpeed * 0.25;
	}
	else if (slope > 27 && slope <= 30)
	{
		speed = OlderSpeed * 0.2;
	}
	else if (slope > 30 && slope <= 33)
	{
		speed = OlderSpeed * 0.15;
	}
	else if (slope > 33 && slope <= 36)
	{
		speed = OlderSpeed * 0.14;
	}
	else if (slope > 36 && slope <= 39)
	{
		speed = OlderSpeed * 0.13;
	}
	else if (slope > 39 && slope <= 42)
	{
		speed = OlderSpeed * 0.12;
	}
	else if (slope > 42 && slope <= 45)
	{
		speed = OlderSpeed * 0.11;
	}
	else if (slope > 45)
	{
		speed = OlderSpeed * 0.1;
	}
	return speed;
}


void shortPath(int v1,CString filepath)
{
	//用迪杰斯特拉算法，求解从待疏散的楼房到避难所的最短距离
	f.open("C:\\Users\\zxw\\Desktop\\最短路径（道路编号）.txt",ios::app);
	int i, j, k, count = 0;				//循环变量
	int dist[MAX_NUMOFROAD];			//声明用于记录起点到当前点的权值，dist[i]指从起点直接连接到点vi的权值
	int S[MAX_NUMOFROAD];				//迪杰斯特拉算法中的集合S。用于表示当前点是否在集合S中，1表示在，0表示不在
	int path[MAX_NUMOFROAD];			//用于保存最短路径，path[2] = 1 表示点v2的前一个点为v1
	structNodes *p1, *p2;
	int n = _Graph.vertexnum;			//图中点节点的个数
	int *TempArray = new int[n+1];		//声明一个大小为n的数组，用于保存点 u 和点 k 之间的权值
	bool RunError = false;				//运行错误标志

	p1 = _Graph._structNodes[v1-1].first;    //指向与v1相连的第一个点的边节点的指针
	
	//AfxMessageBox("3");

	for (i = 1; i <= n; i ++)
	{
		TempArray[i] = dist[i] = INF;						//初始设置为最大值，即起点与vi之间不是直接相连的
		for (p2 = p1; p2 != NULL; p2 = p2->next)
		{
			if (i == p2->num)
			{
				dist[i] = p2->_LineNodes.NumofTimeQuantum;		//vi和v1相连接，之间的权值为两点之间的步行时间
				break;
			}
		}

		S[i] = 0;									//初始时，所有点都不在S集合中
		if (i != v1 && dist[i] < INF)
		{
			path[i] = v1;					//dist[i] < INF，表示vi和v1直接相连，所以vi的上一点为v1
		}
		else
		{
			path[i] = -1;
		}
	}

	S[v1] = 1;							//把v1加入集合S中
	dist[v1] = 0;						//到自身的权值为0
	for (i= 1; i <= n - 1; i++)			//从v1确定 n-1 条最短路径
	{
		double min = INF;
		int u = v1;			//min 最小权值，u 临时变量

		for (j = 1; j <= n; j++)		//选择当前集合V-S中具有最短路径的顶点u
		{
			if (!S[j] && dist[j] < min)
			{
				u = j;					/*求非S集合中，权值最小的点*/	
				min = dist[j];
			}
		}
		S[u] = 1;						//将u加入集合S，表示它的最短路径已经求得
		
		p1 = _Graph._structNodes[u-1].first;		//指向与 点u 直接相连接的第一个点的边节点的指针
		
		for (k = 1; k <= n; k ++)		//修改V-S中顶点的dist 和 path 数组元素值
		{
			for (p2 = p1; p2 != NULL; p2 = p2->next)   //求解点 u 和点 k 之间的权值
			{
				if (k == p2->num)
				{
					TempArray[k] = p2->_LineNodes.NumofTimeQuantum;		//vi和v1相连接，之间的权值为两点之间的步行时间
					break;
				}
			}

			if (!S[k] && TempArray[k] < INF && dist[u] + TempArray[k] < dist[k])
			{
				dist[k] = dist[u] + TempArray[k];			//更新最短路径长度
				path[k] = u;								//修改path数组内容
			}
		}
	}

	//AfxMessageBox("4");
	ofstream outfile;
	//CString filename = filepath + "\\最短路径.txt";
	CString filename = "C:\\Users\\zxw\\Desktop\\最短路径.txt";
	//outfile.Open(filename,CStdioFile::modeWrite||CStdioFile::modeCreate||CStdioFile::modeNoTruncate);
	//outfile.SeekToEnd();
	outfile.open(filename,ios::app);
	ShortPathEdge *p3 = new ShortPathEdge;
	
	int *shortest = new int[MAX_NUMOFROAD];				//输出最短路径上的各个顶点时存放各个顶点的序号
	for (i = 1;i <= n; i ++)
	{
		if (i == v1)
		{
			continue;
		}

		if (path[i] != -1)
		{
			if (_Graph._structNodes[i-1].data.StartOrEnd == 2)
			{
				memset(shortest,0,sizeof(int)*MAX_NUMOFROAD);	//将数组shortest中的元素都置为0
				k = 0;
				shortest[k] = i;						//k表示shortest数组中最后一个元素的下标
				while (path[shortest[k]] != v1)			//存疑问
				{
					k ++;
					shortest[k] = path[shortest[k-1]];	//将最短路径的点的序号存入数组shortest中
				}
				k ++;
				shortest[k] = v1;						//将起点存入数组
				ShortPathCount ++;						//编号加1，存储下一条最短路径
				p3 = ShortestPath.Short_Paht_Code[ShortPathCount].first = NULL;
				for (j = k; j >0; j --)
				{
					outfile << setiosflags(ios::fixed) << setprecision(6) << _Graph._structNodes[shortest[j] - 1].data.lon << "\t";
					outfile << setiosflags(ios::fixed) << setprecision(6) << _Graph._structNodes[shortest[j] - 1].data.lat << "\t";
					outfile << "0" << "\t";				//输出海拔
				//	printf("%d->",shortest[j]);			//输出最短路径
					f << shortest[j] << "->";
				
					if (ShortestPath.Short_Paht_Code[ShortPathCount].first == NULL)  //当前存储的是第一个点
					{
						ShortestPath.Short_Paht_Code[ShortPathCount].first = new ShortPathEdge;
						ShortestPath.Short_Paht_Code[ShortPathCount].first->data = &(_Graph._structNodes[shortest[j] - 1].data);
						ShortestPath.Short_Paht_Code[ShortPathCount].first->next = NULL;
						p3 = ShortestPath.Short_Paht_Code[ShortPathCount].first;
					}
					else
					{
						ShortPathEdge *p4 = new ShortPathEdge;
						p4->data = &(_Graph._structNodes[shortest[j] - 1].data);
															//将最短路径中点，逐渐添加到链表中
						p3->next = p4;
						p3 = p4;
						p4->next = NULL;
					}
				
					if (j == k)
					{
						outfile  << "0" << "\t" << setiosflags(ios::fixed) << setprecision(6) << dist[i] << "\n";
						ShortestPath.Short_Paht_Code[ShortPathCount].dist = dist[i];	//路权存入最短路径中
						ShortestPath.Short_Paht_Code[ShortPathCount].NumofCode = k + 1; //shortest数组刚好为k+1
						ShortestPath.Short_Paht_Code[ShortPathCount].VisitFlag = false;
					}
					else
					{
						outfile << "1" << "\n";
					}
				}

				ShortPathEdge *p5 = new ShortPathEdge;
				p5->data = &(_Graph._structNodes[shortest[0] - 1].data);         //最短路径中的最后一个点

				p3->next = p5;						//将最短路径中最后一个点添加到链表中
				p3 = p5;
				p5->next = NULL;

				outfile << setiosflags(ios::fixed) << setprecision(6) << _Graph._structNodes[shortest[0] - 1].data.lon << "\t";
				outfile << setiosflags(ios::fixed) << setprecision(6) <<_Graph._structNodes[shortest[0] - 1].data.lat << "\t" << "0" << "\t" << "2" << "\n";
				f << shortest[0] <<"\t"<<setiosflags(ios::fixed) << setprecision(6) <<dist[i]  << endl;
				//printf("%d\t",shortest[0]); 
				//printf("%d\n",dist[i]);					//输出顶点v1到顶点i的最短路径长度
			}
		}
		else if(path[i] == -1)
		{
			AfxMessageBox("可能存在以下问题：\n1、导入的路网数据错误。部分楼房或避难所的经纬度不在路网数据中。请将楼房或避难所的经纬度修正，使其数据处在路网数据中！\n2、路网数据所代表的路网，不是互相连通的！");
			
			exit(0);
		}
		
	}	
	outfile.close();
	f.close();
}

void FindRoad(ALGraph &s,CString filepath)				
{
	//自动生成所有路径，从待疏散点到避难所
	MarkStartEnd();
	//int *FLagOfSpeed = new int[s.arcnum];			//0表示该边的疏散时间还没有计算，1表示已经计算完成
	//memset(FLagOfSpeed,0,sizeof(int)*s.arcnum);		//将数组元素全部设为0
	double speed = OlderSpeed;			//设置人群默认的疏散速度
	structNodes *p1, *p2;
	int j;
	f.open("C:\\Users\\zxw\\Desktop\\各边权值.txt");
	f << "道路编号\t" << "道路步行时间\t" << "道路步行花费时间段数量\t" << "边的最大容纳力\n";
	for (int i = 0; i < _Graph.vertexnum; i ++)
	{
		p1 = s._structNodes[i].first;					/*找到邻接表中点节点所指向的第一个相邻的边节点的指针
														即找到以当前点节点为起点的第一条边的指针*/
		for (p2 = p1; p2 != NULL;p2 = p2->next)
		{
			//AfxMessageBox("5");
			//if (FLagOfSpeed[p2->_LineNodes.code-1] == 0)				//0表示该边的疏散时间没有被计算
			//{
			//	FLagOfSpeed[p2->_LineNodes.code-1] = 1;					//将标志置为1，表示该边的疏散时间已经被计算
				speed = SpeedCal(p2->_LineNodes.S_alt,p2->_LineNodes.E_alt,p2->_LineNodes.length);
																		//上一语句表示求解不同路段由于坡度不同造成的疏散速度不同
																		//计算每段路所需要的最长行走时间，按照“0.8m/s*坡度速度影响因子”的速度计算。0.8m/s为老人速度
				p2->_LineNodes.WalkTime = p2->_LineNodes.length * 1000 / speed;  //计算该条道路疏散所需要的时间。乘以1000，是把距离单位千米转换为米
				p2->_LineNodes.NumofTimeQuantum = 1 + p2->_LineNodes.WalkTime / TIMEQUANTUM;  //计算该条道路疏散所需要的Δt的数量
				p2->_LineNodes.Max_Edge_Cap = 0.7 * (speed * TIMEQUANTUM * p2->_LineNodes.width) / 1.5; 
																//0.7为比例因子，即道路总面积的70%为人占用。Δt时间内，道路面积为速度乘以时间再乘以道路宽度。除以 1.5 是指，人均占用面积为1.5平方米
				p2->_LineNodes.Available_Edge_Cap = p2->_LineNodes.Max_Edge_Cap;
				f << p2->_LineNodes.code  << "\t" << p2->_LineNodes.WalkTime << "\t" <<p2->_LineNodes.NumofTimeQuantum 
				  << "\t" << p2->_LineNodes.Max_Edge_Cap << endl;
			//}
			
		}
		//AfxMessageBox("1");
										
	/*	if (s._structNodes[i].data.StartOrEnd != 1)
		{
		
			switch (s._structNodes[i].data.StartOrEnd)
			{
			case 0:   //当该点为待疏散起点时
				 for (j = 0;j < WaterDepRows;j++)
				 {
					if (s._structNodes[i].data.StartOrEnd == 1)
							break;
					if (fabs(s._structNodes[i].data.lon - PointArry[j].lon) <= FLOODPRECISION)	//经度相同
					{
						if (fabs(s._structNodes[i].data.lat - PointArry[j].lat) <= FLOODPRECISION)	//纬度相同
						{
							if (PointArry[j].dep < s._structNodes[i].data.alt)	
									//上一句表示：当水深海拔小于楼房海拔时，即水没有淹没楼房时，楼房无需进行疏散，将该点的起点标志清除，设置为一般点
							{
								s._structNodes[i].data.StartOrEnd = 1;			//将该点设置为一般点
								//AfxMessageBox("0");
							}
						}
					}
				 }	 
				 break;
			case 2:			//当该点为避难所终点时
				//AfxMessageBox("5");
				for (j = 0;j < WaterDepRows;j++)
					 {
						 if (s._structNodes[i].data.StartOrEnd == 1)
							break;
						 if (fabs(s._structNodes[i].data.lon - PointArry[j].lon) <= FLOODPRECISION)	//经度相同
						 {
							 if (fabs(s._structNodes[i].data.lat - PointArry[j].lat) <= FLOODPRECISION)	//纬度相同
							 {
								 if (PointArry[j].dep >= s._structNodes[i].data.alt)	
									 //上一句表示：当水深海拔大于等于避难所海拔时，即水淹没避难所时，该避难所将不能用来疏散，将该点的终点标志清除，设置为一般点
								 {
									 s._structNodes[i].data.StartOrEnd = 1;			//将该点设置为一般点
									 //AfxMessageBox("1");
								 }
							 }
						 }
					}	
					 break;
			default:
				AfxMessageBox("出现错误，请重新启动！");

			}	
		} */
	}

	f.close();

	ShortPathCount = 0;
	//shortPath(1);
	for ( i = 0;i < _Graph.vertexnum;i ++)
	{
		if (_Graph._structNodes[i].data.StartOrEnd == 0)				//如果该点为待疏散起始点，则求以该点为起点的最短路劲
		{
			//AfxMessageBox("2");
			shortPath(_Graph._structNodes[i].data.code,filepath);
		}
		//AfxMessageBox("3");
	}

}

void quickSort(int low,int high)					//为最短路径排序，快速排序算法
{
	int pivot;
	int i =low, j = high;
	if (low >= high) return;
	pivot = ShortestPath.Short_Paht_Code[low].dist;
	while (low < high)
	{
		while (low < high && ShortestPath.Short_Paht_Code[high].dist >= pivot)
		{
			high --;
		}
		ShortestPath.Short_Paht_Code[low] = ShortestPath.Short_Paht_Code[high];
		while (low < high && ShortestPath.Short_Paht_Code[low].dist <= pivot)
		{
			low ++;
		}	
		ShortestPath.Short_Paht_Code[high] = ShortestPath.Short_Paht_Code[low];
	}
	ShortestPath.Short_Paht_Code[low].dist = pivot;
	
	quickSort(i, low-1); 
	quickSort(low+1, j); 
}


void bubblesort(int n)  //冒泡排序
{
	double comp = 0,move = 0;
	int i,LastExchange,j;
	ShortPathCode t;
	i = n; 
    while(i > 1) 
	{
		LastExchange = 1; 
		for(j = 1;j < i; j++)
		{ 
			if(ShortestPath.Short_Paht_Code[j+1].dist < ShortestPath.Short_Paht_Code[j].dist)
			{ 
				t=ShortestPath.Short_Paht_Code[j];
				ShortestPath.Short_Paht_Code[j]=ShortestPath.Short_Paht_Code[j+1];
				ShortestPath.Short_Paht_Code[j+1]=t;
				move = move+3;
				LastExchange = j;
			} 
			comp ++;
		}
		i = LastExchange; 
  }
}

bool People_exist_flag()							//检查起点是否有人未被疏散
{
	bool PeopleFlag = false;
	for (int i = 0;i < _Graph.vertexnum; i ++)
	{
		if(_Graph._structNodes[i].data.StartOrEnd == 0)
		{
			if (_Graph._structNodes[i].data.Available_Code_Cap > 0)
			{
				PeopleFlag = true;
				break;
			}
			
		}			
	}
	if (!PeopleFlag)
	{
		printf("%d  ",PeopleFlag);
	}

	return PeopleFlag;
}


int Min_Capacity_Road(int i)
{
	int flow = 0;
	ShortPathEdge *p1,*p2;
	structNodes *p3,*p4;
	p1 = ShortestPath.Short_Paht_Code[i].first;
	flow = p1->data->Available_Code_Cap;
	for (p2 = p1;p2->next != NULL;p2 = p2->next)
	{
		if (flow > p2->next->data->Available_Code_Cap)		//如果前一个点的可用容纳力小于后一个点，则将flow的值更新
		{
			flow = p2->next->data->Available_Code_Cap;			
		}
		p3 = _Graph._structNodes[p2->data->code - 1].first;

		if (p3->next ==NULL)
		{
			if (p3->num == p2->next->data->code)		/*如果p2所指向的节点的下一个节点的编号等于
								与p3所指向的边节点相连的边的另一头的节点编号，那么这条边就是最短路径中连接两点的边*/
			{
				if (flow > p3->_LineNodes.Available_Edge_Cap)	//如果此时的最小值flow 小于 两点之间的边的可用容纳力，更新flow
				{
					flow = p3->_LineNodes.Available_Edge_Cap;
				}
			}
		}
		for (p4 = p3; p4->next != NULL;p4 = p4->next)
		{
			if (p4->num == p2->next->data->code)		/*如果p2所指向的节点的下一个节点的编号等于
								与p4所指向的边节点相连的边的另一头的节点编号，那么这条边就是最短路径中连接两点的边*/
			{
				if (flow > p4->_LineNodes.Available_Edge_Cap)	//如果此时的最小值flow 小于 两点之间的边的可用容纳力，更新flow
				{
					flow = p4->_LineNodes.Available_Edge_Cap;
				}
			}
		}
	}

	return flow;
}

void  PopAllocation()
{
	int TimeCount = 0;								//初始时刻为t0时刻
	ShortPathEdge *p1,*p2;							//临时变量
	structNodes *p3,*p4;							//临时变量
	bool RoadFlag = true;							//当前最短路径是否可用的标志。不可用代表着有人群在该条道路上疏散
	int flow = 0;									//表示某条最短路径中，起点剩余人数，终点剩余可容纳人数，中间点和边的剩余可容纳人数中的最小值
	int timeSpent = 0;								//表示某条最短路径的时间花费，用于记录时间序列
	int tempFlag = 1;								//临时标志
	int i,j;
	int *Ava = new int[ShortPathCount];				//记录道路的利用率，即利用了几次

	f.open("C:\\Users\\zxw\\Desktop\\未排序之前的最短路径.txt");
	f << "道路权值\t" << "路径编号\n";
	
	for (int lk = 1; lk <= ShortPathCount;lk++)
	{
		Ava [lk - 1] = 0;
		f << ShortestPath.Short_Paht_Code[lk].dist << "\t";
		p1 = ShortestPath.Short_Paht_Code[lk].first;
		f << p1->data->code;
		for (p2 = p1;p2->next != NULL;p2 = p2->next)
		{
			f << "->";
			f << p2->next->data->code;
		}
		f << endl;
	}
	f.close();
	

	//quickSort(1,ShortPathCount);					//对所有的最短路径，按照权值由小到大进行排序
	bubblesort(ShortPathCount);

	f.open("C:\\Users\\zxw\\Desktop\\排序后的最短路径.txt");
	f << "道路权值\t" << "路径编号\n";

	for (lk = 1; lk <= ShortPathCount;lk++)
	{
		f << ShortestPath.Short_Paht_Code[lk].dist << "\t";
		p1 = ShortestPath.Short_Paht_Code[lk].first;
		f << p1->data->code;
		for (p2 = p1;p2->next != NULL;p2 = p2->next)
		{
			f << "->";
			f << p2->next->data->code;
		}
		f << endl;
	}
	f.close();
	
	f.open("C:\\Users\\zxw\\Desktop\\人群疏散数据随时间变化.txt",ios::app);
	f << "起始时刻\t" << "起点经度\t" << "起点纬度\t" << "起点编号\t" << "输送人数\t" << "花费时间\t" << "剩余未疏散人数\t" << "边和时间的序列\n";
	while (People_exist_flag())						//当待疏散点有人需要疏散时，该算法就一直执行，直到所有人被成功疏散
	{
		printf("TimeCount = %d ",TimeCount);
		for (j = 0; j < _Graph.vertexnum; j++)
		{
			if (_Graph._structNodes[j].data.StartOrEnd == 1)
			{
				_Graph._structNodes[j].data.Available_Code_Cap = _Graph._structNodes[j].data.Max_Code_Cap;
				//一个新的时刻到来后，中间点剩余的可容纳人数等于最大的容纳人数
			}
			p3 = _Graph._structNodes[j].first;
			p3->_LineNodes.Available_Edge_Cap = p3->_LineNodes.Max_Edge_Cap;
			for (p4 = p3; p4->next != NULL; p4 = p4->next)
			{
				p4->next->_LineNodes.Available_Edge_Cap = p4->next->_LineNodes.Max_Edge_Cap;
				//一个新的时刻到来后，各条边的剩余的可容纳人数等于最大的容纳人数
			}
		}

		//AfxMessageBox("4");
		for (i = 1; i <= ShortPathCount; i++)
		{	
			RoadFlag =true;
			//AfxMessageBox("1");	
		/*	if (ShortestPath.Short_Paht_Code[i].VisitFlag == true)
			{
				//AfxMessageBox("6");
				continue;							//如果该条最短路已经被访问过（即当前时刻有人从该条道路上疏散），则继续寻找下一条最短路
			}
			else
			{*/
		//		ShortestPath.Short_Paht_Code[i].VisitFlag = true;	//找到了一条时间花费最少的路径，而且没有被访问过，该路径的编号为 i 

			p1 = ShortestPath.Short_Paht_Code[i].first;		//p1指向第i条最短路径的第一个节点
			if (p1->data->Available_Code_Cap <= 0)		//最短路径中的该点的可容纳人数小于0，表示当前该路正在疏散人群，且道路完全被人群占用
			{
				RoadFlag = false;						//表示该条最短路径中的点正在被使用，不能用来疏散
			}
			for (p2 = p1;p2->next != NULL;p2 = p2->next)
			{
				if (p2->next->data->Available_Code_Cap <= 0)		//最短路径中的该点的可容纳人数小于0，表示当前该路正在疏散人群，且道路完全被人群占用
				{
					
					RoadFlag = false;						//表示该条最短路径中的点正在被使用，不能用来疏散
					break;
				}
				
				p3 = _Graph._structNodes[p2->data->code - 1].first;
				
			
				if (p3->num == p2->next->data->code)					/*如果p2所指向的节点的下一个节点的编号等于
					与p3所指向的节点相连的边的另一头的节点编号，那么这条边就是最短路径中连接两点的边,判断该边的可容纳人数即可判断该边是否可用*/
				{
					if (p3->_LineNodes.Available_Edge_Cap <= 0)
					{
						RoadFlag = false;			//表示该条最短路径中的边正在被使用，不能用来疏散
					}
					
				}
				
				for (p4 = p3;p4->next != NULL; p4 = p4->next)			//判断最短路径中边的可容纳人数是否大于0，即判断改边是否可用
				{
				if (p4->next->num == p2->next->data->code)					/*如果p2所指向的节点的下一个节点的编号等于
					与p2所指向的节点相连的边的另一头的节点编号，那么这条边就是最短路径中连接两点的边,判断该边的可容纳人数即可判断该边是否可用*/
				{
					if (p4->next->_LineNodes.Available_Edge_Cap <= 0)
					{
						RoadFlag = false;			//表示该条最短路径中的边正在被使用，不能用来疏散
						break;
					}
					
				}
				}
			}
			
			if (RoadFlag)
			{
				flow = Min_Capacity_Road(i);
				
				timeSpent = TimeCount;								//时间序列的初始值
				p1 = ShortestPath.Short_Paht_Code[i].first;
				f << "T" << TimeCount << "\t" << p1->data->lon << "\t" << p1->data->lat << "\t" << p1->data->code <<"\t";
				f << flow << "\t" << ShortestPath.Short_Paht_Code[i].dist << "\t" << p1->data->Available_Code_Cap - flow << "\t";
				
				printf("T%d, %f, %f, %d, %d, %d, %d\n",TimeCount,p1->data->lon,p1->data->lat,p1->data->code,flow,ShortestPath.Short_Paht_Code[i].dist,p1->data->Available_Code_Cap - flow);
				
				p1->data->Available_Code_Cap -= flow;			//边的起点消减flow
				f << p1->data->code <<"(T" << timeSpent << ")";
				
				printf("%d(T%d)",p1->data->code,timeSpent);
				
				for (p2 = p1;p2->next != NULL;p2 = p2->next)
				{
					//	AfxMessageBox("3");
					
					p2->next->data->Available_Code_Cap -= flow;		//边的终点消减flow
					
					p3 = _Graph._structNodes[p2->data->code - 1].first;
					
					
					if (p3->num == p2->next->data->code)		/*如果p2所指向的节点的下一个节点的编号等于
						与p3所指向的边节点相连的边的另一头的节点编号，那么这条边就是最短路径中连接两点的边*/
					{
						p3->_LineNodes.Available_Edge_Cap -= flow;  //该条边消减flow
						f << "->";
						f << p2->next->data->code <<"(T" << p3->_LineNodes.NumofTimeQuantum + timeSpent << ")";
						
						printf("->%d(T%d)",p2->next->data->code,p3->_LineNodes.NumofTimeQuantum + timeSpent);
						timeSpent += p3->_LineNodes.NumofTimeQuantum;
					}	
					
					for (p4 = p3; p4->next != NULL;p4 = p4->next)
					{
					if (p4->next->num == p2->next->data->code)		/*如果p2所指向的节点的下一个节点的编号等于
						与p4所指向的节点相连的边的另一头的节点编号，那么这条边就是最短路径中连接两点的边*/
					{
						p4->next->_LineNodes.Available_Edge_Cap -= flow;  //该条边消减flow
						f << "->";
						f << p2->next->data->code <<"(T" << p4->next->_LineNodes.NumofTimeQuantum + timeSpent << ")";
						
						printf("->%d(T%d)",p2->next->data->code,p4->next->_LineNodes.NumofTimeQuantum + timeSpent);
						timeSpent += p4->next->_LineNodes.NumofTimeQuantum;
					}	
					}
					
				}
				printf("\n");
				f << "\n";
			}
			
			//}//else
			
			printf("i = %d   ",i);
			
		}//for	

		TimeCount ++;				//时间计数加1，由上一时刻进入下一时刻

		/*for (int tempCount = 1;	tempCount <= ShortPathCount; tempCount++)
		{
			ShortestPath.Short_Paht_Code[tempCount].VisitFlag = false;
			//AfxMessageBox("5");
		}*/
		tempFlag ++;
	
	}//while
	f.close();
	AfxMessageBox("疏散结束！");
}

void CSetDefData::OnBTNReadRoadData() 
{
	//读取路网数据，用于求解最短路径
	// TODO: Add your control notification handler code here

	CString filename;				//保存文件名
	CString filepath;				//保存文件路径
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("文本文档(*.txt)|*.txt||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filepath = filename = opendlg.GetPathName();					//选择路网数据文件，读入相关数据，用于求解最短路径
		filepath = filepath.Left(filepath.ReverseFind('\\'));					//获取文件所在目录
		//printf("%s,%s",filename,filepath);
	
		gainData(filename);					//获取路网数据
		CreateGraph(_Graph,filepath);		//生成路网关系
		printGraph(_Graph,filepath);		//打印路网关系
		FindRoad(_Graph,filepath);			//求最短路径
		PopAllocation();
	} 
	else
	{
		AfxMessageBox("未选择任何文件！");
	}

}

void CSetDefData::PopDistribution()
{
	//估算人口分布情况
	int BuildType = 0;							//设置楼房功能，默认为0，居民区
	double AreaAverage[9] = {32.9,4,3.8,42,12,6,80,8.5,9};
	//设置各种功能楼房的默认人均面积
	double AreaRadio[9] = {0.63,0.4,0.43,0.5,0.43,0.43,0.43,0.43,0.6};
	//存储不同功能楼房占所标记区域的总面积的比例，即有多大比例的面积是实际的楼房面积
	double CapacityRadio[2][9] = {{0.5,1.1,0.35,0.6,0.7,0.5,1,1,0.6},{1,0,0,0,0.5,0.8,0,0,0}};
	//存储不同功能楼房的占用比率，即有多大比例的面积被人占用
	int DayOrNight = 1;							//“0”表示白天，“1”表示晚上
	int NumOfPeo = 0;							//存储人口数量的临时变量
	
	if(this->m_factory > 0)
		AreaAverage[FACTORY] = this->m_factory;
	if(this->m_hospital > 0)
		AreaAverage[HOSPITAL] = this->m_hospital;
	if(this->m_hotel > 0)						//针对各种功能类型的房屋输
		AreaAverage[HOTEL] = this->m_hotel;					//入其人均占用面积
	if(this->m_park > 0)
		AreaAverage[PARK] = this->m_park;
	if(this->m_school > 0)
		AreaAverage[SCHOOL] = this->m_school;
	if(this->m_resident > 0)
		AreaAverage[RESIDENT] = this->m_resident;
	if(this->m_shop > 0)
		AreaAverage[SHOP] = this->m_shop;
	if(this->m_work > 0)
		AreaAverage[WORKPLACE] = this->m_work;
	
	if (m_RadioDay.GetCheck() == 1 && m_RadioNight.GetCheck() == 0)
	{
		DayOrNight = 0;						//设置为0，表示当前选择为白天模式
	}
	else if (m_RadioDay.GetCheck() == 0 && m_RadioNight.GetCheck() == 1)
	{
		DayOrNight = 1;						//设置为1，表示当前选择为夜晚模式
	}
	

	f.open("C:\\Users\\zxw\\Desktop\\楼房人口数据.txt");
	f << "经度\t" << "纬度\t" << "面积\t" << "人数\t" << "楼房类型\n";
	for (int i = 0; i< FloorRows; i++)
	{
		BuildType = Floor[i].getType();
		NumOfPeo = ((1000000 * Floor[i].getArea()*AreaRadio[BuildType])/AreaAverage[BuildType])*CapacityRadio[DayOrNight][BuildType]*(Floor[i].getHight()/2.8);
		//乘以1 000 000是为了把平方千米转换为平方米。计算人口总数：某地区人口数=(某地区总面积*楼房所占面积比例)/人均面积×白天/晚上的系数×楼房高度/每层高度
		Floor[i].setNumOfPeo(NumOfPeo);			//记录每个区域估计出的人口数量
		
		if (i%10 == 0 && i != 0)
		{
			
			printf("\n");
		}
		printf("%d   ",Floor[i].getNumOfPeo());
		f<< setiosflags(ios::fixed) << setprecision(6)  << Floor[i].getLon() << "\t"; 
		f<< setiosflags(ios::fixed) << setprecision(6)	<< Floor[i].getLat() << "\t";
		f<< setiosflags(ios::fixed) << setprecision(6)	<< Floor[i].getArea() << "\t"; 
		f<< setiosflags(ios::fixed) << setprecision(6)	<< Floor[i].getNumOfPeo() <<"\t";
		f<< Floor[i].getType();
		f << endl;
		
	}
	f.close();
}


void CSetDefData::OnBTNReadBuildData() 
{
	//读取房屋信息，用于估计人口分布
	// TODO: Add your control notification handler code here
	CString filename;//保存路径
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("文本文档(*.txt)|*.txt||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();					//选择房屋数据文件，读入相关数据，用于估计人口分布
	} 

	CStdioFile  FileIn;
	CFileException except;							//定义异常对象

	CString StrTemp;

	if(!FileIn.Open(filename,CFile::modeReadWrite,&except))			//打开文件
	{
		switch(except.m_cause){
		case CFileException::fileNotFound:
			MessageBox("文件未找到，请确认文件已经放到指定文件夹下！","错误提示",MB_OKCANCEL);
			break;
		case CFileException::endOfFile:
			MessageBox("文件结束！","错误提示",MB_OKCANCEL);
		default:
			MessageBox("文件读取异常！","错误提示",MB_OKCANCEL);
		}
	}		
	else
	{
		FloorRows = 0;
		while (FileIn.ReadString(StrTemp))
		{
			FloorRows ++;				//计算楼房文件的行数，即计算有多少疏散点等待疏散
		}

		if (FloorRows == 0)
		{
			FloorRows = 1000;
		}
		
		Floor = (CFloor*) malloc(sizeof(CFloor) * FloorRows);   		//动态分配用于存储楼房信息的空间
							
		double lon = 0;					//楼房经度
		double lat = 0;					//楼房纬度
		double alt = 0;					//楼房海拔高度
		double area = 0;				//楼房面积
		double hight = 0;				//楼房高度
		int type = 0;					//楼房功能分类   0:居民区 1:学校 2:商场 3:公园 4:医院 5:宾馆 6:工厂 7:办公场所 8:风景区
		
		int i = 0;					//楼房数组的下标
		FileIn.SeekToBegin();					//将光标定位到文件开头，以便读取文件
		while (FileIn.ReadString(StrTemp))			//读取楼房数据到内存中
		{
			sscanf(StrTemp,"%lf %lf %lf %lf %lf %d",&lon,&lat,&alt,&area,&hight,&type);
			Floor[i].setLon(lon);
			Floor[i].setLat(lat);			//分别将经纬度，海拔高度，楼房高度，楼房面积等存入内存
			Floor[i].setAlt(alt);
			Floor[i].setArea(area);
			Floor[i].setHight(hight);
			Floor[i].setType(type);
			i ++;
		}
				
		PopDistribution();
		FileIn.Close();
	}	

	ReadFloorFlag = true;			//设定读入楼房信息标记为TRUE
	if (ReadShelterFlag && ReadWaterFlag)
	{
		m_ReadRoadBTN.EnableWindow(TRUE);		//使“读入路网数据”按钮正常使用
	}
}

void CSetDefData::OnBTNReadshelterData() 
{
	//读取避难所数据，用于人群疏散
	// TODO: Add your control notification handler code here
	CString filename;//保存路径
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("文本文档(*.txt)|*.txt||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();					//选择避难所数据文件，读入相关数据，用于人群疏散
	} 

	CStdioFile  FileIn;
	CFileException except;							//定义异常对象
	
	CString StrTemp;
	
	if(!FileIn.Open(filename,CFile::modeReadWrite,&except))			//打开文件
	{
		switch(except.m_cause){
		case CFileException::fileNotFound:
			MessageBox("文件未找到，请确认文件已经放到指定文件夹下！","错误提示",MB_OKCANCEL);
			break;
		case CFileException::endOfFile:
			MessageBox("文件结束！","错误提示",MB_OKCANCEL);
		default:
			MessageBox("文件读取异常！","错误提示",MB_OKCANCEL);
		}
	}		
	else
	{
		ShelterRows = 0;
		while (FileIn.ReadString(StrTemp))
		{
			ShelterRows ++;				//计算避难所文件的行数，即计算有多少避难所可以用来疏散
		}

		if (ShelterRows == 0)
		{
			ShelterRows = 100;
		}
		
		Shelter = (CShelter*) malloc(sizeof(CShelter) * ShelterRows);   		//动态分配用于存储避难所信息的空间
					
		double lon = 0;					//避难所经度
		double lat = 0;					//避难所纬度
		double alt = 0;					//避难所海拔高度
		int capacity = 0;				//避难所高度
		int type = 0;					//避难所功能分类   0:居民区 1:学校 2:商场 3:公园 4:医院 5:宾馆 6:工厂 7:办公场所 8:风景区 9:博物馆，大厅
		
		int i = 0;					//楼房数组的下标
		FileIn.SeekToBegin();           //将光标定位文件开头，以便读取文件内容
		while(FileIn.ReadString(StrTemp))		//读取避难所数据到内存中
		{
			sscanf(StrTemp,"%lf %lf %lf %d %d",&lon,&lat,&alt,&capacity,&type);
			Shelter[i].setLon(lon);
			Shelter[i].setLat(lat);			//分别将经纬度，海拔高度，避难所容纳力，避难所功能等存入内存
			Shelter[i].setAlt(alt);
			Shelter[i].setCap(capacity);
			Shelter[i].setType(type);
			i ++;
			printf("%lf,%lf,%lf,%d,%d\n",lon,lat,alt,capacity,type);
		}	
		FileIn.Close();
	}
	
	ReadShelterFlag = true;			//设定读入避难所信息标记为TRUE，当此标志为TRUE而且ReadFloorFlag也为true时，“读入路网数据”按钮可正常使用
	if (ReadFloorFlag && ReadWaterFlag)
	{
		m_ReadRoadBTN.EnableWindow(TRUE);		//使“读入路网数据”按钮正常使用
	}
}

void CSetDefData::OnRADIODay() 
{
	//点击白天选项时的响应函数
	// TODO: Add your control notification handler code here
	m_RadioNight.SetCheck(FALSE);
}

void CSetDefData::OnRADIONight() 
{
	//点击晚上选项时的响应函数
	// TODO: Add your control notification handler code here
	m_RadioDay.SetCheck(FALSE);
}
