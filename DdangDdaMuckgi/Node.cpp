#include "stdafx.h"
#include "Node.h"
#include "DdangDdaMuckgiDlg.h"
#include "DdangDdaMuckgi.h"
#include <ctime>
Node::Node(void)
{
	value = -50;
	ChildCnt = 0;
	Parent = NULL;
	Child = NULL;
}

Node::~Node(void)
{
	if (!Child)
	{
		for (int i = 0; i < ChildCnt; i++)
		{
			if (!Child[i])
				delete Child[i];
		}
	}
}

void Node::CreateChild(int **l, int size)
{
	ChildCnt = size;
	Child = new Node*[ChildCnt];
	for (int i = 0; i < ChildCnt; i++)
	{
		Child[i] = new Node();
		Child[i]->node_line[0] = l[i][0];
		Child[i]->node_line[1] = l[i][1];
		Child[i]->node_line[2] = l[i][2];
		Child[i]->node_line[3] = l[i][3];
		Child[i]->level = level + 1;
		Child[i]->LineCnt = LineCnt + 1;
		Child[i]->Parent = this;
	}
}

Node* MinMax(Node *n, int alpha, int beta)
{
	int best_value;
	Node* best_node = NULL;
	switch (0)
	{
	case 0:
		MaxMove(n, alpha, beta);
		best_value = MIN_VALUE;
		for (int i = 0; i < n->ChildCnt; i++)
		{
			if (n->Child[i] != NULL)
			{
				if (best_value < n->Child[i]->value)
				{
					best_value = n->Child[i]->value;
					best_node = n->Child[i];
				}
			}
		}
		n->value = best_value;

		break;
	case 1:
		MinMove(n, alpha, beta);
		best_value = MAX_VALUE;
		for (int i = 0; i < n->ChildCnt; i++)
		{
			if (n->Child[i] != NULL)
			{
				if (best_value < n->Child[i]->value)
				{
					best_value = n->Child[i]->value;
					best_node = n->Child[i];
				}
			}
		}
		n->value = best_value;
		break;
	}
	return best_node;
}
void MaxMove(Node *n, int alpha, int beta)
{
	CDdangDdaMuckgiDlg *dlg = ((CDdangDdaMuckgiDlg*)theApp.m_pMainWnd);
	if (n->ChildCnt == 0)
	{
		int **l = new int*[n->LineCnt];
		for (int k = 0; k < dlg->count; k++)
		{
			l[k] = new int[4];
			l[k][0] = dlg->line[k][0];
			l[k][1] = dlg->line[k][1];
			l[k][2] = dlg->line[k][2];
			l[k][3] = dlg->line[k][3];
		}
		Node *temp = n;
		for (int k = n->LineCnt - dlg->count - 1; k >= 0; k--)
		{
			l[k + dlg->count] = new int[4];
			l[k + dlg->count][0] = temp->node_line[0];
			l[k + dlg->count][1] = temp->node_line[1];
			l[k + dlg->count][2] = temp->node_line[2];
			l[k + dlg->count][3] = temp->node_line[3];
			temp = temp->Parent;
		}
		n->value = dlg->evaluate(l, n->LineCnt);
		for (int i = 0; i < n->LineCnt; i++)
			delete[] l[i];
		delete[] l;
		return;
	}
	int max_value = MIN_VALUE;
	for (int i = 0; i < n->ChildCnt; i++)
	{
		if (n->Child[i] != NULL)
		{
			MinMove(n->Child[i], alpha, beta);
			if (n->Child[i]->value > max_value)
			{
				max_value = n->Child[i]->value;
				alpha = max_value;
			}
			//if (alpha >= beta)
			//	break;
		}
	}
	n->value = max_value;
}
void MinMove(Node *n, int alpha, int beta)
{
	CDdangDdaMuckgiDlg *dlg = ((CDdangDdaMuckgiDlg*)theApp.m_pMainWnd);
	if (n->ChildCnt == 0)
	{
		int **l = new int*[n->LineCnt];
		for (int k = 0; k < dlg->count; k++)
		{
			l[k] = new int[4];
			l[k][0] = dlg->line[k][0];
			l[k][1] = dlg->line[k][1];
			l[k][2] = dlg->line[k][2];
			l[k][3] = dlg->line[k][3];
		}
		Node *temp = n;
		for (int k = n->LineCnt - dlg->count - 1; k >= 0; k--)
		{
			l[k + dlg->count] = new int[4];
			l[k + dlg->count][0] = temp->node_line[0];
			l[k + dlg->count][1] = temp->node_line[1];
			l[k + dlg->count][2] = temp->node_line[2];
			l[k + dlg->count][3] = temp->node_line[3];
			temp = temp->Parent;
		}
		n->value = dlg->evaluate(l, n->LineCnt);
		for (int i = 0; i < n->LineCnt; i++)
			delete[] l[i];
		delete[] l;
		return;
	}
	int min_value = MAX_VALUE;
	for (int i = 0; i < n->ChildCnt; i++)
	{
		if (n->Child[i] != NULL)
		{
			MaxMove(n->Child[i], alpha, beta);
			if (n->Child[i]->value < min_value)
			{
				min_value = n->Child[i]->value;
				beta = min_value;
			}
			if (alpha >= beta)
				break;
		}
	}
	n->value = min_value;
}
void Clear(Node *&n, Node *next, bool check)
{
	if (check && n->value == -50)
	{
		delete n;
		n = NULL;
		return;
	}
	for (int i = 0; i < n->ChildCnt; i++)
	{
		if (n->Child[i] != NULL)
		{
			if (n == next)
				Clear(n->Child[i], n->Child[i], check);
			else
				Clear(n->Child[i], next, check);
		}
	}
	if (n == next)
		return;
	delete n;
}