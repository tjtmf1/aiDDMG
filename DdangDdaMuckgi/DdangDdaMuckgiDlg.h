// DdangDdaMuckgiDlg.h : ��� ����
//

#pragma once

#include "highgui.h"
#include "cv.h"
#include "cxcore.h"
#include "ml.h"
#include <vector>
#include "Node.h"
#include "Resource.h"
using namespace std;

// CDdangDdaMuckgiDlg ��ȭ ����
class CDdangDdaMuckgiDlg : public CDialog
{

// �����Դϴ�.
public:	
	CDdangDdaMuckgiDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CDdangDdaMuckgiDlg();
	CRect view;
	IplImage *m_view;
	CvvImage m_View;
	
	int size;
	int map[7][7];
	int **line;
	int **searchline;
	int searchsize;
	int first;
	int **com_dot;
	int dot_count;
	
	
	int count;
	CvPoint start_point, end_point;
	bool chk;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DDANGDDAMUCKGI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CStatic m_iView;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int x_point1;
	int x_point2;
	int y_point1;
	int y_point2;
	int red_score;
	int blue_score;
	afx_msg void OnBnClickedInput();
	CString team_name;
	CString histo;
	afx_msg void OnBnClickedRestart();
	bool CheckCross(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2);
	bool CheckPossibleLine(int x1, int y1, int x2, int y2, int& error,int **line,int count);
	int GCD(int a, int b);
	int Com_turn();
	int turn;
	bool CheckInsideVtx(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void ChangeScore(int **line, int count, int &red_score, int &blue_score);
	vector<int*> vtx;
	int evaluate(int **l,int c);
	Node *start_node;
	Node *cur_node;
	void DFS_Traversal(Node *n);
	afx_msg void OnBnClickedfirst();
	int LIMIT;
	int com_turn_cnt;
};
