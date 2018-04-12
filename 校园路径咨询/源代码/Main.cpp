#include"UndirectedGraph.h"
#include<assert.h>
#include<windows.h>
#include<fstream>
#include<graphics.h>//图形库
#include<string>
#include"CVector.h"
using std::string;
void Init(UndirectedGraph* graph);
void InitForArticul(UndirectedGraph* graph);
void InitForAllPath(UndirectedGraph* graph);
void traverse(Vertex *v);


typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();
PROCGETCONSOLEWINDOW GetConsoleWindo;

HWND hwnd;
HDC hdc;
// 画方块函数定义

void outtextxy(int x , int y, string& text);
void gotoXY(int x, int y) {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = {x,y};
	SetConsoleCursorPosition(hout, c);
}
template <class Type>//将string类型转换为type的数字类型
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

bool safeStringToInt(string str, int& num);
void Menu(UndirectedGraph&);

void showViewSpot(UndirectedGraph&);
void showViewSpotInfo(Vertex&);
void showViewSpotNeighborInfo(Vertex&);
void showPathInfo(string strViewSpot1, string strViewSpot2,EdgeNode* path);
void searchSchoolArticul(UndirectedGraph&);

void findPathFromTwoViewSpot(UndirectedGraph&);
void findPathFromTwoViewSpot(UndirectedGraph&graph, string start, string target);

void changeViewSpotInfo(UndirectedGraph&);
void changeViewSpotInfo(UndirectedGraph&, Vertex* viewSpot);
void addViewSpotInfo(UndirectedGraph&);
void changePathInfo(UndirectedGraph&);
void changePathInfo(UndirectedGraph& graph, 
					string strViewSpot1, string strViewSpot2,
					EdgeNode* path);
void addPathInfo(UndirectedGraph&);
void addPathInfo(UndirectedGraph&,string str1,string str2);
void removeViewSpot(UndirectedGraph&);
void removePath(UndirectedGraph&);

void saveGraphInfo(UndirectedGraph&);
void CreateGraphFromFile(UndirectedGraph&,string);
void showAllViewSpots(UndirectedGraph&);

int main() 
{

	UndirectedGraph* graph = new UndirectedGraph();
	//InitForArticul(graph);
	//saveGraphInfo(*graph);
	CreateGraphFromFile(*graph, "graphInfo.txt");
	Menu(*graph);
	//showAllViewSpots(*graph);
	SAFE_DELETE(graph);
	getchar();
	return 0; 
}

void Menu(UndirectedGraph& graph)
{
	system("cls");
	string isContinue = "c";
	string select = "";
	while (isContinue[0] != 'q')
	{
		cout << "Write By---3115005248--15软一欧阳文靖" << endl;
		cout << "校园景点查询系统" << endl;
		cout << "1.显示校园地图" << endl;
		cout << "2.查询景点信息" << endl;
		cout << "3.查询校园关节点" << endl;
		cout << "4.查询两个景点间路径" << endl;
		cout << "5.修改景点信息" << endl;
		cout << "6.修改路径信息" << endl;
		cout << "7.删除景点" << endl;
		cout << "8.删除路径" << endl;
		cout << "9.增加景点" << endl;
		cout << "a.增加路径" << endl;
		cout << "b.保存" << endl;
		cout << "\n\n"<< endl;
		cout << "选择功能：";
		cin >> select;
		switch (select[0])
		{
		case '1':	showAllViewSpots(graph);
					break;
		case '2':	showViewSpot(graph);
					break;
		case '3':	searchSchoolArticul(graph);
					break;
		case '4':	findPathFromTwoViewSpot(graph);
					break;
		case '5':	changeViewSpotInfo(graph);
					break;
		case '6':	changePathInfo(graph);
					break;
		case '7':	removeViewSpot(graph);
					break;
		case '8':	removePath(graph);
					break;
		case '9':  addViewSpotInfo(graph);
					break;
		case 'a':   addPathInfo(graph);
					break;
		case 'b':   saveGraphInfo(graph);
					break;
		default:cout << "无此功能，请重新输入。\n任意键继续；" << endl;
			system("pause");
			Menu(graph);
		}
	}
}


//输入景点，如果存在这个景点，则显示；
void showViewSpot(UndirectedGraph& graph) {
	system("cls");
	string viewSpot;
	cout << "输入查询的景点:";
	cin >> viewSpot;

	Vertex* viewSpotVertex = graph.GetVex(viewSpot);
	if (viewSpotVertex == NULL)
	{
		cout << "无此景点。" << endl;
		cout << "是否继续查询？" << endl;
		cout << "确定按‘y’,其他放回菜单";

		string select;
		cin >> select;
		if (select[0] == 'y')
			showViewSpot(graph);
		else
			Menu(graph);
	}
	else {

		cout << "\n\n";
		showViewSpotInfo(*viewSpotVertex);
		cout << "\n";
		showViewSpotNeighborInfo(*viewSpotVertex);

		cout << "\n\n";
		cout << "任意键返回菜单";
		system("pause");
		Menu(graph);
	}
}

void showViewSpotInfo(Vertex& viewSpot)
{
	cout<<"景点名字："+ viewSpot.name << endl;
	cout << "简介：" + viewSpot.introduce <<endl;
	cout << "景点等级：" << viewSpot.sceneryLevel << endl;
	cout << "景点位置  X:" << viewSpot.position.x << "   Y:" << viewSpot.position.y << endl;

}

void showViewSpotNeighborInfo(Vertex& viewSpot)
{
	CVector<Neighbor>& neighbors = viewSpot.neighbors;
	bool hasPrintDirection = false;
	for (UINT i = 0; i < neighbors.length; i++)
	{

		if (neighbors[i].direction == Weast)
		{
			if (!hasPrintDirection)
			{
				cout << "向西：";
				hasPrintDirection = true;
			}
			cout << neighbors[i].neighbor->name << "\t";
		}
	}
	hasPrintDirection = false;
	for (UINT i = 0; i < neighbors.length; i++)
	{

		if (neighbors[i].direction == North)
		{
			if (!hasPrintDirection)
			{
				cout << "向北：";
				hasPrintDirection = true;
			}
			cout << neighbors[i].neighbor->name << "\t";
		}

	}
	cout << endl;

	hasPrintDirection = false;
	for (UINT i = 0; i < neighbors.length; i++)
	{

		if (neighbors[i].direction == East)
		{
			if (!hasPrintDirection)
			{
				cout << "向东：";
				hasPrintDirection = true;
			}
			cout << neighbors[i].neighbor->name << "\t";
		}

	}
	cout << endl;

	hasPrintDirection = false;
	for (UINT i = 0; i < neighbors.length; i++)
	{

		if (neighbors[i].direction == South)
		{
			if (!hasPrintDirection)
			{
				cout << "向南：";
				hasPrintDirection = true;
			}
			cout << neighbors[i].neighbor->name << "\t";
		}

	}
}
void showPathInfo(string strViewSpot1, string strViewSpot2, EdgeNode* path)
{
	cout << "道路：" + strViewSpot1 + "<--->" + strViewSpot2 << endl;
	cout << "道路类型：";
	if (path->path == CarPath)
		cout << "车行道\n";
	else
		cout << "人行道\n";
	cout << "道路长度：" << path->length << "米\n";
}
void searchSchoolArticul(UndirectedGraph& graph)
{
	system("cls");
	cout <<"校园关节点有："<<endl;
	CVector<Vertex*> articuls;
	graph.FindArticul(articuls);
	for (UINT i = 0; i < articuls.length; i++)
		cout << articuls[i]->name << "\t";
	
	cout << "\n\n任意键返回主菜单\n" << endl;

	system("pause");
	Menu(graph);
}

void findPathFromTwoViewSpot(UndirectedGraph& graph)
{
	system("cls");
	string start;
	string target;
	cout << "输入起始点：";

	
	cin >> start;
	cout << "\n输入目的地：";
	cin >> target;
	cout << "\n";
	Vertex* vStart = graph.GetVex(start);
	Vertex* vTarget = graph.GetVex(target);

	//景点不存在
	if (vStart == NULL || vTarget == NULL)
	{
		cout << endl;
		if (vStart == NULL)
			cout << "景点 " + start + "不存在" << endl;
		else
			cout << "景点 " + target + "不存在" << endl;
		cout << "\n继续按‘y’,其它返回主菜单";
		cout << "是否继续：";
		
		string select;
		cin >> select;
		if (select[0] == 'y')
			findPathFromTwoViewSpot(graph);
		else
			Menu(graph);
	}
	findPathFromTwoViewSpot(graph,start, target);
}

void findPathFromTwoViewSpot(UndirectedGraph& graph,string start, string target)
{

	system("cls");
	cout << "起点：" + start << endl;
	cout << "目的地：" + target + "\n\n";

	cout << "查询模式：\n";
	cout << "1.最短路径\n";
	cout << "2.沿途景色等级最高\n";
	cout << "3.所有路径\n";
	cout << "4.车行道\n";
	cout << "5.人行道\n";
	cout << "6.返回主菜单\n\n" << endl;

	CVector<CVector<Vertex*>> pathes;
	CVector<Vertex*> shortPath;
	string select;
	cout << "选择模式:";
	cin >> select;
	cout << "\n\n";
	switch (select[0])
	{
	case '1':	graph.ShortestPath(start, target, shortPath);
				cout << "最短路径：\n";
				break;
	case '2':	graph.FindPathBySceneryLevel(start,target,pathes);
				cout << "景色最佳的路径有:\n";
				break;
	case '3':	graph.FindAllPath(start, target, pathes);
				cout << "所有路径：\n";
				break;
	case '4':	graph.FindPathByPathType(start, target, CarPath, pathes);
				cout << "到达目的地的车行道有：\n";
				break;
	case '5':	graph.FindPathByPathType(start, target, FootPath, pathes);
				cout << "到达目的地的人行道有：\n";
				break;
	case '6':	Menu(graph);
				break;
	default:
		cout << "无此选项。\n";
		system("pause");
		findPathFromTwoViewSpot(graph,start, target);
	}
	

	bool isShowStart = true;//用于开头时，检测是否需要显示"->"这个符号。
	//最短路径和其它路径模式所用的数组形式是不同的
	if (select[0] == '1')
	{
		if (shortPath.length <= 0)
			cout << "无路径\n";
		else
		{
			for (UINT i = 0; i < shortPath.length; i++)
			{
				if (isShowStart)
					isShowStart = false;
				else
					cout << "->";
				cout << shortPath[i]->name;
			}
		}
	}
	else
	{
		if(pathes.length<=0)
			cout << "无路径\n";
		else {
			CVector<Vertex*>* path;
			for (UINT i = 0; i < pathes.length; i++)
			{
				isShowStart = true;
				path = &pathes[i];
				cout << "第"<<i<<"条:";
				for (UINT j = 0; j < path->length; j++)
				{
					if (isShowStart)
						isShowStart = false;
					else
						cout << "->";
					cout << (*path)[j]->name;
				}
				cout << "\n";
			}
		}
	}

	cout << "\n\n";
	cout << "按'y'继续查询，其余返回主菜单：";
	cin >> select;
	if (select[0] == 'y')
		findPathFromTwoViewSpot(graph, start, target);
	else
		Menu(graph);
}


void changeViewSpotInfo(UndirectedGraph& graph)
{
	system("cls");
	string viewSpot;
	cout << "输入要修改的景点:";
	cin >> viewSpot;

	Vertex* viewSpotVertex = graph.GetVex(viewSpot);
	if (viewSpotVertex == NULL)
	{
		cout << "无此景点。" << endl;
		cout << "是否继续查询？" << endl;
		cout << "确定按‘y’,其他放回菜单";

		string select;
		cin >> select;
		if (select[0] == 'y')
			showViewSpot(graph);
		else
			Menu(graph);
	}
	else {
		changeViewSpotInfo(graph, viewSpotVertex);
	}
}

void changeViewSpotInfo(UndirectedGraph& graph, Vertex* viewSpot)
{
	showViewSpotInfo(*viewSpot);
	cout << "\n";
	cout << "输入修改信息（按‘#’表示该项不修改）：";

	Vertex temp = *viewSpot;
	string changeInfo;
ViewSpotName:
	cout << "\n景点名字：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (graph.GetVex(changeInfo) != NULL)
		{
			cout << "景点名重复，请重新输入！\n";
			goto ViewSpotName;
		}
		else
			temp.name = changeInfo;
	}
		

	cout << "\n景点简介：";
	cin >> temp.introduce;
	if (changeInfo[0] != '#')
		temp.introduce = changeInfo;

ViewSpotLevel:
	cout << "\n景点等级：";
	cin >> changeInfo;
	int aInt;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "含有非数字字符，请重新输入；\n";
			goto ViewSpotLevel;

		}
		else
			temp.sceneryLevel = aInt;
	}

ViewSpotPositionX:
	cout << "\n景点地点 X（正数）：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "含有非数字字符，请重新输入；\n";
			goto ViewSpotPositionX;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX需要>=0；\n";
				goto ViewSpotPositionX;
			}
			else
				temp.position.x = aInt;
		}
	}
ViewSpotPositionY:
	cout << "\n景点地点 Y（正数）：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "含有非数字字符，请重新输入；\n";
			goto ViewSpotPositionY;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX需要>=0；\n";
				goto ViewSpotPositionY;
			}
			else
				temp.position.x = aInt;
		}
	}
	cout << "\n确认修改按‘y’,重新输入按‘a’,其它键返回主菜单:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		*viewSpot = temp;
		showViewSpotInfo(*viewSpot);
		cout << "\n\n任意键放回菜单\n";
		system("pause");
		Menu(graph);
	}
	else if (select[0] == 'a')
		changeViewSpotInfo(graph, viewSpot);
	else
		Menu(graph);
}
void addViewSpotInfo(UndirectedGraph&graph)
{
	system("cls");
	Vertex vertex;
	string changeInfo;
	cout << "录入景点\n" << endl;
Go_AddName:
	cout << "\n景点名字：";
	cin >>changeInfo ;
	if (changeInfo[0] != '#')
	{
		if (graph.GetVex(changeInfo) != NULL)
		{
			cout << "景点名重复，请重新输入！\n";
			goto Go_AddName;
		}
		else
			vertex.name = changeInfo;
	}
	cout << "\n景点简介：";
	cin >> vertex.introduce;
	
Go_AddSeceneryLevel:
	int aInt;
	cout << "\n景点等级：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "含有非数字字符，请重新输入；\n";
			goto Go_AddSeceneryLevel;

		}
		else
			vertex.sceneryLevel = aInt;
	}
Go_AddViewSpotPosX:
	cout << "\n景点地点 X（正数）：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "含有非数字字符，请重新输入；\n";
			goto Go_AddViewSpotPosX;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX需要>=0；\n";
				goto Go_AddViewSpotPosX;
			}
			else
				vertex.position.x = aInt;
		}
	}
Go_AddViewSpotPosY:
	cout << "\n景点地点 Y（正数）：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "含有非数字字符，请重新输入；\n";
			goto Go_AddViewSpotPosY;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX需要>=0；\n";
				goto Go_AddViewSpotPosY;
			}
			else
				vertex.position.y = aInt;
		}
	}
	cout << "\n确认添加按‘y’,重新输入按‘a’,其它键返回主菜单:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		graph.AddVertex(vertex);
		showViewSpotInfo(vertex);
		cout << "\n继续按‘y’,其它键返回主菜单:";
		cin >> select;
		if (select[0] == 'y')
			addViewSpotInfo(graph);
		else
			Menu(graph);
	}
	else if (select[0] == 'a')
		addViewSpotInfo(graph);
	else
		Menu(graph);
}
void addPathInfo(UndirectedGraph& graph)
{
	system("cls");
	string strViewSpot1, strViewSpot2;
	EdgeNode* edge = NULL;
	cout << "添加边\n";
	cout << "输入两个连接的景点：";
	cin >> strViewSpot1;
	cin >> strViewSpot2;
	if (strViewSpot1 == strViewSpot2)
	{
		cout << "\n景点不能自连接!" << endl;
		system("pause");
		Menu(graph);
	}
	//edge = graph.GetEdge(strViewSpot1, strViewSpot2);
	//if ((edge == NULL))
		//addPathInfo(graph, strViewSpot1, strViewSpot2);
	Vertex* temp1 = NULL, *temp2 = NULL;
	temp1 = graph.GetVex(strViewSpot1);
	temp2 = graph.GetVex(strViewSpot2);
	if (temp1 == NULL || temp2 == NULL)
	{
		cout << "\n没有景点:";
		if (temp1 == NULL)
			cout << strViewSpot1 + "  ";
		if (temp2 == NULL)
			cout << strViewSpot2 + "  ";
		cout << "\n";
		string select;
		cout << "重新输入按‘y’,其余返回主菜单:";
		cin >> select;
		if (select[0] == 'y')
			addPathInfo(graph);
		else
			Menu(graph);
	}
	addPathInfo(graph,strViewSpot1,strViewSpot2);
}
void addPathInfo(UndirectedGraph& graph, string strViewSpot1, string strViewSpot2)
{
	system("cls");
	cout << "增加道路：" + strViewSpot1 + "<--->" + strViewSpot2 << endl;
	EdgeNode  temp;
	string changeInfo;

	cout << "道路类型（1为车行道，2（及其它）为人行道）：";
	cin >> changeInfo;
	switch (changeInfo[0])
	{
	case '1':temp.path = CarPath;
		break;
	default:temp.path = FootPath;
	}
	int aInt;
Go_AddPathLength:
	cout << "道路长度：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "\n输入含有非数字字符,重新输入\n";
			goto Go_AddPathLength;
		}
		else
		{
			if (aInt <= 0)
			{
				cout << "\n道路长度不可以小于0，重新输入\n";
				goto Go_AddPathLength;
			}
			else
				temp.length = aInt;
		}
	}


	cout << "\n确认添加按‘y’,重新输入按‘a’,其它键返回主菜单:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		graph.AddEdge(strViewSpot1, strViewSpot2, temp);

		cout << "\n继续按‘y’，其它返回主菜单\n";
		cin >> select;
		if (select[0] == 'y')
			addPathInfo(graph);
		else
			Menu(graph);
	}
	else if (select[0] == 'a')
		addPathInfo(graph, strViewSpot1, strViewSpot2);
	else
		Menu(graph);

}
void changePathInfo(UndirectedGraph& graph)
{
	system("cls");
	string strViewSpot1, strViewSpot2;
	EdgeNode* edge = NULL;
	cout << "输入两个连接的景点：";
	cin >> strViewSpot1;
	cin >> strViewSpot2;

	edge = graph.GetEdge(strViewSpot1, strViewSpot2);
	if ((edge != NULL))
		changePathInfo(graph,strViewSpot1,strViewSpot2,edge);
	else 
	{
		bool isVertexProble = false;
		Vertex* temp1 = NULL,*temp2 = NULL;
		temp1 = graph.GetVex(strViewSpot1);
		temp2 = graph.GetVex(strViewSpot2);
		if (temp1 == NULL || temp2 == NULL)
		{
			isVertexProble = true;
			cout << "\n没有景点:";
			if (temp1 == NULL)
				cout << strViewSpot1 + "  ";
			if (temp2 == NULL)
				cout << strViewSpot2 + "  ";
			cout << "\n";
		}
		if (!isVertexProble)
			cout << "\n这两个景点之间没有路径\n "<< endl;
		string select;
		cout << "重新输入按‘y’,其余返回主菜单:";
		cin >> select;
		if (select[0] == 'y')
			changePathInfo(graph);
		else
			Menu(graph);
	}
}
void changePathInfo(UndirectedGraph& graph,
	string strViewSpot1, string strViewSpot2,
	EdgeNode* path)
{
	system("cls");
	showPathInfo(strViewSpot1, strViewSpot2, path);
	cout << "\n";
	cout << "输入修改信息（按‘#’表示该项不修改）：\n";
	EdgeNode  temp = *path;
	string changeInfo;

	cout << "修改道路类型（1为车行道，2为人行道，其它不变）：";
	cin >> changeInfo;
	switch (changeInfo[0])
	{
	case '1':temp.path = CarPath;
		break;
	case '2': temp.path = FootPath;
		break;
	}
	
	int aInt;
ChangeLenght:
	cout << "修改道路长度：";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "\n输入含有非数字字符,重新输入\n";
			goto ChangeLenght;
		}
		else
		{
			if (aInt <= 0)
			{
				cout << "\n道路长度不可以小于0，重新输入\n";
				goto ChangeLenght;
			}
			else
				temp.length = aInt;
		}
	}
	

	cout << "\n确认修改按‘y’,重新输入按‘a’,其它键返回主菜单:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		*path = temp;
		cout << "\n\n任意键放回菜单\n";
		system("pause");
		Menu(graph);
	}
	else if (select[0] == 'a')
		changePathInfo(graph, strViewSpot1, strViewSpot2, path);
	else
		Menu(graph);
	
}
void removeViewSpot(UndirectedGraph& graph)
{
	system("cls");
	cout << "输入要删除的景点：";
	string strViewSpot;
	cin >> strViewSpot;
	cout << "\n\n";
	Vertex* viewSpot = graph.GetVex(strViewSpot);
	string select;
	if (viewSpot != NULL)
	{
		showViewSpotInfo(*viewSpot);
		cout << "是否确认删除('y'确认):";
		cin >> select;
		if (select[0] == 'y')
			graph.DeleteVertex(strViewSpot);
		else if (select[0] == 'a')
			removeViewSpot(graph);
		else
			Menu(graph);
	}
	else {
		cout << "没有这个景点，删除失败。" << endl;
	}
	cout << "继续输入按‘y’,其余返回主菜单:";
	cin >> select;
	if (select[0] == 'y')
		removeViewSpot(graph);
	else
		Menu(graph);
}
void removePath(UndirectedGraph& graph)
{
	system("cls");
	string strViewSpot1, strViewSpot2;
	EdgeNode* edge = NULL;
	cout << "输入两个连接的景点：";
	cin >> strViewSpot1;
	cin >> strViewSpot2;

	cout << endl;//换行
	edge = graph.GetEdge(strViewSpot1, strViewSpot2);
	if (edge != NULL)
	{	
		showPathInfo(strViewSpot1, strViewSpot2, edge);
		cout << "是否确认删除('y'确认)";
		string select;
		cin >> select;
		if (select[0] == 'y')
		{
			graph.DeleteEdge(strViewSpot1, strViewSpot2);
			cout << "删除成功\n\n";
		}	
	}
	else
	{
		bool isVertexProble = false;
		Vertex* temp1 = NULL, *temp2 = NULL;
		temp1 = graph.GetVex(strViewSpot1);
		temp2 = graph.GetVex(strViewSpot2);
		if (temp1 == NULL || temp2 == NULL)
		{
			isVertexProble = true;
			cout << "\n没有景点:";
			if (temp1 == NULL)
				cout << strViewSpot1 + "  ";
			if (temp2 == NULL)
				cout << strViewSpot2 + "  ";
			cout << "\n";
		}
		if (!isVertexProble)
			cout << "\n这两个景点之间没有路径\n " << endl;
	}

	string select;
	cout << "继续按‘y’,其余返回主菜单:";
	cin >> select;
	if (select[0] == 'y')
		removePath(graph);
	else
		Menu(graph);
}
void traverse(Vertex *v)
{
	cout << v->name << endl;
}

void Init(UndirectedGraph* graph)
{
	Vertex node;
	EdgeNode edge;
	node.name = "世界";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "美国";
	edge.length = 1800;
	graph->AddVertex(node);
	graph->AddEdge("世界", "美国", edge);


	node.name = "中国";
	edge.length = 2016;
	graph->AddVertex(node);
	graph->AddEdge("世界", "中国", edge);


	node.name = "俄罗斯";
	edge.length = 2000;
	graph->AddVertex(node);
	graph->AddEdge("中国", "俄罗斯", edge);

	node.name = "墨西哥";
	edge.length = 1000;
	graph->AddVertex(node);
	graph->AddEdge("墨西哥", "美国", edge);

	node.name = "日本";
	edge.length = 2012;
	graph->AddVertex(node);
	graph->AddEdge("日本", "中国", edge);

	node.name = "印度";
	edge.length = 23;
	graph->AddVertex(node);
	graph->AddEdge("印度", "日本", edge);
	edge.length = 90;
}

void InitForArticul(UndirectedGraph* graph)
{
	Vertex node;
	EdgeNode edge;
	node.name = "世界";
	//node.introduce = "包含物质和世界，就是世界1";
	node.weight = 10000;
	graph->AddVertex(node);
	node.name = "世界1";
	node.weight = 10000;
	//node.introduce = "包含物质和世界，就是世界1";
	graph->AddVertex(node);
	graph->AddEdge("世界", "世界1", edge);

	node.name = "美国";
	edge.length = 1800;
	graph->AddVertex(node);
	graph->AddEdge("世界", "美国", edge);


	node.name = "中国";
	edge.length = 2016;
	graph->AddVertex(node);
	graph->AddEdge("世界1", "中国", edge);

	node.name = "日本";
	edge.length = 2012;
	graph->AddVertex(node);
	graph->AddEdge("世界1", "日本", edge);

	node.name = "墨西哥";
	edge.length = 1000;
	graph->AddVertex(node);
	graph->AddEdge("墨西哥", "世界", edge);
}

void InitForAllPath(UndirectedGraph* graph)
{
	Vertex node;
	EdgeNode edge;
	node.name = "世界";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "中国";
	node.weight = 10000;
	node.sceneryLevel = 5;
	graph->AddVertex(node);

	node.name = "日本";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "俄罗斯";
	node.sceneryLevel = 10;
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "哈尼亚";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "世界另一端";
	node.weight = 10000;
	graph->AddVertex(node);

	edge.length = 10;
	graph->AddEdge("世界", "中国", edge);
	graph->AddEdge("世界另一端", "中国", edge);

	edge.length = 10;
	graph->AddEdge("世界", "日本", edge);
	graph->AddEdge("世界另一端", "日本", edge);

	edge.length = 10;
	graph->AddEdge("世界", "哈尼亚", edge);
	graph->AddEdge("世界另一端", "哈尼亚", edge);

	edge.length = 10;
	graph->AddEdge("世界", "俄罗斯", edge);
	graph->AddEdge("世界另一端", "俄罗斯", edge);
}

bool safeStringToInt(string str, int& num)
{
	
	int tempNum = 0;
	UINT i = 0;
	//是否负数检测
	if (str[0] == '-')
	{
		if (str.length() < 2)
			return false;
		else
			i = 1;
	}
	//如果数据大于这个int的最大值2147483647就结束，注意通过就会*10，所以小于214748364才可以
	for (; i <str.length()&&abs(tempNum)<= 21474836; i++)
	{
		if (str[i] > '9' || str[i] < '0')
			return false;
		tempNum = tempNum * 10 + str[i] - '0';
	}
	if (str[0] == '-')
		num = -tempNum;
	else
		num = tempNum;
	return true;
}
void saveGraphInfo(UndirectedGraph& graph)
{
	ofstream outFile("graphInfo.txt");
	if (!outFile)
	{
		cerr << "Create graphInfo.txt false" << endl;
		return;
	}
	int vertexNum = graph.GetVertexNum();
	outFile << vertexNum << endl;
	Vertex* temp;
	for (int i = 1; i <= vertexNum; i++)
	{
		temp = graph.GetVex(i);
		outFile << temp->name << endl;
		if (temp->introduce == "")
			outFile << "无简介" << endl;
		else
			outFile << temp->introduce << endl;
		outFile << temp->position.x << "\t" << temp->position.y << "\t"<< temp->sceneryLevel << endl;
	}
	
	int edgeNum = graph.GetEdgeNum();
	outFile << edgeNum << endl;
	
	EdgeNode* edge;
	for (int i = 1; i <= edgeNum; i++)
	{
		edge = graph.GetEdge(i); 
		//边连接双方的景点名字
		outFile << edge->vertex1->name << "\t" << edge->vertex2->name<< endl;
		//道路长度，等级和类型
		outFile << edge->length << "\t" << edge->sceneryLevel << "\t";
		if (edge->path == CarPath)
			outFile << 1 << endl;
		else
			outFile << 2 << endl;
	}
	outFile.close();
}
void CreateGraphFromFile(UndirectedGraph& graph,string fileName)
{
	ifstream fin(fileName);
	if (!fin.is_open())
	{
		cout << "Error opening file" << endl;
		getchar();
		exit(1);
	}
	int vertexNum;
	fin >> vertexNum;
	cout << "vertexNum=" << vertexNum << endl;
	Vertex temp;
	for (int i = 0; i < vertexNum;i++)
	{
		fin >> temp.name >> temp.introduce>> temp.position.x >> temp.position.y >> temp.sceneryLevel;
		cout << "name:" + temp.name << "introduce" + temp.introduce << endl;
		graph.AddVertex(temp);
	}
	int edgeNum;
	fin >> edgeNum;
	EdgeNode edge;
	string name1, name2;
	int pathType;
	for (int i = 0; i < vertexNum; i++)
	{
		fin >> name1 >> name2;
		fin >> edge.length >> edge.sceneryLevel;
		fin >> pathType;
		if (pathType == 1)
			edge.path = CarPath;
		else
			edge.path = FootPath;
		graph.AddEdge(name1, name2,edge);
	}
	fin.close();
}
void outtextxy(int x, int y, string& text) {
	wchar_t* wtext;
	char * temp = const_cast<char*>(text.c_str());
	int length = text.length();
	int len = MultiByteToWideChar(CP_ACP, 0, temp,length,NULL,0);
	wtext = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, temp, length, wtext, len);
	wtext[len] = '\0';
	outtextxy(x,y,wtext);
	delete[] wtext;
}
void showAllViewSpots(UndirectedGraph& graph)
{
	initgraph(800,500);
	setbkcolor(WHITE);
	
	settextcolor(RGB(0,0,0));cleardevice();
	int textHeight = textheight(L"霜");
	int textWidth = textwidth(L"霜");
	float heightOffset = (float)textHeight / 2;
	float widthOffset1 = 0;
	float widthOffset2 = 0;
	int edgeNum = graph.GetEdgeNum();
	EdgeNode* edge = NULL;
	for (int i = 1; i <= edgeNum; i++)
	{
		edge = graph.GetEdge(i);
		if (edge->path == CarPath)
			setlinecolor(BLUE);
		else
			setlinecolor(GREEN);
		widthOffset1 = (((float)edge->vertex1->name.length()-4)*textWidth) / 2;
		widthOffset2 = (((float)edge->vertex2->name.length()-4)*textWidth) / 2;
		line(edge->vertex1->position.x+widthOffset1, edge->vertex1->position.y+heightOffset, 
			edge->vertex2->position.x+widthOffset2, edge->vertex2->position.y+heightOffset);
	}
	Vertex* vertex = NULL;
	int vertexNum = graph.GetVertexNum();
	
	for (int i = 1; i <= vertexNum; i++)
	{
		vertex = graph.GetVex(i);
		outtextxy((int)vertex->position.x, (int)vertex->position.y, vertex->name);
	}
	settextcolor(RED);
	outtextxy(600, 10, L"车行道");
	setlinecolor(BLUE);
	line(650, 10 + heightOffset, 750, 10 + heightOffset);
	outtextxy(600, 50, L"人行道");
	setlinecolor(GREEN);
	line(650, 50 + heightOffset, 750, 50 + heightOffset);
	getchar();
	getchar();
	getchar();
	getchar();
	closegraph();
}