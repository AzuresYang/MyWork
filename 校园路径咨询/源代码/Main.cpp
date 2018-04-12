#include"UndirectedGraph.h"
#include<assert.h>
#include<windows.h>
#include<fstream>
#include<graphics.h>//ͼ�ο�
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
// �����麯������

void outtextxy(int x , int y, string& text);
void gotoXY(int x, int y) {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = {x,y};
	SetConsoleCursorPosition(hout, c);
}
template <class Type>//��string����ת��Ϊtype����������
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
		cout << "Write By---3115005248--15��һŷ���ľ�" << endl;
		cout << "У԰�����ѯϵͳ" << endl;
		cout << "1.��ʾУ԰��ͼ" << endl;
		cout << "2.��ѯ������Ϣ" << endl;
		cout << "3.��ѯУ԰�ؽڵ�" << endl;
		cout << "4.��ѯ���������·��" << endl;
		cout << "5.�޸ľ�����Ϣ" << endl;
		cout << "6.�޸�·����Ϣ" << endl;
		cout << "7.ɾ������" << endl;
		cout << "8.ɾ��·��" << endl;
		cout << "9.���Ӿ���" << endl;
		cout << "a.����·��" << endl;
		cout << "b.����" << endl;
		cout << "\n\n"<< endl;
		cout << "ѡ���ܣ�";
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
		default:cout << "�޴˹��ܣ����������롣\n�����������" << endl;
			system("pause");
			Menu(graph);
		}
	}
}


//���뾰�㣬�������������㣬����ʾ��
void showViewSpot(UndirectedGraph& graph) {
	system("cls");
	string viewSpot;
	cout << "�����ѯ�ľ���:";
	cin >> viewSpot;

	Vertex* viewSpotVertex = graph.GetVex(viewSpot);
	if (viewSpotVertex == NULL)
	{
		cout << "�޴˾��㡣" << endl;
		cout << "�Ƿ������ѯ��" << endl;
		cout << "ȷ������y��,�����Żز˵�";

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
		cout << "��������ز˵�";
		system("pause");
		Menu(graph);
	}
}

void showViewSpotInfo(Vertex& viewSpot)
{
	cout<<"�������֣�"+ viewSpot.name << endl;
	cout << "��飺" + viewSpot.introduce <<endl;
	cout << "����ȼ���" << viewSpot.sceneryLevel << endl;
	cout << "����λ��  X:" << viewSpot.position.x << "   Y:" << viewSpot.position.y << endl;

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
				cout << "������";
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
				cout << "�򱱣�";
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
				cout << "�򶫣�";
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
				cout << "���ϣ�";
				hasPrintDirection = true;
			}
			cout << neighbors[i].neighbor->name << "\t";
		}

	}
}
void showPathInfo(string strViewSpot1, string strViewSpot2, EdgeNode* path)
{
	cout << "��·��" + strViewSpot1 + "<--->" + strViewSpot2 << endl;
	cout << "��·���ͣ�";
	if (path->path == CarPath)
		cout << "���е�\n";
	else
		cout << "���е�\n";
	cout << "��·���ȣ�" << path->length << "��\n";
}
void searchSchoolArticul(UndirectedGraph& graph)
{
	system("cls");
	cout <<"У԰�ؽڵ��У�"<<endl;
	CVector<Vertex*> articuls;
	graph.FindArticul(articuls);
	for (UINT i = 0; i < articuls.length; i++)
		cout << articuls[i]->name << "\t";
	
	cout << "\n\n������������˵�\n" << endl;

	system("pause");
	Menu(graph);
}

void findPathFromTwoViewSpot(UndirectedGraph& graph)
{
	system("cls");
	string start;
	string target;
	cout << "������ʼ�㣺";

	
	cin >> start;
	cout << "\n����Ŀ�ĵأ�";
	cin >> target;
	cout << "\n";
	Vertex* vStart = graph.GetVex(start);
	Vertex* vTarget = graph.GetVex(target);

	//���㲻����
	if (vStart == NULL || vTarget == NULL)
	{
		cout << endl;
		if (vStart == NULL)
			cout << "���� " + start + "������" << endl;
		else
			cout << "���� " + target + "������" << endl;
		cout << "\n��������y��,�����������˵�";
		cout << "�Ƿ������";
		
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
	cout << "��㣺" + start << endl;
	cout << "Ŀ�ĵأ�" + target + "\n\n";

	cout << "��ѯģʽ��\n";
	cout << "1.���·��\n";
	cout << "2.��;��ɫ�ȼ����\n";
	cout << "3.����·��\n";
	cout << "4.���е�\n";
	cout << "5.���е�\n";
	cout << "6.�������˵�\n\n" << endl;

	CVector<CVector<Vertex*>> pathes;
	CVector<Vertex*> shortPath;
	string select;
	cout << "ѡ��ģʽ:";
	cin >> select;
	cout << "\n\n";
	switch (select[0])
	{
	case '1':	graph.ShortestPath(start, target, shortPath);
				cout << "���·����\n";
				break;
	case '2':	graph.FindPathBySceneryLevel(start,target,pathes);
				cout << "��ɫ��ѵ�·����:\n";
				break;
	case '3':	graph.FindAllPath(start, target, pathes);
				cout << "����·����\n";
				break;
	case '4':	graph.FindPathByPathType(start, target, CarPath, pathes);
				cout << "����Ŀ�ĵصĳ��е��У�\n";
				break;
	case '5':	graph.FindPathByPathType(start, target, FootPath, pathes);
				cout << "����Ŀ�ĵص����е��У�\n";
				break;
	case '6':	Menu(graph);
				break;
	default:
		cout << "�޴�ѡ�\n";
		system("pause");
		findPathFromTwoViewSpot(graph,start, target);
	}
	

	bool isShowStart = true;//���ڿ�ͷʱ������Ƿ���Ҫ��ʾ"->"������š�
	//���·��������·��ģʽ���õ�������ʽ�ǲ�ͬ��
	if (select[0] == '1')
	{
		if (shortPath.length <= 0)
			cout << "��·��\n";
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
			cout << "��·��\n";
		else {
			CVector<Vertex*>* path;
			for (UINT i = 0; i < pathes.length; i++)
			{
				isShowStart = true;
				path = &pathes[i];
				cout << "��"<<i<<"��:";
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
	cout << "��'y'������ѯ�����෵�����˵���";
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
	cout << "����Ҫ�޸ĵľ���:";
	cin >> viewSpot;

	Vertex* viewSpotVertex = graph.GetVex(viewSpot);
	if (viewSpotVertex == NULL)
	{
		cout << "�޴˾��㡣" << endl;
		cout << "�Ƿ������ѯ��" << endl;
		cout << "ȷ������y��,�����Żز˵�";

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
	cout << "�����޸���Ϣ������#����ʾ����޸ģ���";

	Vertex temp = *viewSpot;
	string changeInfo;
ViewSpotName:
	cout << "\n�������֣�";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (graph.GetVex(changeInfo) != NULL)
		{
			cout << "�������ظ������������룡\n";
			goto ViewSpotName;
		}
		else
			temp.name = changeInfo;
	}
		

	cout << "\n�����飺";
	cin >> temp.introduce;
	if (changeInfo[0] != '#')
		temp.introduce = changeInfo;

ViewSpotLevel:
	cout << "\n����ȼ���";
	cin >> changeInfo;
	int aInt;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "���з������ַ������������룻\n";
			goto ViewSpotLevel;

		}
		else
			temp.sceneryLevel = aInt;
	}

ViewSpotPositionX:
	cout << "\n����ص� X����������";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "���з������ַ������������룻\n";
			goto ViewSpotPositionX;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX��Ҫ>=0��\n";
				goto ViewSpotPositionX;
			}
			else
				temp.position.x = aInt;
		}
	}
ViewSpotPositionY:
	cout << "\n����ص� Y����������";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "���з������ַ������������룻\n";
			goto ViewSpotPositionY;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX��Ҫ>=0��\n";
				goto ViewSpotPositionY;
			}
			else
				temp.position.x = aInt;
		}
	}
	cout << "\nȷ���޸İ���y��,�������밴��a��,�������������˵�:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		*viewSpot = temp;
		showViewSpotInfo(*viewSpot);
		cout << "\n\n������Żز˵�\n";
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
	cout << "¼�뾰��\n" << endl;
Go_AddName:
	cout << "\n�������֣�";
	cin >>changeInfo ;
	if (changeInfo[0] != '#')
	{
		if (graph.GetVex(changeInfo) != NULL)
		{
			cout << "�������ظ������������룡\n";
			goto Go_AddName;
		}
		else
			vertex.name = changeInfo;
	}
	cout << "\n�����飺";
	cin >> vertex.introduce;
	
Go_AddSeceneryLevel:
	int aInt;
	cout << "\n����ȼ���";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "���з������ַ������������룻\n";
			goto Go_AddSeceneryLevel;

		}
		else
			vertex.sceneryLevel = aInt;
	}
Go_AddViewSpotPosX:
	cout << "\n����ص� X����������";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "���з������ַ������������룻\n";
			goto Go_AddViewSpotPosX;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX��Ҫ>=0��\n";
				goto Go_AddViewSpotPosX;
			}
			else
				vertex.position.x = aInt;
		}
	}
Go_AddViewSpotPosY:
	cout << "\n����ص� Y����������";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "���з������ַ������������룻\n";
			goto Go_AddViewSpotPosY;

		}
		else
		{
			if (aInt < 0)
			{
				cout << "\nX��Ҫ>=0��\n";
				goto Go_AddViewSpotPosY;
			}
			else
				vertex.position.y = aInt;
		}
	}
	cout << "\nȷ����Ӱ���y��,�������밴��a��,�������������˵�:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		graph.AddVertex(vertex);
		showViewSpotInfo(vertex);
		cout << "\n��������y��,�������������˵�:";
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
	cout << "��ӱ�\n";
	cout << "�����������ӵľ��㣺";
	cin >> strViewSpot1;
	cin >> strViewSpot2;
	if (strViewSpot1 == strViewSpot2)
	{
		cout << "\n���㲻��������!" << endl;
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
		cout << "\nû�о���:";
		if (temp1 == NULL)
			cout << strViewSpot1 + "  ";
		if (temp2 == NULL)
			cout << strViewSpot2 + "  ";
		cout << "\n";
		string select;
		cout << "�������밴��y��,���෵�����˵�:";
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
	cout << "���ӵ�·��" + strViewSpot1 + "<--->" + strViewSpot2 << endl;
	EdgeNode  temp;
	string changeInfo;

	cout << "��·���ͣ�1Ϊ���е���2����������Ϊ���е�����";
	cin >> changeInfo;
	switch (changeInfo[0])
	{
	case '1':temp.path = CarPath;
		break;
	default:temp.path = FootPath;
	}
	int aInt;
Go_AddPathLength:
	cout << "��·���ȣ�";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "\n���뺬�з������ַ�,��������\n";
			goto Go_AddPathLength;
		}
		else
		{
			if (aInt <= 0)
			{
				cout << "\n��·���Ȳ�����С��0����������\n";
				goto Go_AddPathLength;
			}
			else
				temp.length = aInt;
		}
	}


	cout << "\nȷ����Ӱ���y��,�������밴��a��,�������������˵�:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		graph.AddEdge(strViewSpot1, strViewSpot2, temp);

		cout << "\n��������y���������������˵�\n";
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
	cout << "�����������ӵľ��㣺";
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
			cout << "\nû�о���:";
			if (temp1 == NULL)
				cout << strViewSpot1 + "  ";
			if (temp2 == NULL)
				cout << strViewSpot2 + "  ";
			cout << "\n";
		}
		if (!isVertexProble)
			cout << "\n����������֮��û��·��\n "<< endl;
		string select;
		cout << "�������밴��y��,���෵�����˵�:";
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
	cout << "�����޸���Ϣ������#����ʾ����޸ģ���\n";
	EdgeNode  temp = *path;
	string changeInfo;

	cout << "�޸ĵ�·���ͣ�1Ϊ���е���2Ϊ���е����������䣩��";
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
	cout << "�޸ĵ�·���ȣ�";
	cin >> changeInfo;
	if (changeInfo[0] != '#')
	{
		if (!safeStringToInt(changeInfo, aInt))
		{
			cout << "\n���뺬�з������ַ�,��������\n";
			goto ChangeLenght;
		}
		else
		{
			if (aInt <= 0)
			{
				cout << "\n��·���Ȳ�����С��0����������\n";
				goto ChangeLenght;
			}
			else
				temp.length = aInt;
		}
	}
	

	cout << "\nȷ���޸İ���y��,�������밴��a��,�������������˵�:";
	string select;
	cin >> select;
	if (select[0] == 'y')
	{
		*path = temp;
		cout << "\n\n������Żز˵�\n";
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
	cout << "����Ҫɾ���ľ��㣺";
	string strViewSpot;
	cin >> strViewSpot;
	cout << "\n\n";
	Vertex* viewSpot = graph.GetVex(strViewSpot);
	string select;
	if (viewSpot != NULL)
	{
		showViewSpotInfo(*viewSpot);
		cout << "�Ƿ�ȷ��ɾ��('y'ȷ��):";
		cin >> select;
		if (select[0] == 'y')
			graph.DeleteVertex(strViewSpot);
		else if (select[0] == 'a')
			removeViewSpot(graph);
		else
			Menu(graph);
	}
	else {
		cout << "û��������㣬ɾ��ʧ�ܡ�" << endl;
	}
	cout << "�������밴��y��,���෵�����˵�:";
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
	cout << "�����������ӵľ��㣺";
	cin >> strViewSpot1;
	cin >> strViewSpot2;

	cout << endl;//����
	edge = graph.GetEdge(strViewSpot1, strViewSpot2);
	if (edge != NULL)
	{	
		showPathInfo(strViewSpot1, strViewSpot2, edge);
		cout << "�Ƿ�ȷ��ɾ��('y'ȷ��)";
		string select;
		cin >> select;
		if (select[0] == 'y')
		{
			graph.DeleteEdge(strViewSpot1, strViewSpot2);
			cout << "ɾ���ɹ�\n\n";
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
			cout << "\nû�о���:";
			if (temp1 == NULL)
				cout << strViewSpot1 + "  ";
			if (temp2 == NULL)
				cout << strViewSpot2 + "  ";
			cout << "\n";
		}
		if (!isVertexProble)
			cout << "\n����������֮��û��·��\n " << endl;
	}

	string select;
	cout << "��������y��,���෵�����˵�:";
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
	node.name = "����";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "����";
	edge.length = 1800;
	graph->AddVertex(node);
	graph->AddEdge("����", "����", edge);


	node.name = "�й�";
	edge.length = 2016;
	graph->AddVertex(node);
	graph->AddEdge("����", "�й�", edge);


	node.name = "����˹";
	edge.length = 2000;
	graph->AddVertex(node);
	graph->AddEdge("�й�", "����˹", edge);

	node.name = "ī����";
	edge.length = 1000;
	graph->AddVertex(node);
	graph->AddEdge("ī����", "����", edge);

	node.name = "�ձ�";
	edge.length = 2012;
	graph->AddVertex(node);
	graph->AddEdge("�ձ�", "�й�", edge);

	node.name = "ӡ��";
	edge.length = 23;
	graph->AddVertex(node);
	graph->AddEdge("ӡ��", "�ձ�", edge);
	edge.length = 90;
}

void InitForArticul(UndirectedGraph* graph)
{
	Vertex node;
	EdgeNode edge;
	node.name = "����";
	//node.introduce = "�������ʺ����磬��������1";
	node.weight = 10000;
	graph->AddVertex(node);
	node.name = "����1";
	node.weight = 10000;
	//node.introduce = "�������ʺ����磬��������1";
	graph->AddVertex(node);
	graph->AddEdge("����", "����1", edge);

	node.name = "����";
	edge.length = 1800;
	graph->AddVertex(node);
	graph->AddEdge("����", "����", edge);


	node.name = "�й�";
	edge.length = 2016;
	graph->AddVertex(node);
	graph->AddEdge("����1", "�й�", edge);

	node.name = "�ձ�";
	edge.length = 2012;
	graph->AddVertex(node);
	graph->AddEdge("����1", "�ձ�", edge);

	node.name = "ī����";
	edge.length = 1000;
	graph->AddVertex(node);
	graph->AddEdge("ī����", "����", edge);
}

void InitForAllPath(UndirectedGraph* graph)
{
	Vertex node;
	EdgeNode edge;
	node.name = "����";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "�й�";
	node.weight = 10000;
	node.sceneryLevel = 5;
	graph->AddVertex(node);

	node.name = "�ձ�";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "����˹";
	node.sceneryLevel = 10;
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "������";
	node.weight = 10000;
	graph->AddVertex(node);

	node.name = "������һ��";
	node.weight = 10000;
	graph->AddVertex(node);

	edge.length = 10;
	graph->AddEdge("����", "�й�", edge);
	graph->AddEdge("������һ��", "�й�", edge);

	edge.length = 10;
	graph->AddEdge("����", "�ձ�", edge);
	graph->AddEdge("������һ��", "�ձ�", edge);

	edge.length = 10;
	graph->AddEdge("����", "������", edge);
	graph->AddEdge("������һ��", "������", edge);

	edge.length = 10;
	graph->AddEdge("����", "����˹", edge);
	graph->AddEdge("������һ��", "����˹", edge);
}

bool safeStringToInt(string str, int& num)
{
	
	int tempNum = 0;
	UINT i = 0;
	//�Ƿ������
	if (str[0] == '-')
	{
		if (str.length() < 2)
			return false;
		else
			i = 1;
	}
	//������ݴ������int�����ֵ2147483647�ͽ�����ע��ͨ���ͻ�*10������С��214748364�ſ���
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
			outFile << "�޼��" << endl;
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
		//������˫���ľ�������
		outFile << edge->vertex1->name << "\t" << edge->vertex2->name<< endl;
		//��·���ȣ��ȼ�������
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
	int textHeight = textheight(L"˪");
	int textWidth = textwidth(L"˪");
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
	outtextxy(600, 10, L"���е�");
	setlinecolor(BLUE);
	line(650, 10 + heightOffset, 750, 10 + heightOffset);
	outtextxy(600, 50, L"���е�");
	setlinecolor(GREEN);
	line(650, 50 + heightOffset, 750, 50 + heightOffset);
	getchar();
	getchar();
	getchar();
	getchar();
	closegraph();
}