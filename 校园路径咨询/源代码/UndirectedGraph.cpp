#include"UndirectedGraph.h"


//����һ���ھ�
void Vertex::addNeighbor(Direction direction, Vertex* v, EdgeNode* edge) {
	Neighbor neighbor;
	neighbor.neighbor = v;
	neighbor.path = edge;
	long xDistance = v->position.x - position.x;
	long yDistance = v->position.y - position.y;
	if (abs(xDistance) > viewSpotScope)
	{
		if (xDistance > 0)
			neighbor.direction = East;
		else
			neighbor.direction = Weast;
	}
	else
	{
		if (yDistance > 0)
			neighbor.direction = North;
		else
			neighbor.direction = South;
	}
	neighbors.add(neighbor);
}
//Ѱ��һ�����㣬ʧ���򷵻�NULL
VertexNode* UndirectedGraph::FindVertex(string name)
{
	for (unsigned int i = 0; i < vertices.length; i++)
		if (vertices[i]->name == name)
			return vertices[i];
	return NULL;
}

//Desc:����һ������ĸ�����Ϊ�µĶ�������±�
//Return:û��v1,,v2���㣬���ʧ�ܣ�����G_ERROR;
//		��ӳɹ�������G_OK;

Status UndirectedGraph::AddVertex(Vertex& vertex)
{
	CVector<EdgeNode*> edgevector(vertices.length, NULL);//Ϊ�ڽӾ�������һ��
	PVertexNode vn = new VertexNode();//�½�һ������
	(*vn) = vertex;						//���Ƴ�Ϊ����
	vn->index = vertices.length;//�����±�
	vertices.add(vn);			//����붥������
	adjMatrix.add(edgevector);//���������new����� ����֮����������
	for(UINT i = 0; i < vertices.length; i++)//�����ڽӾ��������
		adjMatrix[i].add(NULL);
	return G_OK;
}

//Desc������һ���ߵĸ��������ԭ�������������бߣ��򸲸�֮
Status UndirectedGraph::AddEdge(string v1, string v2, EdgeNode& edge)
{
	int index1, index2;					//����Ƿ�������������
	PVertexNode vertex1,vertex2;
	vertex1 = FindVertex(v1);
	vertex2 = FindVertex(v2);
	if (vertex1 == NULL || vertex2 == NULL)
		return G_ERROR;

	index1 = vertex1->index;
	index2 = vertex2->index;
	if (adjMatrix[index1][index2] != NULL)		//ԭ���б��򸲸�֮
	{
		SAFE_DELETE(adjMatrix[index1][index2]);
		adjMatrix[index2][index1] = NULL;
	}

	EdgeNode* e = new EdgeNode();	//���ڽӾ������ӱ�			
	*e = edge;
	e->vertex1 = vertices[index1];
	e->vertex2 = vertices[index2];
	adjMatrix[index1][index2] = e;
	adjMatrix[index2][index1] = e;

	//Ϊ˫������ھ�
	Neighbor neighbor;
	neighbor.path = e;
	neighbor.neighbor = vertex1;
	//�������direction;
	long xDistance = vertex1->position.x - vertex2->position.x;
	long yDistance = vertex1->position.y - vertex2->position.y;
	if (abs(xDistance) > viewSpotScope)
	{
		if (xDistance > 0)
		{
			neighbor.direction = East;
			vertex2->neighbors.add(neighbor);

			neighbor.neighbor = vertex2;
			neighbor.direction = Weast;
			vertex1->neighbors.add(neighbor);
		}	
		else
		{
			neighbor.direction = Weast;
			vertex2->neighbors.add(neighbor);

			neighbor.neighbor = vertex2;
			neighbor.direction = East;
			vertex1->neighbors.add(neighbor);
		}
	}
	else
	{
		if (yDistance > 0)
		{
			neighbor.direction = North;
			vertex2->neighbors.add(neighbor);

			neighbor.neighbor = vertex2;
			neighbor.direction = South;
			vertex1->neighbors.add(neighbor);
		}
		else
		{
			neighbor.direction = South;
			vertex2->neighbors.add(neighbor);

			neighbor.neighbor = vertex2;
			neighbor.direction = North;
			vertex1->neighbors.add(neighbor);
		}
	}
	return G_OK;
}

//Desc��ɾ��һ���������֮�йصıߣ�
Status UndirectedGraph::DeleteVertex(string name)
{
	PVertexNode vertex;
	vertex = FindVertex(name);
	if (vertex == NULL)
		return G_ERROR;
	else
	{
		DeleteVertex(vertex->index);
		return G_OK;
	}
}

//Desc��ɾ��һ���������֮�йصı�
Status UndirectedGraph::DeleteVertex(unsigned int index)
{
	SAFE_DELETE(vertices[index]);
	vertices.erase(index);			//ɾ������

	CVector<EdgeNode*>* en = &adjMatrix[index];//�ͷ��������������
	for (UINT i = 0; i < en->length; i++)
	{
		SAFE_DELETE(en[index][i]);
	}

	adjMatrix.erase(index);//
	for (UINT i = 0; i < vertices.length; i++)//�ͷŸ��������������
	{
		adjMatrix[i][index] = NULL;//�ϱ��Ѿ�ɾ������
		en = &adjMatrix[i];
		en->erase(index);
	}
	for (UINT i = 0; i < vertices.length; i++)//���°󶨶����±�
		vertices[i]->index = i;
	return G_OK;
}

//Desc��ɾ��һ���ߣ�ͬʱ��˫�����ھӹ�ϵɾ��֮
Status UndirectedGraph::DeleteEdge(string v1, string v2)
{
	int v, w;
	PVertexNode vertex1, vertex2;	//����Ƿ���������
	vertex1 = FindVertex(v1);
	vertex2 = FindVertex(v2);
	if (vertex1 == NULL || vertex2 == NULL)
		return G_ERROR;
	v = vertex1->index;
	w = vertex2->index;

	//ɾ���ھӹ�ϵ

	for (UINT i = 0; i < vertex1->neighbors.length; i++)
		if (vertex2->name == vertex1->neighbors[i].neighbor->name)
			vertex1->neighbors.erase(i);

	for (UINT i = 0; i < vertex2->neighbors.length; i++)
		if (vertex1->name == vertex2->neighbors[i].neighbor->name)
			vertex2->neighbors.erase(i);

	if (adjMatrix[v][w] != NULL)
	{
		SAFE_DELETE(adjMatrix[v][w]);
		adjMatrix[w][v] = NULL;
	}
	return G_OK;
}

//�Ծ���ʽ��ʾͼ����Ϣ
void UndirectedGraph::DisplayGraph()
{
	cout << "\t";
	for (UINT i = 0; i < vertices.length; i++)
	{
		cout << vertices[i]->name << "\t";
		if (i != 0 && i % 8 == 0)
			cout << endl;
	}
	cout << endl;
	for (UINT i = 0; i < vertices.length; i++)
	{
		cout << vertices[i]->name << "\t";
		for (UINT j = 0; j < vertices.length; j++)
		{
			EdgeNode* a = adjMatrix[i][j] ;
			CVector<EdgeNode*> *en = &adjMatrix[i];
			if (a == NULL)
				cout <<00 << "\t";
			else
				cout << adjMatrix[i][j]->length<<"\t";
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;
	for (UINT i = 0; i < vertices.length; i++)
	{
		cout << vertices[i]->name << "s Leve: " << vertices[i]->sceneryLevel << endl;
	}
	cout << endl;
	cout << endl;
}

//Desc����ȡһ����
//Return:���򷵻رߣ����򷵻ؿ�ָ��
EdgeNode* UndirectedGraph::GetEdge(string v, string w)
{
	VertexNode* v1 = FindVertex(v);
	VertexNode* v2 = FindVertex(w);
	if (v1 == NULL || v2 == NULL)
		return NULL;
	else
		return adjMatrix[v1->index][v2->index];
}

//Desc����ȡ����ĵ�һ����
//Return:���򷵻رߣ����򷵻ؿ�ָ��
EdgeNode* UndirectedGraph::GetVexFirstEdge(string v)
{
	VertexNode* vertex = FindVertex(v);
	if (vertex != NULL)
	{
		for (UINT i = 0; i < vertices.length; i++)
			if (adjMatrix[vertex->index][i] != NULL)
				return adjMatrix[vertex->index][i];
	}
	return NULL;
}

Status UndirectedGraph::DFSTraverse(string v, void(*fun)(Vertex *))
{
	VertexNode *vertex = FindVertex(v);

	if (vertex == NULL)
		return G_CANTFIND;
	else
		{
			DFSTraverse(vertex->index, fun);
			return G_OK;
		}

}
void UndirectedGraph::DFSTraverse(UINT index,void(*fun)(Vertex *))
{
	bool* visit = new bool[vertices.length];
	UINT i, k;
	for ( i = 0; i < vertices.length; i++)
		visit[i] = false;

	bool isFirstTime = true;//��һ�α�����ʼ�Ķ���
	for (i = 0; i < vertices.length; i++)
	{
		if (isFirstTime)
		{
			k = index;
			isFirstTime = false;
			i--;
		}
		else
			k = i;
		if (!visit[k])
			DFS(visit, k, fun);
	}	
	delete[] visit;
}
//����������£�ͬʱ����������������ÿһ������
void UndirectedGraph::DFS(bool* visit,int i, void(*fun)(Vertex *))
{
	UINT j;
	fun(vertices[i]);
	visit[i] = true;
	for (j = 0; j < vertices.length; j++)
		if (adjMatrix[i][j] != NULL&&!visit[j])
			DFS(visit,j,fun);
}
//�����������
void UndirectedGraph::DFS(bool* visit, int i)
{
	UINT j;
	visit[i] = true;
	for (j = 0; j < vertices.length; j++)
		if (adjMatrix[i][j] != NULL&&!visit[j])
			DFS(visit, j);

}
//��ȱ���
Status UndirectedGraph::HFSTraverse(string v, void(*fun)(Vertex *))
{
	VertexNode *vertex = FindVertex(v);
	if (vertex == NULL)
		return G_CANTFIND;
	else
	{
		HFSTraverse(vertex->index, fun);
		return G_OK;
	}
}

void UndirectedGraph::HFSTraverse(UINT index, void(*fun)(Vertex *))
{
	bool* visit = new bool[vertices.length];//�ڵ���ʼ�¼

	UINT* queue = new UINT[vertices.length * 2];//����һ��ѭ������
	int queuesize = vertices.length * 2;
	int rear, front;//ѭ�������±�
	rear = front = 0;

	UINT i, j,k;
	for (i = 0; i < vertices.length; i++)
		visit[i] = false;

	//��ʼ����
	bool isFirstTime = true;			//�Ƿ��һ�ο�ʼ
	for (i = 0; i < vertices.length; i++)
	{
		if (isFirstTime)				//��һ�η��ʣ���Ϊ�ڵ��������Ǵ��㿪ʼ�ģ���Щ��ͬ
		{
			isFirstTime = false;
			k = index;
			i--;
		}
		else
			k = i;

		if (!visit[k])
		{
			visit[k] = true;
			fun(vertices[k]);

			queue[rear] = k;
			rear = (rear + 1) % queuesize;

			while (rear - front > 0)//�����в�Ϊ��ʱ
			{
				k = queue[front];			//����
				front = (front + 1) % queuesize;
				
				//���ڵ���ڽӽڵ����
				for (j = 0; j < vertices.length; j++)
				{
					if (adjMatrix[k][j]!=NULL && !visit[j])
					{
						visit[j] = true;
						fun(vertices[j]);

						queue[rear] = j;//���
						rear = (rear + 1) % queuesize;
					}
				}
			}
		}
	}

	delete[]visit;
	delete[]queue;
}

//Desc:Ѱ�Ҵӿ�ʼ��Ŀ�ĵ� �����·��
//���أ����벻��ȷ:G_ERROR;
//		�Ҳ���·��:G_CANFIND;
//		��      ��:G_OK;
Status UndirectedGraph::ShortestPath(string from, string target, CVector<Vertex*>& path)
{
	VertexNode* start = FindVertex(from);	//�����Լ��
	VertexNode* end = FindVertex(target);
	if (start == NULL || end == NULL)
		return G_ERROR;

	UINT *pathIndex = new UINT[vertices.length];//��start��end���·���У�ÿ���ڵ��Ӧ��ǰ��
	ShortestPath(end->index,start->index ,pathIndex);//��start��end

	path.Clear();
	UINT MAX = vertices.MAXSIZE;//��ShortestPath����Լ���ã���������ǰ����ֵΪMAX

	if (pathIndex[start->index] == MAX)//start��endû��һ��·��
		return G_CANTFIND;

	UINT k = start->index; 
	path.add(vertices[start->index]);
	for (UINT i = 0; i < vertices.length; i++)
	{
		path.add(vertices[pathIndex[k]]);
		k = pathIndex[k];
		if (k == end->index)
			break;		
	}

	delete[]pathIndex;
	return G_OK;
}

//Desc:����һ��vector���ҵ��Ĺؽڵ㽫��ŵ�����Ĺؽڵ�
//ATT:�������ָ�룬�ܶ�ͼ��Ľڵ���в���
int UndirectedGraph::FindArticul(CVector<Vertex*>& articul){
	bool* visit = new bool[vertices.length];
	for (UINT i = 0; i < vertices.length; i++)
		visit[i] = false;
	
	UINT articulCount = GetArticulCount(visit);
	UINT articalCountAfter = 0;
	articul.Clear();
	for (UINT i = 0; i < vertices.length; i++)
	{
		for (UINT j = 0; j < vertices.length; j++)
			visit[j] = false;
		visit[i] = true;
		articalCountAfter = GetArticulCount(visit);
		if (articalCountAfter > articulCount)
			articul.add(vertices[i]);			
	}

	delete[] visit;
	return articulCount;
}

//Desc:�õ��ؽڵ㣨��㣩������
//Return:�ؽڵ㣨��㣩������
//Input:visit:�ڵ���ʼ�¼
int UndirectedGraph::GetArticulCount(bool* visit) 
{
	UINT articulCount = 0;
	for (UINT i = 0; i < vertices.length; i++)
		if (!visit[i])
		{
			articulCount++;
			DFS(visit, i);
		}

	return articulCount;
}
void UndirectedGraph::ShortestPath(UINT from, UINT target, UINT* path)
{

	CVector<bool> final(vertices.length, false);
	CVector<unsigned long> shortestDis(vertices.length,INIFINITY);
	UINT MAX = vertices.MAXSIZE;//û�����ǰ���Ľڵ��ǰ��ΪMAX

	for (UINT i = 0; i < vertices.length; i++)
	{
		path[i] = from;//���нڵ��ǰ��Ϊ
		if (adjMatrix[from][i] == NULL)
			shortestDis[i] = INIFINITY;
		else
			shortestDis[i] = adjMatrix[from][i]->length;
	}

	path[from] = INIFINITY;
	final[from] = true;//from��from��·��Ϊ0
	shortestDis[from] = 0;
	UINT w, k, min;
	for (UINT v = 1; v < vertices.length; v++)
	{
		min = INIFINITY;
//		if (final[target])//��Ȼ�ҵ��ˣ��Ͳ���������
//			return;
		for (w = 0; w < vertices.length; w++)
		{
			if (!final[w] && shortestDis[w] < min)
			{
				k = w;
				min = shortestDis[w];
			}
		}
		final[k] = true;
		//�ҵ���������㣬�����֮�����ģ�û�б������ĵ㣬
		//��������ĳһ����w,����С����shortestDis��¼������shortestDis[w]��ͬʱ��path[w]��Ϊk
		for (w = 0; w < vertices.length; w++)
		{
			long dis;
			if (adjMatrix[k][w] == NULL)
				dis = INIFINITY;
			else
				dis = adjMatrix[k][w]->length;
			if (!final[w] && (min + dis) < shortestDis[w])
			{
				shortestDis[w] = min + dis;
				path[w] = k;
			}
		}
	}
}


//�ҵ�����������·�������ֺ��ֵ�·���
Status UndirectedGraph::FindAllPath(string from, string target, CVector<CVector<Vertex*>>& path)
{
	VertexNode* start = FindVertex(from);
	VertexNode* end = FindVertex(target);
	if (start == NULL || end == NULL)
		return G_ERROR;

	
	CVector<Vertex*> stack;
	stack.add(vertices[start->index]);
	for (UINT i = 0; i < vertices.length; i++)
	{
		if (adjMatrix[start->index][i] != NULL)
			if (FindAllPath(vertices[i], end, false,FootPath,stack, path))
			{
				stack.erase(stack.length - 1);
			}
	}
	stack.Clear();
	//FindAllPath(start->index, end->index);
	return G_OK;
}

Status UndirectedGraph::FindPathByPathType(string from, string target,
	PathType pathType, CVector<CVector<Vertex*>>& path)
{
	VertexNode* start = FindVertex(from);
	VertexNode* end = FindVertex(target);
	if (start == NULL || end == NULL)
		return G_ERROR;

	CVector<Vertex*> stack;
	stack.add(vertices[start->index]);
	for (UINT i = 0; i < vertices.length; i++)
	{
		if (adjMatrix[start->index][i] != NULL&&adjMatrix[start->index][i]->path == pathType)
			if (FindAllPath(vertices[i], end, true, pathType, stack, path))
			{
				stack.erase(stack.length - 1);
			}
	}
	stack.Clear();
	return G_OK;
}
//ʹ�õݹ��ջ��ջ���浱ǰ�ߵ�·����Ϣ��
//current�ǵ�ǰ���ͽ����Ľڵ�
//
bool UndirectedGraph::FindAllPath(VertexNode* current, VertexNode* target, 
						bool findByPath, PathType pathType,
						CVector<Vertex*>&stack,CVector<CVector<Vertex*>>& path)
{
	Vertex* top;
	if (stack.length > 0)
	{
		top = stack[stack.length - 1];
		if (top == current)
			return false;//����ָ������,��һ���Ի�·
		if (inStack(stack, current))
			return false;
	}
	//���������Ŀ��㣬���浱ǰ·��
	if (current == target)
	{
		stack.add(current);
		SavePath(stack, path);
		return true;
	}
	
	//����ǰ����ջ
	stack.add(current);
	for (UINT i = 0; i < vertices.length; i++)
	{
		EdgeNode* edge = adjMatrix[current->index][i];
		if (findByPath)
		{
			//��������ѡ��ĵ�·�ǣ��ż�������
			if (edge != NULL&&edge->path == pathType)
				if (FindAllPath(vertices[i], target, findByPath, pathType, stack, path))
					stack.erase(stack.length - 1);
		}
		else
		{
			if (edge != NULL)
				if (FindAllPath(vertices[i], target, findByPath, pathType, stack, path))
					stack.erase(stack.length - 1);
		}
		
	}
	return true;
}

//���رߵ���Ŀ��
int UndirectedGraph::GetEdgeNum() {
	int edgeNum = 0;
	int vertexNum = vertices.length;
	for (int i = 0; i < vertexNum;i++)
	{
		for (int j = i; j < vertexNum; j++)
		{
			if (adjMatrix[i][j] != NULL)
				edgeNum++;
		}
	}
	return edgeNum;
}

//�ߵļ�����1��ʼ����һ������0
//Return:���򷵻رߣ����򷵻�NULL
EdgeNode* UndirectedGraph::GetEdge(int index)
{
	if (index <= 0)
		return NULL;
	int edgeIndex = 0;
	for (int i = 0; i < vertices.length; i++)
	{
		for (int j = i; j < vertices.length; j++)
		{
			if (adjMatrix[i][j] != NULL)
			{
				edgeIndex++;
				if (edgeIndex == index)
					return adjMatrix[i][j];
			}
		}
	}
	return NULL;
}

Status UndirectedGraph::FindPathBySceneryLevel(string from, string target,
	CVector<CVector<Vertex*>>& path)
{
	CVector<CVector<Vertex*>> allPath;
	Status result = FindAllPath(from, target, allPath);
	if (result != G_OK)
		return result;


	int *levelCount = new int[allPath.length];
	for (UINT i = 0; i < allPath.length; i++)
	{
		levelCount[i] = 0;//��ʼ��
		for (UINT j = 0; j < allPath[i].length; j++)
		{
			levelCount[i] += allPath[i][j]->sceneryLevel;
		}
			
	}

	int MaxLevelCount = 0;
	for (UINT i = 0; i < allPath.length; i++)
	{
		if (levelCount[i] > MaxLevelCount)
			MaxLevelCount = levelCount[i];
	}
	path.Clear();
	//·��һ��ֻ��һ������
	for (UINT i = 0; i < allPath.length; i++)
	{
		if (levelCount[i] == MaxLevelCount)
		{
			path.add(allPath[i]);
		}
	}
	delete[] levelCount;
	return G_OK;
}
bool  UndirectedGraph::inStack(CVector<Vertex*>& stack, Vertex* node)
{
	for (UINT i = 0; i < stack.length; i++)
		if (stack[i] == node)
			return true;
	return false;
}

void UndirectedGraph::SavePath(CVector<Vertex*>&stack, CVector<CVector<Vertex*>>& path)
{
	CVector<Vertex*> temp;
	path.add(temp);
	UINT last = path.length - 1;
	for (UINT i = 0; i < stack.length; i++)
		path[last].add(stack[i]);
}
UndirectedGraph::~UndirectedGraph() {
	for (UINT i = 0; i < vertices.length; i++)
	{
		for (UINT j = 0; j <= i; j++)
		{
			SAFE_DELETE(adjMatrix[i][j]);
			adjMatrix[j][i] = NULL;
		}
	}
	for (UINT i = 0; i < vertices.length; i++)
		SAFE_DELETE(vertices[i]);
}