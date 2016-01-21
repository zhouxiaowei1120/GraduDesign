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

#define PRECISION_Start_End 0.000001			//���������ɢ���ͱ�������������·��
#define PRECISION_Flood 0.0002					//���������ɢ���ͱ��������ڵ��Ƿ���û
#define OlderSpeed 0.8							//���˲�����ɢ���ٶ�0.8m/s
#define MAX_NUMOFROAD 1000						//��·����������޶�Ϊ1000��
#define INF 1000000								//��1000000����Ϊ�����
#define FLOODPRECISION 0.0002					//�����ж���û�ľ�γ����Χ����γ�����С��0.0002��������Ϊ��ͬһ����
#define TIMEQUANTUM 180							//��t�Ĵ�С���˴��趨Ϊ180��
#define RESIDENT 0
#define SCHOOL 1
#define SHOP 2
#define PARK 3									//��ͬ��¥���Ĵ���
#define HOSPITAL 4	
#define HOTEL 5
#define FACTORY 6
#define WORKPLACE 7
/////////////////////////////////////////////////////////////////////////////
// CSetDefData dialog

ofstream f;
CFloor *Floor;							//����ָ��¥�����������ָ��
CShelter *Shelter;						//����ָ����������������ָ��
int FloorRows = 1000;					//��¼¥�������ļ�������
int ShelterRows = 1000;					//��¼�������ļ�������
int WaterDepRows = 100;					//��¼ˮ�������ļ�������
Point * PointArry;						//����ָ��ˮ��ṹ�������ָ��
extern ALGraph _Graph;					//����ȫ�ֱ��� _Graph
extern int NUMOFELEMENT;				//����ȫ�ֱ�����·���бߵ�����
bool ReadFloorFlag = false,ReadShelterFlag = false,ReadWaterFlag = true;      //����Ƿ����¥���ͱ�������Ϣ
ShortPath ShortestPath;					//�����洢���·��������
int ShortPathCount = 0;					//���ڼ���

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
	//��ȡˮ�������ļ���������ʾ��û��Χ
	// TODO: Add your control notification handler code here

	CString filename;//����·��
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("�ı��ĵ�(*.txt)|*.txt|�����ļ�(*.xyz,*.dep,*.dat)|*.xyz;*.dep;*.dat||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();					//ѡ��Х���������ļ������뾭γ������
	} 

	CStdioFile  FileIn;
	CFileException except;							//�����쳣����
	
	CString StrTemp;										//��ʱ����
	
	if(!FileIn.Open(filename,CFile::modeReadWrite,&except))			//���ļ�
	{
		switch(except.m_cause){
		case CFileException::fileNotFound:
			MessageBox("�ļ�δ�ҵ�����ȷ���ļ��Ѿ��ŵ�ָ���ļ����£�","������ʾ",MB_OKCANCEL);
			break;
		case CFileException::endOfFile:
			MessageBox("�ļ�������","������ʾ",MB_OKCANCEL);
		default:
			MessageBox("�ļ���ȡ�쳣��","������ʾ",MB_OKCANCEL);
		}
	}		
	else
	{
		WaterDepRows = 0;
		while (FileIn.ReadString(StrTemp))
		{
			WaterDepRows ++;				//����ˮ���ļ����������������ж���ˮ�����ݵ�
		}

		PointArry =	(Point *) malloc(sizeof(Point) * WaterDepRows);   		//��̬�������ڴ洢ˮ����Ϣ�Ŀռ�
		
		int i = 0;					//ˮ�������±�

		FileIn.SeekToBegin();		//����궨λ���ļ���ͷ���Ա��ͷ��ʼ��ȡ����
		while (FileIn.ReadString(StrTemp))
		{
			sscanf(StrTemp,"%lf %lf %lf",&PointArry[i].lon,&PointArry[i].lat,&PointArry[i].dep);   //��ȡˮ�����ݵ��ڴ���
							
		/*	if (i%1000 == 0 && i!= 0 && WaterDepRows > 1000)
			{
				printf("%f,%f,%f\n",PointArry[i].lon, PointArry[i].lat, PointArry[i].dep);
			}*/
			i ++;
		}
		FileIn.Close();

	/*	if (opendlg.DoModal()==IDOK)
		{
			filename=opendlg.GetPathName();			//ѡ��Х���������ļ��������뾭γ�����Ӧ��ˮ�����ݣ�������ʾ��û��Χ
		} 
		
		ifstream f;									//�����뾭γ�����Ӧ��ˮ������
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
		m_ReadRoadBTN.EnableWindow(TRUE);		//ʹ������·�����ݡ���ť����ʹ��
	}
}

void MarkStartEnd()				
{
	//��Ǵ���ɢ��㣬�������յ���ͼ�е�λ��

	int j;
	structNodes *p1, *p2;
	int num = 0;     //��¼�õ��뼸��·�ཻ
	for (int i = 0;i < _Graph.vertexnum; i ++)
	{
		_Graph._structNodes[i].data.StartOrEnd = 1;  //��Ǹõ�Ϊ������

		for (j = 0;j < FloorRows; j++)
		{

			if (fabs(_Graph._structNodes[i].data.lon - Floor[j].getLon()) < PRECISION_Start_End)  //������ͬ
			{
				if (fabs(_Graph._structNodes[i].data.lat - Floor[j].getLat()) < PRECISION_Start_End)   //γ����ͬ
				{
					_Graph._structNodes[i].data.StartOrEnd = 0;  //��Ǹõ�Ϊ�������
					_Graph._structNodes[i].data.Max_Code_Cap = Floor[j].getNumOfPeo();   //�����Ĵ���ɢ��������¼���ڵ�������������������
					_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap; 
											//��ʼʱ�̣����ʣ��������������Ĵ���ɢ������
				}
			}

		}

		for (j = 0; j < ShelterRows;j++)
		{
			if (_Graph._structNodes[i].data.StartOrEnd != 0)
			{
			
				if (fabs(_Graph._structNodes[i].data.lon - Shelter[j].getLon()) < PRECISION_Start_End)  //������ͬ
				{
			
					if (fabs(_Graph._structNodes[i].data.lat - Shelter[j].getLat()) < PRECISION_Start_End)   //γ����ͬ
					{
						_Graph._structNodes[i].data.StartOrEnd = 2;  //��Ǹõ�Ϊ�յ�����
						_Graph._structNodes[i].data.Max_Code_Cap = Shelter[j].getCap();   //�����յ����Ŀ���������Ϊ��������������
						_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap; 
											//��ʼʱ�̣��յ�Ŀ��������������յ��������������
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
				_Graph._structNodes[i].data.Max_Code_Cap = p1->_LineNodes.Max_Edge_Cap;  //�õ�������·������Ϊһ����·���м�㣬�õ�����������ڵ�·������
				_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap;
			}
			else
			{
				_Graph._structNodes[i].data.Max_Code_Cap =0.7 * TIMEQUANTUM * 0.5 * p1->_LineNodes.width * num / 4;
								//�����·�еĽڵ����������Ҳ�ǰ��զ�t��ʱ��ν��й����
				_Graph._structNodes[i].data.Available_Code_Cap = _Graph._structNodes[i].data.Max_Code_Cap;
			}
		}
	}
	
	f.open("C:\\Users\\zxw\\Desktop\\�������������Ϣ.txt");
	f << "�������\t" << "�������������\t" << "���㵱ǰ����������\t" << "��ֹ���־\n";
		
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
	double slope;				//��¼�¶ȵĴ�С
	double speed = OlderSpeed;
	slope = fabs(S_alt - E_alt)/(Length * 1000) *100;		//�����¶ȣ����¶ȵĴ�С;length * 1000,��ǧ��ת��Ϊ�ף��ٳ���100���¶�ģ��Ҫ��
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
	else if (slope > 12 && slope <= 15)				/*�����¶ȵĴ�С�������ٶ�ϵ�������ʵ�ʵ���ɢ�ٶ�*/
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
	//�õϽ�˹�����㷨�����Ӵ���ɢ��¥��������������̾���
	f.open("C:\\Users\\zxw\\Desktop\\���·������·��ţ�.txt",ios::app);
	int i, j, k, count = 0;				//ѭ������
	int dist[MAX_NUMOFROAD];			//�������ڼ�¼��㵽��ǰ���Ȩֵ��dist[i]ָ�����ֱ�����ӵ���vi��Ȩֵ
	int S[MAX_NUMOFROAD];				//�Ͻ�˹�����㷨�еļ���S�����ڱ�ʾ��ǰ���Ƿ��ڼ���S�У�1��ʾ�ڣ�0��ʾ����
	int path[MAX_NUMOFROAD];			//���ڱ������·����path[2] = 1 ��ʾ��v2��ǰһ����Ϊv1
	structNodes *p1, *p2;
	int n = _Graph.vertexnum;			//ͼ�е�ڵ�ĸ���
	int *TempArray = new int[n+1];		//����һ����СΪn�����飬���ڱ���� u �͵� k ֮���Ȩֵ
	bool RunError = false;				//���д����־

	p1 = _Graph._structNodes[v1-1].first;    //ָ����v1�����ĵ�һ����ı߽ڵ��ָ��
	
	//AfxMessageBox("3");

	for (i = 1; i <= n; i ++)
	{
		TempArray[i] = dist[i] = INF;						//��ʼ����Ϊ���ֵ���������vi֮�䲻��ֱ��������
		for (p2 = p1; p2 != NULL; p2 = p2->next)
		{
			if (i == p2->num)
			{
				dist[i] = p2->_LineNodes.NumofTimeQuantum;		//vi��v1�����ӣ�֮���ȨֵΪ����֮��Ĳ���ʱ��
				break;
			}
		}

		S[i] = 0;									//��ʼʱ�����е㶼����S������
		if (i != v1 && dist[i] < INF)
		{
			path[i] = v1;					//dist[i] < INF����ʾvi��v1ֱ������������vi����һ��Ϊv1
		}
		else
		{
			path[i] = -1;
		}
	}

	S[v1] = 1;							//��v1���뼯��S��
	dist[v1] = 0;						//�������ȨֵΪ0
	for (i= 1; i <= n - 1; i++)			//��v1ȷ�� n-1 �����·��
	{
		double min = INF;
		int u = v1;			//min ��СȨֵ��u ��ʱ����

		for (j = 1; j <= n; j++)		//ѡ��ǰ����V-S�о������·���Ķ���u
		{
			if (!S[j] && dist[j] < min)
			{
				u = j;					/*���S�����У�Ȩֵ��С�ĵ�*/	
				min = dist[j];
			}
		}
		S[u] = 1;						//��u���뼯��S����ʾ�������·���Ѿ����
		
		p1 = _Graph._structNodes[u-1].first;		//ָ���� ��u ֱ�������ӵĵ�һ����ı߽ڵ��ָ��
		
		for (k = 1; k <= n; k ++)		//�޸�V-S�ж����dist �� path ����Ԫ��ֵ
		{
			for (p2 = p1; p2 != NULL; p2 = p2->next)   //���� u �͵� k ֮���Ȩֵ
			{
				if (k == p2->num)
				{
					TempArray[k] = p2->_LineNodes.NumofTimeQuantum;		//vi��v1�����ӣ�֮���ȨֵΪ����֮��Ĳ���ʱ��
					break;
				}
			}

			if (!S[k] && TempArray[k] < INF && dist[u] + TempArray[k] < dist[k])
			{
				dist[k] = dist[u] + TempArray[k];			//�������·������
				path[k] = u;								//�޸�path��������
			}
		}
	}

	//AfxMessageBox("4");
	ofstream outfile;
	//CString filename = filepath + "\\���·��.txt";
	CString filename = "C:\\Users\\zxw\\Desktop\\���·��.txt";
	//outfile.Open(filename,CStdioFile::modeWrite||CStdioFile::modeCreate||CStdioFile::modeNoTruncate);
	//outfile.SeekToEnd();
	outfile.open(filename,ios::app);
	ShortPathEdge *p3 = new ShortPathEdge;
	
	int *shortest = new int[MAX_NUMOFROAD];				//������·���ϵĸ�������ʱ��Ÿ�����������
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
				memset(shortest,0,sizeof(int)*MAX_NUMOFROAD);	//������shortest�е�Ԫ�ض���Ϊ0
				k = 0;
				shortest[k] = i;						//k��ʾshortest���������һ��Ԫ�ص��±�
				while (path[shortest[k]] != v1)			//������
				{
					k ++;
					shortest[k] = path[shortest[k-1]];	//�����·���ĵ����Ŵ�������shortest��
				}
				k ++;
				shortest[k] = v1;						//������������
				ShortPathCount ++;						//��ż�1���洢��һ�����·��
				p3 = ShortestPath.Short_Paht_Code[ShortPathCount].first = NULL;
				for (j = k; j >0; j --)
				{
					outfile << setiosflags(ios::fixed) << setprecision(6) << _Graph._structNodes[shortest[j] - 1].data.lon << "\t";
					outfile << setiosflags(ios::fixed) << setprecision(6) << _Graph._structNodes[shortest[j] - 1].data.lat << "\t";
					outfile << "0" << "\t";				//�������
				//	printf("%d->",shortest[j]);			//������·��
					f << shortest[j] << "->";
				
					if (ShortestPath.Short_Paht_Code[ShortPathCount].first == NULL)  //��ǰ�洢���ǵ�һ����
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
															//�����·���е㣬����ӵ�������
						p3->next = p4;
						p3 = p4;
						p4->next = NULL;
					}
				
					if (j == k)
					{
						outfile  << "0" << "\t" << setiosflags(ios::fixed) << setprecision(6) << dist[i] << "\n";
						ShortestPath.Short_Paht_Code[ShortPathCount].dist = dist[i];	//·Ȩ�������·����
						ShortestPath.Short_Paht_Code[ShortPathCount].NumofCode = k + 1; //shortest����պ�Ϊk+1
						ShortestPath.Short_Paht_Code[ShortPathCount].VisitFlag = false;
					}
					else
					{
						outfile << "1" << "\n";
					}
				}

				ShortPathEdge *p5 = new ShortPathEdge;
				p5->data = &(_Graph._structNodes[shortest[0] - 1].data);         //���·���е����һ����

				p3->next = p5;						//�����·�������һ������ӵ�������
				p3 = p5;
				p5->next = NULL;

				outfile << setiosflags(ios::fixed) << setprecision(6) << _Graph._structNodes[shortest[0] - 1].data.lon << "\t";
				outfile << setiosflags(ios::fixed) << setprecision(6) <<_Graph._structNodes[shortest[0] - 1].data.lat << "\t" << "0" << "\t" << "2" << "\n";
				f << shortest[0] <<"\t"<<setiosflags(ios::fixed) << setprecision(6) <<dist[i]  << endl;
				//printf("%d\t",shortest[0]); 
				//printf("%d\n",dist[i]);					//�������v1������i�����·������
			}
		}
		else if(path[i] == -1)
		{
			AfxMessageBox("���ܴ����������⣺\n1�������·�����ݴ��󡣲���¥����������ľ�γ�Ȳ���·�������С��뽫¥����������ľ�γ��������ʹ�����ݴ���·�������У�\n2��·�������������·�������ǻ�����ͨ�ģ�");
			
			exit(0);
		}
		
	}	
	outfile.close();
	f.close();
}

void FindRoad(ALGraph &s,CString filepath)				
{
	//�Զ���������·�����Ӵ���ɢ�㵽������
	MarkStartEnd();
	//int *FLagOfSpeed = new int[s.arcnum];			//0��ʾ�ñߵ���ɢʱ�仹û�м��㣬1��ʾ�Ѿ��������
	//memset(FLagOfSpeed,0,sizeof(int)*s.arcnum);		//������Ԫ��ȫ����Ϊ0
	double speed = OlderSpeed;			//������ȺĬ�ϵ���ɢ�ٶ�
	structNodes *p1, *p2;
	int j;
	f.open("C:\\Users\\zxw\\Desktop\\����Ȩֵ.txt");
	f << "��·���\t" << "��·����ʱ��\t" << "��·���л���ʱ�������\t" << "�ߵ����������\n";
	for (int i = 0; i < _Graph.vertexnum; i ++)
	{
		p1 = s._structNodes[i].first;					/*�ҵ��ڽӱ��е�ڵ���ָ��ĵ�һ�����ڵı߽ڵ��ָ��
														���ҵ��Ե�ǰ��ڵ�Ϊ���ĵ�һ���ߵ�ָ��*/
		for (p2 = p1; p2 != NULL;p2 = p2->next)
		{
			//AfxMessageBox("5");
			//if (FLagOfSpeed[p2->_LineNodes.code-1] == 0)				//0��ʾ�ñߵ���ɢʱ��û�б�����
			//{
			//	FLagOfSpeed[p2->_LineNodes.code-1] = 1;					//����־��Ϊ1����ʾ�ñߵ���ɢʱ���Ѿ�������
				speed = SpeedCal(p2->_LineNodes.S_alt,p2->_LineNodes.E_alt,p2->_LineNodes.length);
																		//��һ����ʾ��ⲻͬ·�������¶Ȳ�ͬ��ɵ���ɢ�ٶȲ�ͬ
																		//����ÿ��·����Ҫ�������ʱ�䣬���ա�0.8m/s*�¶��ٶ�Ӱ�����ӡ����ٶȼ��㡣0.8m/sΪ�����ٶ�
				p2->_LineNodes.WalkTime = p2->_LineNodes.length * 1000 / speed;  //���������·��ɢ����Ҫ��ʱ�䡣����1000���ǰѾ��뵥λǧ��ת��Ϊ��
				p2->_LineNodes.NumofTimeQuantum = 1 + p2->_LineNodes.WalkTime / TIMEQUANTUM;  //���������·��ɢ����Ҫ�Ħ�t������
				p2->_LineNodes.Max_Edge_Cap = 0.7 * (speed * TIMEQUANTUM * p2->_LineNodes.width) / 1.5; 
																//0.7Ϊ�������ӣ�����·�������70%Ϊ��ռ�á���tʱ���ڣ���·���Ϊ�ٶȳ���ʱ���ٳ��Ե�·��ȡ����� 1.5 ��ָ���˾�ռ�����Ϊ1.5ƽ����
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
			case 0:   //���õ�Ϊ����ɢ���ʱ
				 for (j = 0;j < WaterDepRows;j++)
				 {
					if (s._structNodes[i].data.StartOrEnd == 1)
							break;
					if (fabs(s._structNodes[i].data.lon - PointArry[j].lon) <= FLOODPRECISION)	//������ͬ
					{
						if (fabs(s._structNodes[i].data.lat - PointArry[j].lat) <= FLOODPRECISION)	//γ����ͬ
						{
							if (PointArry[j].dep < s._structNodes[i].data.alt)	
									//��һ���ʾ����ˮ���С��¥������ʱ����ˮû����û¥��ʱ��¥�����������ɢ�����õ������־���������Ϊһ���
							{
								s._structNodes[i].data.StartOrEnd = 1;			//���õ�����Ϊһ���
								//AfxMessageBox("0");
							}
						}
					}
				 }	 
				 break;
			case 2:			//���õ�Ϊ�������յ�ʱ
				//AfxMessageBox("5");
				for (j = 0;j < WaterDepRows;j++)
					 {
						 if (s._structNodes[i].data.StartOrEnd == 1)
							break;
						 if (fabs(s._structNodes[i].data.lon - PointArry[j].lon) <= FLOODPRECISION)	//������ͬ
						 {
							 if (fabs(s._structNodes[i].data.lat - PointArry[j].lat) <= FLOODPRECISION)	//γ����ͬ
							 {
								 if (PointArry[j].dep >= s._structNodes[i].data.alt)	
									 //��һ���ʾ����ˮ��δ��ڵ��ڱ���������ʱ����ˮ��û������ʱ���ñ�����������������ɢ�����õ���յ��־���������Ϊһ���
								 {
									 s._structNodes[i].data.StartOrEnd = 1;			//���õ�����Ϊһ���
									 //AfxMessageBox("1");
								 }
							 }
						 }
					}	
					 break;
			default:
				AfxMessageBox("���ִ���������������");

			}	
		} */
	}

	f.close();

	ShortPathCount = 0;
	//shortPath(1);
	for ( i = 0;i < _Graph.vertexnum;i ++)
	{
		if (_Graph._structNodes[i].data.StartOrEnd == 0)				//����õ�Ϊ����ɢ��ʼ�㣬�����Ըõ�Ϊ�������·��
		{
			//AfxMessageBox("2");
			shortPath(_Graph._structNodes[i].data.code,filepath);
		}
		//AfxMessageBox("3");
	}

}

void quickSort(int low,int high)					//Ϊ���·�����򣬿��������㷨
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


void bubblesort(int n)  //ð������
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

bool People_exist_flag()							//�������Ƿ�����δ����ɢ
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
		if (flow > p2->next->data->Available_Code_Cap)		//���ǰһ����Ŀ���������С�ں�һ���㣬��flow��ֵ����
		{
			flow = p2->next->data->Available_Code_Cap;			
		}
		p3 = _Graph._structNodes[p2->data->code - 1].first;

		if (p3->next ==NULL)
		{
			if (p3->num == p2->next->data->code)		/*���p2��ָ��Ľڵ����һ���ڵ�ı�ŵ���
								��p3��ָ��ı߽ڵ������ıߵ���һͷ�Ľڵ��ţ���ô�����߾������·������������ı�*/
			{
				if (flow > p3->_LineNodes.Available_Edge_Cap)	//�����ʱ����Сֵflow С�� ����֮��ıߵĿ���������������flow
				{
					flow = p3->_LineNodes.Available_Edge_Cap;
				}
			}
		}
		for (p4 = p3; p4->next != NULL;p4 = p4->next)
		{
			if (p4->num == p2->next->data->code)		/*���p2��ָ��Ľڵ����һ���ڵ�ı�ŵ���
								��p4��ָ��ı߽ڵ������ıߵ���һͷ�Ľڵ��ţ���ô�����߾������·������������ı�*/
			{
				if (flow > p4->_LineNodes.Available_Edge_Cap)	//�����ʱ����Сֵflow С�� ����֮��ıߵĿ���������������flow
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
	int TimeCount = 0;								//��ʼʱ��Ϊt0ʱ��
	ShortPathEdge *p1,*p2;							//��ʱ����
	structNodes *p3,*p4;							//��ʱ����
	bool RoadFlag = true;							//��ǰ���·���Ƿ���õı�־�������ô���������Ⱥ�ڸ�����·����ɢ
	int flow = 0;									//��ʾĳ�����·���У����ʣ���������յ�ʣ��������������м��ͱߵ�ʣ������������е���Сֵ
	int timeSpent = 0;								//��ʾĳ�����·����ʱ�仨�ѣ����ڼ�¼ʱ������
	int tempFlag = 1;								//��ʱ��־
	int i,j;
	int *Ava = new int[ShortPathCount];				//��¼��·�������ʣ��������˼���

	f.open("C:\\Users\\zxw\\Desktop\\δ����֮ǰ�����·��.txt");
	f << "��·Ȩֵ\t" << "·�����\n";
	
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
	

	//quickSort(1,ShortPathCount);					//�����е����·��������Ȩֵ��С�����������
	bubblesort(ShortPathCount);

	f.open("C:\\Users\\zxw\\Desktop\\���������·��.txt");
	f << "��·Ȩֵ\t" << "·�����\n";

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
	
	f.open("C:\\Users\\zxw\\Desktop\\��Ⱥ��ɢ������ʱ��仯.txt",ios::app);
	f << "��ʼʱ��\t" << "��㾭��\t" << "���γ��\t" << "�����\t" << "��������\t" << "����ʱ��\t" << "ʣ��δ��ɢ����\t" << "�ߺ�ʱ�������\n";
	while (People_exist_flag())						//������ɢ��������Ҫ��ɢʱ�����㷨��һֱִ�У�ֱ�������˱��ɹ���ɢ
	{
		printf("TimeCount = %d ",TimeCount);
		for (j = 0; j < _Graph.vertexnum; j++)
		{
			if (_Graph._structNodes[j].data.StartOrEnd == 1)
			{
				_Graph._structNodes[j].data.Available_Code_Cap = _Graph._structNodes[j].data.Max_Code_Cap;
				//һ���µ�ʱ�̵������м��ʣ��Ŀ�������������������������
			}
			p3 = _Graph._structNodes[j].first;
			p3->_LineNodes.Available_Edge_Cap = p3->_LineNodes.Max_Edge_Cap;
			for (p4 = p3; p4->next != NULL; p4 = p4->next)
			{
				p4->next->_LineNodes.Available_Edge_Cap = p4->next->_LineNodes.Max_Edge_Cap;
				//һ���µ�ʱ�̵����󣬸����ߵ�ʣ��Ŀ�������������������������
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
				continue;							//����������·�Ѿ������ʹ�������ǰʱ�����˴Ӹ�����·����ɢ���������Ѱ����һ�����·
			}
			else
			{*/
		//		ShortestPath.Short_Paht_Code[i].VisitFlag = true;	//�ҵ���һ��ʱ�仨�����ٵ�·��������û�б����ʹ�����·���ı��Ϊ i 

			p1 = ShortestPath.Short_Paht_Code[i].first;		//p1ָ���i�����·���ĵ�һ���ڵ�
			if (p1->data->Available_Code_Cap <= 0)		//���·���еĸõ�Ŀ���������С��0����ʾ��ǰ��·������ɢ��Ⱥ���ҵ�·��ȫ����Ⱥռ��
			{
				RoadFlag = false;						//��ʾ�������·���еĵ����ڱ�ʹ�ã�����������ɢ
			}
			for (p2 = p1;p2->next != NULL;p2 = p2->next)
			{
				if (p2->next->data->Available_Code_Cap <= 0)		//���·���еĸõ�Ŀ���������С��0����ʾ��ǰ��·������ɢ��Ⱥ���ҵ�·��ȫ����Ⱥռ��
				{
					
					RoadFlag = false;						//��ʾ�������·���еĵ����ڱ�ʹ�ã�����������ɢ
					break;
				}
				
				p3 = _Graph._structNodes[p2->data->code - 1].first;
				
			
				if (p3->num == p2->next->data->code)					/*���p2��ָ��Ľڵ����һ���ڵ�ı�ŵ���
					��p3��ָ��Ľڵ������ıߵ���һͷ�Ľڵ��ţ���ô�����߾������·������������ı�,�жϸñߵĿ��������������жϸñ��Ƿ����*/
				{
					if (p3->_LineNodes.Available_Edge_Cap <= 0)
					{
						RoadFlag = false;			//��ʾ�������·���еı����ڱ�ʹ�ã�����������ɢ
					}
					
				}
				
				for (p4 = p3;p4->next != NULL; p4 = p4->next)			//�ж����·���бߵĿ����������Ƿ����0�����жϸı��Ƿ����
				{
				if (p4->next->num == p2->next->data->code)					/*���p2��ָ��Ľڵ����һ���ڵ�ı�ŵ���
					��p2��ָ��Ľڵ������ıߵ���һͷ�Ľڵ��ţ���ô�����߾������·������������ı�,�жϸñߵĿ��������������жϸñ��Ƿ����*/
				{
					if (p4->next->_LineNodes.Available_Edge_Cap <= 0)
					{
						RoadFlag = false;			//��ʾ�������·���еı����ڱ�ʹ�ã�����������ɢ
						break;
					}
					
				}
				}
			}
			
			if (RoadFlag)
			{
				flow = Min_Capacity_Road(i);
				
				timeSpent = TimeCount;								//ʱ�����еĳ�ʼֵ
				p1 = ShortestPath.Short_Paht_Code[i].first;
				f << "T" << TimeCount << "\t" << p1->data->lon << "\t" << p1->data->lat << "\t" << p1->data->code <<"\t";
				f << flow << "\t" << ShortestPath.Short_Paht_Code[i].dist << "\t" << p1->data->Available_Code_Cap - flow << "\t";
				
				printf("T%d, %f, %f, %d, %d, %d, %d\n",TimeCount,p1->data->lon,p1->data->lat,p1->data->code,flow,ShortestPath.Short_Paht_Code[i].dist,p1->data->Available_Code_Cap - flow);
				
				p1->data->Available_Code_Cap -= flow;			//�ߵ��������flow
				f << p1->data->code <<"(T" << timeSpent << ")";
				
				printf("%d(T%d)",p1->data->code,timeSpent);
				
				for (p2 = p1;p2->next != NULL;p2 = p2->next)
				{
					//	AfxMessageBox("3");
					
					p2->next->data->Available_Code_Cap -= flow;		//�ߵ��յ�����flow
					
					p3 = _Graph._structNodes[p2->data->code - 1].first;
					
					
					if (p3->num == p2->next->data->code)		/*���p2��ָ��Ľڵ����һ���ڵ�ı�ŵ���
						��p3��ָ��ı߽ڵ������ıߵ���һͷ�Ľڵ��ţ���ô�����߾������·������������ı�*/
					{
						p3->_LineNodes.Available_Edge_Cap -= flow;  //����������flow
						f << "->";
						f << p2->next->data->code <<"(T" << p3->_LineNodes.NumofTimeQuantum + timeSpent << ")";
						
						printf("->%d(T%d)",p2->next->data->code,p3->_LineNodes.NumofTimeQuantum + timeSpent);
						timeSpent += p3->_LineNodes.NumofTimeQuantum;
					}	
					
					for (p4 = p3; p4->next != NULL;p4 = p4->next)
					{
					if (p4->next->num == p2->next->data->code)		/*���p2��ָ��Ľڵ����һ���ڵ�ı�ŵ���
						��p4��ָ��Ľڵ������ıߵ���һͷ�Ľڵ��ţ���ô�����߾������·������������ı�*/
					{
						p4->next->_LineNodes.Available_Edge_Cap -= flow;  //����������flow
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

		TimeCount ++;				//ʱ�������1������һʱ�̽�����һʱ��

		/*for (int tempCount = 1;	tempCount <= ShortPathCount; tempCount++)
		{
			ShortestPath.Short_Paht_Code[tempCount].VisitFlag = false;
			//AfxMessageBox("5");
		}*/
		tempFlag ++;
	
	}//while
	f.close();
	AfxMessageBox("��ɢ������");
}

void CSetDefData::OnBTNReadRoadData() 
{
	//��ȡ·�����ݣ�����������·��
	// TODO: Add your control notification handler code here

	CString filename;				//�����ļ���
	CString filepath;				//�����ļ�·��
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("�ı��ĵ�(*.txt)|*.txt||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filepath = filename = opendlg.GetPathName();					//ѡ��·�������ļ�������������ݣ�����������·��
		filepath = filepath.Left(filepath.ReverseFind('\\'));					//��ȡ�ļ�����Ŀ¼
		//printf("%s,%s",filename,filepath);
	
		gainData(filename);					//��ȡ·������
		CreateGraph(_Graph,filepath);		//����·����ϵ
		printGraph(_Graph,filepath);		//��ӡ·����ϵ
		FindRoad(_Graph,filepath);			//�����·��
		PopAllocation();
	} 
	else
	{
		AfxMessageBox("δѡ���κ��ļ���");
	}

}

void CSetDefData::PopDistribution()
{
	//�����˿ڷֲ����
	int BuildType = 0;							//����¥�����ܣ�Ĭ��Ϊ0��������
	double AreaAverage[9] = {32.9,4,3.8,42,12,6,80,8.5,9};
	//���ø��ֹ���¥����Ĭ���˾����
	double AreaRadio[9] = {0.63,0.4,0.43,0.5,0.43,0.43,0.43,0.43,0.6};
	//�洢��ͬ����¥��ռ����������������ı��������ж������������ʵ�ʵ�¥�����
	double CapacityRadio[2][9] = {{0.5,1.1,0.35,0.6,0.7,0.5,1,1,0.6},{1,0,0,0,0.5,0.8,0,0,0}};
	//�洢��ͬ����¥����ռ�ñ��ʣ����ж��������������ռ��
	int DayOrNight = 1;							//��0����ʾ���죬��1����ʾ����
	int NumOfPeo = 0;							//�洢�˿���������ʱ����
	
	if(this->m_factory > 0)
		AreaAverage[FACTORY] = this->m_factory;
	if(this->m_hospital > 0)
		AreaAverage[HOSPITAL] = this->m_hospital;
	if(this->m_hotel > 0)						//��Ը��ֹ������͵ķ�����
		AreaAverage[HOTEL] = this->m_hotel;					//�����˾�ռ�����
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
		DayOrNight = 0;						//����Ϊ0����ʾ��ǰѡ��Ϊ����ģʽ
	}
	else if (m_RadioDay.GetCheck() == 0 && m_RadioNight.GetCheck() == 1)
	{
		DayOrNight = 1;						//����Ϊ1����ʾ��ǰѡ��Ϊҹ��ģʽ
	}
	

	f.open("C:\\Users\\zxw\\Desktop\\¥���˿�����.txt");
	f << "����\t" << "γ��\t" << "���\t" << "����\t" << "¥������\n";
	for (int i = 0; i< FloorRows; i++)
	{
		BuildType = Floor[i].getType();
		NumOfPeo = ((1000000 * Floor[i].getArea()*AreaRadio[BuildType])/AreaAverage[BuildType])*CapacityRadio[DayOrNight][BuildType]*(Floor[i].getHight()/2.8);
		//����1 000 000��Ϊ�˰�ƽ��ǧ��ת��Ϊƽ���ס������˿�������ĳ�����˿���=(ĳ���������*¥����ռ�������)/�˾����������/���ϵ�ϵ����¥���߶�/ÿ��߶�
		Floor[i].setNumOfPeo(NumOfPeo);			//��¼ÿ��������Ƴ����˿�����
		
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
	//��ȡ������Ϣ�����ڹ����˿ڷֲ�
	// TODO: Add your control notification handler code here
	CString filename;//����·��
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("�ı��ĵ�(*.txt)|*.txt||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();					//ѡ���������ļ�������������ݣ����ڹ����˿ڷֲ�
	} 

	CStdioFile  FileIn;
	CFileException except;							//�����쳣����

	CString StrTemp;

	if(!FileIn.Open(filename,CFile::modeReadWrite,&except))			//���ļ�
	{
		switch(except.m_cause){
		case CFileException::fileNotFound:
			MessageBox("�ļ�δ�ҵ�����ȷ���ļ��Ѿ��ŵ�ָ���ļ����£�","������ʾ",MB_OKCANCEL);
			break;
		case CFileException::endOfFile:
			MessageBox("�ļ�������","������ʾ",MB_OKCANCEL);
		default:
			MessageBox("�ļ���ȡ�쳣��","������ʾ",MB_OKCANCEL);
		}
	}		
	else
	{
		FloorRows = 0;
		while (FileIn.ReadString(StrTemp))
		{
			FloorRows ++;				//����¥���ļ����������������ж�����ɢ��ȴ���ɢ
		}

		if (FloorRows == 0)
		{
			FloorRows = 1000;
		}
		
		Floor = (CFloor*) malloc(sizeof(CFloor) * FloorRows);   		//��̬�������ڴ洢¥����Ϣ�Ŀռ�
							
		double lon = 0;					//¥������
		double lat = 0;					//¥��γ��
		double alt = 0;					//¥�����θ߶�
		double area = 0;				//¥�����
		double hight = 0;				//¥���߶�
		int type = 0;					//¥�����ܷ���   0:������ 1:ѧУ 2:�̳� 3:��԰ 4:ҽԺ 5:���� 6:���� 7:�칫���� 8:�羰��
		
		int i = 0;					//¥��������±�
		FileIn.SeekToBegin();					//����궨λ���ļ���ͷ���Ա��ȡ�ļ�
		while (FileIn.ReadString(StrTemp))			//��ȡ¥�����ݵ��ڴ���
		{
			sscanf(StrTemp,"%lf %lf %lf %lf %lf %d",&lon,&lat,&alt,&area,&hight,&type);
			Floor[i].setLon(lon);
			Floor[i].setLat(lat);			//�ֱ𽫾�γ�ȣ����θ߶ȣ�¥���߶ȣ�¥������ȴ����ڴ�
			Floor[i].setAlt(alt);
			Floor[i].setArea(area);
			Floor[i].setHight(hight);
			Floor[i].setType(type);
			i ++;
		}
				
		PopDistribution();
		FileIn.Close();
	}	

	ReadFloorFlag = true;			//�趨����¥����Ϣ���ΪTRUE
	if (ReadShelterFlag && ReadWaterFlag)
	{
		m_ReadRoadBTN.EnableWindow(TRUE);		//ʹ������·�����ݡ���ť����ʹ��
	}
}

void CSetDefData::OnBTNReadshelterData() 
{
	//��ȡ���������ݣ�������Ⱥ��ɢ
	// TODO: Add your control notification handler code here
	CString filename;//����·��
	
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("�ı��ĵ�(*.txt)|*.txt||"),NULL);   
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();					//ѡ������������ļ�������������ݣ�������Ⱥ��ɢ
	} 

	CStdioFile  FileIn;
	CFileException except;							//�����쳣����
	
	CString StrTemp;
	
	if(!FileIn.Open(filename,CFile::modeReadWrite,&except))			//���ļ�
	{
		switch(except.m_cause){
		case CFileException::fileNotFound:
			MessageBox("�ļ�δ�ҵ�����ȷ���ļ��Ѿ��ŵ�ָ���ļ����£�","������ʾ",MB_OKCANCEL);
			break;
		case CFileException::endOfFile:
			MessageBox("�ļ�������","������ʾ",MB_OKCANCEL);
		default:
			MessageBox("�ļ���ȡ�쳣��","������ʾ",MB_OKCANCEL);
		}
	}		
	else
	{
		ShelterRows = 0;
		while (FileIn.ReadString(StrTemp))
		{
			ShelterRows ++;				//����������ļ����������������ж��ٱ���������������ɢ
		}

		if (ShelterRows == 0)
		{
			ShelterRows = 100;
		}
		
		Shelter = (CShelter*) malloc(sizeof(CShelter) * ShelterRows);   		//��̬�������ڴ洢��������Ϣ�Ŀռ�
					
		double lon = 0;					//����������
		double lat = 0;					//������γ��
		double alt = 0;					//���������θ߶�
		int capacity = 0;				//�������߶�
		int type = 0;					//���������ܷ���   0:������ 1:ѧУ 2:�̳� 3:��԰ 4:ҽԺ 5:���� 6:���� 7:�칫���� 8:�羰�� 9:����ݣ�����
		
		int i = 0;					//¥��������±�
		FileIn.SeekToBegin();           //����궨λ�ļ���ͷ���Ա��ȡ�ļ�����
		while(FileIn.ReadString(StrTemp))		//��ȡ���������ݵ��ڴ���
		{
			sscanf(StrTemp,"%lf %lf %lf %d %d",&lon,&lat,&alt,&capacity,&type);
			Shelter[i].setLon(lon);
			Shelter[i].setLat(lat);			//�ֱ𽫾�γ�ȣ����θ߶ȣ������������������������ܵȴ����ڴ�
			Shelter[i].setAlt(alt);
			Shelter[i].setCap(capacity);
			Shelter[i].setType(type);
			i ++;
			printf("%lf,%lf,%lf,%d,%d\n",lon,lat,alt,capacity,type);
		}	
		FileIn.Close();
	}
	
	ReadShelterFlag = true;			//�趨�����������Ϣ���ΪTRUE�����˱�־ΪTRUE����ReadFloorFlagҲΪtrueʱ��������·�����ݡ���ť������ʹ��
	if (ReadFloorFlag && ReadWaterFlag)
	{
		m_ReadRoadBTN.EnableWindow(TRUE);		//ʹ������·�����ݡ���ť����ʹ��
	}
}

void CSetDefData::OnRADIODay() 
{
	//�������ѡ��ʱ����Ӧ����
	// TODO: Add your control notification handler code here
	m_RadioNight.SetCheck(FALSE);
}

void CSetDefData::OnRADIONight() 
{
	//�������ѡ��ʱ����Ӧ����
	// TODO: Add your control notification handler code here
	m_RadioDay.SetCheck(FALSE);
}
