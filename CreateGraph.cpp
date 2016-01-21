#include "Typedefine.h"

LineNodes LineArray[1000]; // 全局的道路数组
int NUMOFELEMENT = 0;			//记录总的路段数
ALGraph _Graph;				//声明为全局变量，存储图的结构
Vertexs _vertex[1000];

void gainData(CString filename)	//从文件读取数据
{
	CString g_altUrl = filename;
	
	ifstream infileAlt;
	infileAlt.open(g_altUrl);
	
	NUMOFELEMENT = 0;				//行数置0

	while(!infileAlt.eof())
		//for(; NUMOFELEMENT < 10;)
	{
		infileAlt >> LineArray[NUMOFELEMENT].S_lon;
		infileAlt >> LineArray[NUMOFELEMENT].S_lat;
		infileAlt >> LineArray[NUMOFELEMENT].E_lon;
		infileAlt >> LineArray[NUMOFELEMENT].E_lat;
		infileAlt >> LineArray[NUMOFELEMENT].S_alt;
		infileAlt >> LineArray[NUMOFELEMENT].E_alt;
		infileAlt >> LineArray[NUMOFELEMENT].length;
		infileAlt >> LineArray[NUMOFELEMENT].width;
		infileAlt >> LineArray[NUMOFELEMENT].code;
		NUMOFELEMENT ++;
	}
	infileAlt.close();
}

BOOL TheVertexIN(double lon, double lat, int sum)
{
	BOOL m_true;	//true 表示这个点在_vertex中不存在
	for(int i = 0; i < sum; i ++)
	{
		if(lon != _vertex[i].lon || lat != _vertex[i].lat)
		{
			m_true = TRUE;
			
		}
		else
		{
			m_true = FALSE;
			return m_true;
		}
	}
	return m_true;
}

int CaculateVertexofGraph(CString filepath)  //找到路网中的所有结点
{
	int SUMOFVERTEXSOFGRAPH = 0;
	for(int i = 0; i < NUMOFELEMENT; i ++)
	{
		if(i == 0)
		{
			_vertex[SUMOFVERTEXSOFGRAPH].lon     = LineArray[i].S_lon;
			_vertex[SUMOFVERTEXSOFGRAPH].lat     = LineArray[i].S_lat;
			_vertex[SUMOFVERTEXSOFGRAPH].alt     = LineArray[i].S_alt;
			_vertex[SUMOFVERTEXSOFGRAPH].code    = SUMOFVERTEXSOFGRAPH + 1;
			_vertex[SUMOFVERTEXSOFGRAPH + 1].lon = LineArray[i].E_lon;
			_vertex[SUMOFVERTEXSOFGRAPH + 1].lat = LineArray[i].E_lat;
			_vertex[SUMOFVERTEXSOFGRAPH + 1].alt = LineArray[i].E_alt;
			SUMOFVERTEXSOFGRAPH += 2;
			_vertex[SUMOFVERTEXSOFGRAPH - 1].code = SUMOFVERTEXSOFGRAPH;
		}
		else
		{
			//for(int j = 0; j < SUMOFVERTEXSOFGRAPH; j ++)
			//{
			if(TheVertexIN(LineArray[i].S_lon, LineArray[i].S_lat, SUMOFVERTEXSOFGRAPH) &&
				!TheVertexIN(LineArray[i].E_lon, LineArray[i].E_lat, SUMOFVERTEXSOFGRAPH))
			{
				_vertex[SUMOFVERTEXSOFGRAPH].lon = LineArray[i].S_lon;
				_vertex[SUMOFVERTEXSOFGRAPH].lat = LineArray[i].S_lat;
				_vertex[SUMOFVERTEXSOFGRAPH].alt = LineArray[i].S_alt;
				SUMOFVERTEXSOFGRAPH ++;
				_vertex[SUMOFVERTEXSOFGRAPH - 1].code = SUMOFVERTEXSOFGRAPH;
			}
			if(!TheVertexIN(LineArray[i].S_lon, LineArray[i].S_lat, SUMOFVERTEXSOFGRAPH) &&
				TheVertexIN(LineArray[i].E_lon, LineArray[i].E_lat, SUMOFVERTEXSOFGRAPH))
			{
				_vertex[SUMOFVERTEXSOFGRAPH].lon = LineArray[i].E_lon;
				_vertex[SUMOFVERTEXSOFGRAPH].lat = LineArray[i].E_lat;
				_vertex[SUMOFVERTEXSOFGRAPH].alt = LineArray[i].E_alt;
				SUMOFVERTEXSOFGRAPH ++;
				_vertex[SUMOFVERTEXSOFGRAPH - 1].code = SUMOFVERTEXSOFGRAPH;
			}
			if(TheVertexIN(LineArray[i].S_lon, LineArray[i].S_lat, SUMOFVERTEXSOFGRAPH) &&
				TheVertexIN(LineArray[i].E_lon, LineArray[i].E_lat, SUMOFVERTEXSOFGRAPH))
			{
				_vertex[SUMOFVERTEXSOFGRAPH].lon = LineArray[i].S_lon;
				_vertex[SUMOFVERTEXSOFGRAPH].lat = LineArray[i].S_lat;
				_vertex[SUMOFVERTEXSOFGRAPH].alt = LineArray[i].S_alt;
				SUMOFVERTEXSOFGRAPH ++;
				_vertex[SUMOFVERTEXSOFGRAPH - 1].code = SUMOFVERTEXSOFGRAPH; 
				_vertex[SUMOFVERTEXSOFGRAPH].lon = LineArray[i].E_lon;
				_vertex[SUMOFVERTEXSOFGRAPH].lat = LineArray[i].E_lat;
				_vertex[SUMOFVERTEXSOFGRAPH].alt = LineArray[i].E_alt;
				SUMOFVERTEXSOFGRAPH ++;
				_vertex[SUMOFVERTEXSOFGRAPH - 1].code = SUMOFVERTEXSOFGRAPH;
			}
			//	}
		}
	}
	ofstream outfile;
	CString m_Url = filepath + "\\vertex.txt";
	outfile.open(m_Url);
	for(int k = 0; k < SUMOFVERTEXSOFGRAPH; k ++)
	{
		outfile << setiosflags(ios::fixed) << setprecision(6) <<_vertex[k].lon << "\t";
		outfile << setiosflags(ios::fixed) << setprecision(6) << _vertex[k].lat << "\t";
		outfile << setiosflags(ios::fixed) << setprecision(6) << _vertex[k].alt << "\t";
		outfile << _vertex[k].StartOrEnd << "\t";
		outfile << _vertex[k].code;
		outfile << endl;
	}
	outfile.close();
	
	return SUMOFVERTEXSOFGRAPH;
}

int GetCodeOfArcPointTo(LineNodes temp, int k)
{
	double t_lon;
	double t_lat;
	int code;
	
	if(temp.S_lon == _vertex[k].lon && temp.S_lat == _vertex[k].lat)
	{
		t_lon = temp.E_lon;
		t_lat = temp.E_lat;
	}
	else
	{
		t_lon = temp.S_lon;
		t_lat = temp.S_lat;
	}
	for(int i = 0; i < _Graph.vertexnum; i ++)
	{
		if(_vertex[i].lon == t_lon && _vertex[i].lat == t_lat)
		{
			code = _vertex[i].code;
		}
	}
	return code;
}

void CreateGraph(ALGraph &s,CString filepath)	//创建图
{
	s.vertexnum = CaculateVertexofGraph(filepath);
	s.arcnum = NUMOFELEMENT;
	for(int i = 0; i < s.vertexnum; i ++)
	{
		s._structNodes[i].data = _vertex[i];
		
		structNodes *p1 = new structNodes;
		s._structNodes[i].first = NULL;
		
		for(int k = 0; k < NUMOFELEMENT; k ++)
		{
			if((s._structNodes[i].data.lon == LineArray[k].S_lon && s._structNodes[i].data.lat == LineArray[k].S_lat)
				||(s._structNodes[i].data.lon == LineArray[k].E_lon && s._structNodes[i].data.lat == LineArray[k].E_lat))
			{
				structNodes *p2 = new structNodes;
				p2->_LineNodes = LineArray[k];
				p2->num = GetCodeOfArcPointTo(LineArray[k], i);
				if(s._structNodes[i].first == NULL)
				{
					p1 = p2;
					s._structNodes[i].first = p1;
					p1->next = NULL;
				}
				else
				{
					p1->next = p2;
					p1 = p2;
					p2->next = NULL;
				}
			}
		}
	}
}

void printGraph(ALGraph &s,CString filepath)	//输出图
{
	CString m_url = filepath + "\\FstreetList.txt";
	ofstream outfileList;
	outfileList.open(m_url);
	structNodes *p1;
	structNodes *p2;
	p1 = new structNodes;
	for(int i = 0; i < s.vertexnum; i ++)
	{
		//p1 = &structArray[i];
		outfileList << "(" << setiosflags(ios::fixed) << setprecision(6) << s._structNodes[i].data.lon << ",";
		outfileList << setiosflags(ios::fixed) << setprecision(6) <<s._structNodes[i].data.lat << ",";
		outfileList << setiosflags(ios::fixed) << setprecision(6) <<s._structNodes[i].data.code << ")";
		p1 = s._structNodes[i].first;
		for(p2 =p1; p2 != NULL; p2 = p2->next)
		{
			outfileList <<  "->" <<	"(" <<p2->_LineNodes.code << "," << p2->num << ")";
		}
		outfileList << endl;
	}
	outfileList.close();
}
