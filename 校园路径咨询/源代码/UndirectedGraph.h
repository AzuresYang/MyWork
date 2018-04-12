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
int const   viewSpotScope = 10;//���㷶Χ��С�������жϷ���

//����ֵ����
typedef enum Status {
	G_ERROR = -1, G_FALSE, G_OK, G_TRUE,G_CANTFIND
}Status;

//·�ڷ���
typedef enum Direction {
	Weast,North,East,South
}Direction;

//��·����
enum PathType {
	CarPath,FootPath
};

//�ͻ��ö���
class Vertex;
//�����
typedef struct EdgeNode
{
	Vertex* vertex1, *vertex2;	//�����ӵ�������
	long length;//Ȩֵ
	PathType path;
	int sceneryLevel;
	EdgeNode() {
		length = 0;
		path = FootPath;
		sceneryLevel = 1;
	}
}EdgeNode, *PEdgeNode;

//�ھ�
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

//����ʹ�ö���
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
	static const LONG INIFINITY = 2147483647;//2147483647;//����������
	CVector<VertexNode*> vertices;
	CVector<CVector<EdgeNode*>> adjMatrix;

	Status DeleteVertex(unsigned int index);
	VertexNode* FindVertex(string name);

	void DFS(bool* visit, int i,void(*fun)(Vertex *));
	void DFS(bool* visit, int i);
	void DFSTraverse(UINT index, void(*fun)(Vertex *));//������ȱ���
	void HFSTraverse(UINT index, void(*fun)(Vertex *));//������ȱ���
	void ShortestPath(UINT from, UINT target, UINT* path);//���������·����
	bool FindAllPath(VertexNode* current, VertexNode* target,
					bool findByPath, PathType pathType,
					CVector<Vertex*>&stack, CVector<CVector<Vertex*>>& path);//Ѱ������·��
	bool inStack(CVector<Vertex*>& stack, Vertex* node);//��ջ���򷵻���
	void SavePath(CVector<Vertex*>&stack,
				CVector<CVector<Vertex*>>& path);//��������·��
public:
	Status AddVertex(Vertex& vertex);
	Status AddEdge(string v1, string v2, EdgeNode& edge);//���һ�λ�
	Status DeleteVertex(string name);
	Status DeleteEdge(string v1, string v2);
	void DisplayGraph();

	Vertex* GetVex(string v) { return FindVertex(v); }
	//��һ�������������1
	Vertex* GetVex(int i) { if (i <= 0 || i > vertices.length)return NULL; else return vertices[i-1]; }
	EdgeNode* GetEdge(string v, string w);
	EdgeNode* GetEdge(int index);
	int GetEdgeNum();
	EdgeNode* GetVexFirstEdge(string v);

	Status DFSTraverse(string v, void(*fun)(Vertex *));
	Status HFSTraverse(string v, void(*fun)(Vertex *));//������ȱ���
	Status ShortestPath(string from, string target, CVector<Vertex*>& path);
	Status FindAllPath(string from, string target, CVector<CVector<Vertex*>>& path);
	Status FindPathBySceneryLevel(string from, string target, 
								  CVector<CVector<Vertex*>>& path);


	//Ѱ������������ָ����·����·����
	Status FindPathByPathType(string from, string target,
						PathType pathType,CVector<CVector<Vertex*>>& path);
	//Status ChangeVertex(string v, Vertex* change);
	int FindArticul(CVector<Vertex*>& articul);//Ѱ��ͼ�еĹؽڵ�
	int GetArticulCount(bool* visit);//�õ��ؽڵ����Ŀ
	int GetVertexNum() { return vertices.length; }
	//���·��
	~UndirectedGraph();
};

#endif