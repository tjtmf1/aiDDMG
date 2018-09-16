#pragma once
class Node
{
public:
	int node_line[4];	//line
	int level;		//
	int LineCnt;	//
	int value;		//
	int ChildCnt;
	Node *Parent;
	Node **Child;
	Node();
	~Node();
	void CreateChild(int **l, int size);
};

Node* MinMax(Node *n, int alpha, int beta);
void MaxMove(Node *n, int alpha, int beta);
void MinMove(Node *n, int alpha, int beta);
void Clear(Node *&n, Node *next, bool check = false);
const int MAX_VALUE = 9999;
const int MIN_VALUE = -9999;