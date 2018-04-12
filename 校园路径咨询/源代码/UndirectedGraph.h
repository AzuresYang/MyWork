#ifndef _UndirectedGraph_H_
#define _UndirectedGraph_H_
#include<string>
#include<windows.h>
#include<iostream>
#include"CVector.h"

#define SAFE_DELETE(p)       { if(p !=NULL) { delete (p);     (p)=NULL; } }
using namespace std;
using  std::string;

float const CircleRadius = 3;
int const   viewSpotScope = 10;//景点范围大小，用来判断方向

//返回值类型
typedef enum Status {
	G_ERROR = -1, G_FALSE, G_OK, G_TRUE,G_CANTFIND
}Status;

//路口方向
typedef enum Direction {
	Weast,North,East,South
}Direction;

//道路类型
enum PathType {
	CarPath,FootPath
};

//客户用顶点
class Vertex;
//无向边
typedef struct EdgeNode
{
	Vertex* vertex1, *vertex2;	//边连接的两个点
	long length;//权值
	PathType path;
	int sceneryLevel;
	EdgeNode() {
		length = 0;
		path = FootPath;
		sceneryLevel = 1;
	}
}EdgeNode, *PEdgeNode;

//邻居
class Neighbor
{
public:
	Vertex* neighbor;
	Direction direction;
	EdgeNode* path;
	Neighbor() {
		neighbor = NULL;
		path = NULL;
	}
};
class Neighbor;

typedef class Vertex
{
public:
	string name;
	int weight;
	string introduce;
	POINT position;
	int sceneryLevel;
	CVector<Neighbor>neighbors;
	Vertex() { weight = sceneryLevel = 0; }
	void addNeighbor(Direction direction, Vertex* v, EdgeNode* edge);
}Vertex;

//类内使用顶点
typedef class VertexNode: public Vertex
{
public:
	int index;
	VertexNode& operator=( Vertex& v)
	{
		name = v.name;
		weight = v.weight;
		position = v.position;
		sceneryLevel = v.sceneryLevel;
		introduce = v.introduce;
		neighbors.Clear();
		for (UINT i = 0; i < v.neighbors.length; i++)
			neighbors.add(v.neighbors[i]);
		return *this;
	}
}VertexNode,*PVertexNode;



class UndirectedGraph
{
private:
	static const LONG INIFINITY = 2147483647;//2147483647;//代表正无穷
	CVector<VertexNode*> vertices;
	CVector<CVector<EdgeNode*>> adjMatrix;

	Status DeleteVertex(unsigned int index);
	VertexNode* FindVertex(string name);

	void DFS(bool* visit, int i,void(*fun)(Vertex *));
	void DFS(bool* visit, int i);
	void DFSTraverse(UINT index, void(*fun)(Vertex *));//深度优先遍历
	void HFSTraverse(UINT index, void(*fun)(Vertex *));//广度优先遍历
	void ShortestPath(UINT from, UINT target, UINT* path);//类内用最短路径求法
	bool FindAllPath(VertexNode* current, VertexNode* target,
					bool findByPath, PathType pathType,
					CVector<Vertex*>&stack, CVector<CVector<Vertex*>>& path);//寻找所有路径
	bool inStack(CVector<Vertex*>& stack, Vertex* node);//在栈中则返回真
	void SavePath(CVector<Vertex*>&stack,
				CVector<CVector<Vertex*>>& path);//保存搜索路径
public:
	Status AddVertex(Vertex& vertex);
	Status AddEdge(string v1, string v2, EdgeNode& edge);//添加一段弧
	Status DeleteVertex(string name);
	Status DeleteEdge(string v1, string v2);
	void DisplayGraph();

	Vertex* GetVex(string v) { return FindVertex(v); }
	//第一个顶点的索引是1
	Vertex* GetVex(int i) { if (i <= 0 || i > vertices.length)return NULL; else return vertices[i-1]; }
	EdgeNode* GetEdge(string v, string w);
	EdgeNode* GetEdge(int index);
	int GetEdgeNum();
	EdgeNode* GetVexFirstEdge(string v);

	Status DFSTraverse(string v, void(*fun)(Vertex *));
	Status HFSTraverse(string v, void(*fun)(Vertex *));//广度优先遍历
	Status ShortestPath(string from, string target, CVector<Vertex*>& path);
	Status FindAllPath(string from, string target, CVector<CVector<Vertex*>>& path);
	Status FindPathBySceneryLevel(string from, string target, 
								  CVector<CVector<Vertex*>>& path);


	//寻找两点间的所有指定道路类型路径，
	Status FindPathByPathType(string from, string target,
						PathType pathType,CVector<CVector<Vertex*>>& path);
	//Status ChangeVertex(string v, Vertex* change);
	int FindArticul(CVector<Vertex*>& articul);//寻找图中的关节点
	int GetArticulCount(bool* visit);//得到关节点的数目
	int GetVertexNum() { return vertices.length; }
	//最短路径
	~UndirectedGraph();
};

#endif