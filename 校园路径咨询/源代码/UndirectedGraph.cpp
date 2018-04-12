#include"UndirectedGraph.h"


//增加一个邻居
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
//寻找一个顶点，失败则返回NULL
VertexNode* UndirectedGraph::FindVertex(string name)
{
	for (unsigned int i = 0; i < vertices.length; i++)
		if (vertices[i]->name == name)
			return vertices[i];
	return NULL;
}

//Desc:增加一个顶点的副本，为新的顶点分配下标
//Return:没有v1,,v2顶点，添加失败，返回G_ERROR;
//		添加成功，返回G_OK;

Status UndirectedGraph::AddVertex(Vertex& vertex)
{
	CVector<EdgeNode*> edgevector(vertices.length, NULL);//为邻接矩阵增加一行
	PVertexNode vn = new VertexNode();//新建一个顶点
	(*vn) = vertex;						//复制成为副本
	vn->index = vertices.length;//分配下标
	vertices.add(vn);			//添加入顶点数组
	adjMatrix.add(edgevector);//如果不适用new，则会 在这之后发生析构。
	for(UINT i = 0; i < vertices.length; i++)//补上邻接矩阵的列数
		adjMatrix[i].add(NULL);
	return G_OK;
}

//Desc：增加一条边的副本，如果原来的两个顶点有边，则覆盖之
Status UndirectedGraph::AddEdge(string v1, string v2, EdgeNode& edge)
{
	int index1, index2;					//检测是否有这两个顶点
	PVertexNode vertex1,vertex2;
	vertex1 = FindVertex(v1);
	vertex2 = FindVertex(v2);
	if (vertex1 == NULL || vertex2 == NULL)
		return G_ERROR;

	index1 = vertex1->index;
	index2 = vertex2->index;
	if (adjMatrix[index1][index2] != NULL)		//原来有边则覆盖之
	{
		SAFE_DELETE(adjMatrix[index1][index2]);
		adjMatrix[index2][index1] = NULL;
	}

	EdgeNode* e = new EdgeNode();	//在邻接矩阵增加边			
	*e = edge;
	e->vertex1 = vertices[index1];
	e->vertex2 = vertices[index2];
	adjMatrix[index1][index2] = e;
	adjMatrix[index2][index1] = e;

	//为双方添加邻居
	Neighbor neighbor;
	neighbor.path = e;
	neighbor.neighbor = vertex1;
	//这里计算direction;
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

//Desc：删除一个顶点和与之有关的边，
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

//Desc：删除一个顶点和与之有关的边
Status UndirectedGraph::DeleteVertex(unsigned int index)
{
	SAFE_DELETE(vertices[index]);
	vertices.erase(index);			//删除顶点

	CVector<EdgeNode*>* en = &adjMatrix[index];//释放行所储存的内容
	for (UINT i = 0; i < en->length; i++)
	{
		SAFE_DELETE(en[index][i]);
	}

	adjMatrix.erase(index);//
	for (UINT i = 0; i < vertices.length; i++)//释放该列所储存的内容
	{
		adjMatrix[i][index] = NULL;//上边已经删除过了
		en = &adjMatrix[i];
		en->erase(index);
	}
	for (UINT i = 0; i < vertices.length; i++)//重新绑定顶点下标
		vertices[i]->index = i;
	return G_OK;
}

//Desc：删除一条边，同时把双方的邻居关系删除之
Status UndirectedGraph::DeleteEdge(string v1, string v2)
{
	int v, w;
	PVertexNode vertex1, vertex2;	//检测是否有这条边
	vertex1 = FindVertex(v1);
	vertex2 = FindVertex(v2);
	if (vertex1 == NULL || vertex2 == NULL)
		return G_ERROR;
	v = vertex1->index;
	w = vertex2->index;

	//删除邻居关系

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

//以矩阵方式显示图的信息
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

//Desc：获取一条边
//Return:有则返回边，无则返回空指针
EdgeNode* UndirectedGraph::GetEdge(string v, string w)
{
	VertexNode* v1 = FindVertex(v);
	VertexNode* v2 = FindVertex(w);
	if (v1 == NULL || v2 == NULL)
		return NULL;
	else
		return adjMatrix[v1->index][v2->index];
}

//Desc：获取顶点的第一条边
//Return:有则返回边，无则返回空指针
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

	bool isFirstTime = true;//第一次遍历开始的顶点
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
//将遍历表更新，同时将遍历函数作用于每一个顶点
void UndirectedGraph::DFS(bool* visit,int i, void(*fun)(Vertex *))
{
	UINT j;
	fun(vertices[i]);
	visit[i] = true;
	for (j = 0; j < vertices.length; j++)
		if (adjMatrix[i][j] != NULL&&!visit[j])
			DFS(visit,j,fun);
}
//将遍历表更新
void UndirectedGraph::DFS(bool* visit, int i)
{
	UINT j;
	visit[i] = true;
	for (j = 0; j < vertices.length; j++)
		if (adjMatrix[i][j] != NULL&&!visit[j])
			DFS(visit, j);

}
//广度遍历
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
	bool* visit = new bool[vertices.length];//节点访问记录

	UINT* queue = new UINT[vertices.length * 2];//制作一个循环队列
	int queuesize = vertices.length * 2;
	int rear, front;//循环队列下标
	rear = front = 0;

	UINT i, j,k;
	for (i = 0; i < vertices.length; i++)
		visit[i] = false;

	//开始遍历
	bool isFirstTime = true;			//是否第一次开始
	for (i = 0; i < vertices.length; i++)
	{
		if (isFirstTime)				//第一次访问，因为节点索引不是从零开始的，有些不同
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

			while (rear - front > 0)//当队列不为空时
			{
				k = queue[front];			//出队
				front = (front + 1) % queuesize;
				
				//将节点的邻接节点入队
				for (j = 0; j < vertices.length; j++)
				{
					if (adjMatrix[k][j]!=NULL && !visit[j])
					{
						visit[j] = true;
						fun(vertices[j]);

						queue[rear] = j;//入队
						rear = (rear + 1) % queuesize;
					}
				}
			}
		}
	}

	delete[]visit;
	delete[]queue;
}

//Desc:寻找从开始到目的地 的最短路径
//返回：输入不正确:G_ERROR;
//		找不到路径:G_CANFIND;
//		找      到:G_OK;
Status UndirectedGraph::ShortestPath(string from, string target, CVector<Vertex*>& path)
{
	VertexNode* start = FindVertex(from);	//存在性检测
	VertexNode* end = FindVertex(target);
	if (start == NULL || end == NULL)
		return G_ERROR;

	UINT *pathIndex = new UINT[vertices.length];//由start到end最短路径中，每个节点对应的前驱
	ShortestPath(end->index,start->index ,pathIndex);//从start到end

	path.Clear();
	UINT MAX = vertices.MAXSIZE;//与ShortestPath函数约定好，若不存在前驱则值为MAX

	if (pathIndex[start->index] == MAX)//start到end没有一条路径
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

//Desc:输入一个vector，找到的关节点将会放到输入的关节点
//ATT:放入的是指针，能对图里的节点进行操作
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

//Desc:得到关节点（割点）的数量
//Return:关节点（割点）的数量
//Input:visit:节点访问记录
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
	UINT MAX = vertices.MAXSIZE;//没有最近前驱的节点的前驱为MAX

	for (UINT i = 0; i < vertices.length; i++)
	{
		path[i] = from;//所有节点的前驱为
		if (adjMatrix[from][i] == NULL)
			shortestDis[i] = INIFINITY;
		else
			shortestDis[i] = adjMatrix[from][i]->length;
	}

	path[from] = INIFINITY;
	final[from] = true;//from到from的路径为0
	shortestDis[from] = 0;
	UINT w, k, min;
	for (UINT v = 1; v < vertices.length; v++)
	{
		min = INIFINITY;
//		if (final[target])//既然找到了，就不继续找了
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
		//找到的最近顶点，检查与之相连的，没有遍历过的点，
		//如果到达的某一个点w,距离小于在shortestDis记录，更新shortestDis[w]，同时将path[w]置为k
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


//找到两点间的任意路径，不分何种道路与否
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
//使用递归和栈，栈储存当前走的路径信息。
//current是当前传送进来的节点
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
			return false;//自身指向自身,是一个自回路
		if (inStack(stack, current))
			return false;
	}
	//如果到达了目标点，储存当前路径
	if (current == target)
	{
		stack.add(current);
		SavePath(stack, path);
		return true;
	}
	
	//将当前点入栈
	stack.add(current);
	for (UINT i = 0; i < vertices.length; i++)
	{
		EdgeNode* edge = adjMatrix[current->index][i];
		if (findByPath)
		{
			//仅当是所选择的道路是，才继续搜索
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

//返回边的数目。
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

//边的计算以1开始，第一条边是0
//Return:有则返回边，无则返回NULL
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
		levelCount[i] = 0;//初始化
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
	//路不一定只有一条的嘛
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