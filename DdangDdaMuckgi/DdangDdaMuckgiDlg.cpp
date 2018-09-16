// DdangDdaMuckgiDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "DdangDdaMuckgi.h"
#include "DdangDdaMuckgiDlg.h"
#include <vector>
#include <cmath>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#define MIN(a, b)(a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDdangDdaMuckgiDlg 대화 상자




CDdangDdaMuckgiDlg::CDdangDdaMuckgiDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDdangDdaMuckgiDlg::IDD, pParent)
, x_point1(0)
, x_point2(0)
, y_point1(0)
, y_point2(0)
, red_score(0)
, blue_score(0)
, count(0)
, team_name(_T("RED"))
, histo(_T(""))
, chk(true)
, turn(0)
, size(0)
, cur_node(NULL)
, dot_count(0)
, searchline(NULL)
, searchsize(0)
, first(0)
, LIMIT(2)
, com_turn_cnt(0)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_view = cvCreateImage(cvSize(700, 700),8,3);

	int temp=0;
	srand((unsigned int)time(NULL));

	line = new int*[160];
	for (int i = 0; i < 160; i++)
		line[i] = new int[4];
	/*
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (map[i][j] = rand() % 2)
				dot_count++;
		}
	}
	*/

	map[0][0] = 1;
	map[6][6] = 1;
	map[6][0] = 1;
	map[0][6] = 1;
	map[3][3] = 1;
	map[3][2] = 1;
	dot_count = 6;
	com_dot = new int*[dot_count];
	int i = 0;
	for (int j = 0; j < 7; j++)
	{
		for (int k = 0; k < 7; k++)
		{
			if (map[j][k] == 1)
			{
				com_dot[i] = new int[2];
				com_dot[i][0] = j + 1;
				com_dot[i++][1] = k + 1;
			}
		}
	}


	for(int i=0; i<700; i++){
		for(int j =0; j<700; j++){

			m_view->imageData[(i*m_view->width+j)*3+2] = (unsigned char)255;
			m_view->imageData[(i*m_view->width+j)*3+1] = (unsigned char)255;
			m_view->imageData[(i*m_view->width+j)*3] = (unsigned char)255;

			if(i%100==0 || j%100==0){
				m_view->imageData[(i*m_view->width+j)*3+2] = (unsigned char)200;
				m_view->imageData[(i*m_view->width+j)*3+1] = (unsigned char)200;
				m_view->imageData[(i*m_view->width+j)*3] = (unsigned char)200;
			}
			else if(i%50==0 && j%50==0)
				if(map[((j-50)/100)][((i-50)/100)] == 1){
					for(int y=-2; y<3; y++)
						for(int x=-2; x<3; x++)
						{
							m_view->imageData[((i+y)*m_view->width+(j+x))*3+2] = (unsigned char)0;
							m_view->imageData[((i+y)*m_view->width+(j+x))*3+1] = (unsigned char)0;
							m_view->imageData[((i+y)*m_view->width+(j+x))*3] = (unsigned char)0;
						}
						temp++;
				}
		}
	}
	

}
CDdangDdaMuckgiDlg::~CDdangDdaMuckgiDlg()
{
	for (int i = 0; i < 160; i++)
		delete[] line[i];
	delete[] line;
	for (int i = 0; i < dot_count; i++)
		delete[] com_dot[i];
	delete[] com_dot;
	if (cur_node != NULL)
	{
		Clear(cur_node, NULL);
	}
}

void CDdangDdaMuckgiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VIEW, m_iView);
	DDX_Text(pDX, IDC_X1, x_point1);
	DDV_MinMaxInt(pDX, x_point1, 1, 7);
	DDX_Text(pDX, IDC_X2, x_point2);
	DDV_MinMaxInt(pDX, x_point2, 1, 7);
	DDX_Text(pDX, IDC_Y1, y_point1);
	DDV_MinMaxInt(pDX, y_point1, 1, 7);
	DDX_Text(pDX, IDC_Y2, y_point2);
	DDV_MinMaxInt(pDX, y_point2, 1, 7);
	DDX_Text(pDX, IDC_RED_SCORE, red_score);
	DDX_Text(pDX, IDC_BLUE_SCORE, blue_score);
	DDX_Text(pDX, IDC_TEAM_NAME, team_name);
	DDX_Text(pDX, IDC_HISTO, histo);
}

BEGIN_MESSAGE_MAP(CDdangDdaMuckgiDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_INPUT, &CDdangDdaMuckgiDlg::OnBnClickedInput)
	ON_BN_CLICKED(IDC_RESTART, &CDdangDdaMuckgiDlg::OnBnClickedRestart)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_first, &CDdangDdaMuckgiDlg::OnBnClickedfirst)
END_MESSAGE_MAP()


// CDdangDdaMuckgiDlg 메시지 처리기

BOOL CDdangDdaMuckgiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	SetTimer(1, (UINT)((1/(double)12)*1),NULL);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDdangDdaMuckgiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDdangDdaMuckgiDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.  	
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		m_View.CopyOf(m_view);

		m_iView.GetClientRect(&view);
		m_View.DrawToHDC(m_iView.GetDC()->GetSafeHdc(), view);

		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDdangDdaMuckgiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDdangDdaMuckgiDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(1 == nIDEvent){

		for(int i=0; i<count; i++){

			if(i%2 == 0)
				cvLine(m_view, cvPoint(line[i][0]*100-50, line[i][1]*100-50), cvPoint(line[i][2]*100-50, line[i][3]*100-50), CV_RGB(255, 0, 0), 5);
			else
				cvLine(m_view, cvPoint(line[i][0]*100-50, line[i][1]*100-50), cvPoint(line[i][2]*100-50, line[i][3]*100-50), CV_RGB(0, 0, 255), 5);

		}


		if(chk == true){
			m_View.CopyOf(m_view);

			m_iView.GetClientRect(&view);
			m_View.DrawToHDC(m_iView.GetDC()->GetSafeHdc(), view);

			chk = false;

		}

	}


	CDialog::OnTimer(nIDEvent);
}


void CDdangDdaMuckgiDlg::OnBnClickedInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int error;
	if(turn%2==first)//내가 선공
	{
		if (CheckPossibleLine(x_point1, y_point1, x_point2, y_point2, error,line,count))
		{
			//점을 집어 넣음.
			line[count][0] = x_point1;
			line[count][1] = y_point1;
			line[count][2] = x_point2;
			line[count++][3] = y_point2;
			if (com_turn_cnt > 2)
			{
				for (int i = 0; i < cur_node->ChildCnt; i++)
				{
					if (cur_node->Child[i] != NULL)
					{
						Node *next_turn = cur_node->Child[i];
						if ((next_turn->node_line[0] == x_point1 && next_turn->node_line[1] == y_point1 && next_turn->node_line[2] == x_point2 && next_turn->node_line[3] == y_point2) ||
							(next_turn->node_line[2] == x_point1 && next_turn->node_line[3] == y_point1 && next_turn->node_line[0] == x_point2 && next_turn->node_line[1] == y_point2))
						{
							Clear(cur_node, next_turn);
							cur_node = next_turn;

							searchline = NULL;
							DFS_Traversal(cur_node);
							for (int j = 0; j < searchsize; j++)
								delete[] searchline[j];
							delete[] searchline;
							searchsize = 0;

							break;
						}
					}
				}
			}

			
			CString str;
			str.Format(_T("%s : (%d, %d), (%d, %d) \r\n"), team_name, x_point1, y_point1, x_point2, y_point2);
			histo += str;
			turn++;
		}
		else
		{
			switch (error)
			{
			case 0:
				MessageBox(_T("이미 그려져 있는 선분입니다."));
				break;
			case 1:
				MessageBox(_T("정해진 점을 이용한 선만을 그릴 수 있습니다."));
				break;
			case 2:
				MessageBox(_T("이미 그려진 선분과 교차합니다."));
				break;
			case 3:
				MessageBox(_T("선분 중간에 점이 있습니다."));
				break;
			case 4:
				MessageBox(_T("선분을 그려야 합니다."));
				break;
			default:
				break;
			}

		}
	}
	else
	{
		if (com_turn_cnt < 2) // count 4전에는 민맥스를 돌리지 않음.
		{
			int longest[5] = { 0, 0, 0, 0, 0 }; //x1,y1,x2,y2,l^2
			for (int i = 0; i < dot_count; i++)
			{
				for (int j = i + 1; j < dot_count; j++)
				{
					if (CheckPossibleLine(com_dot[i][0], com_dot[i][1], com_dot[j][0], com_dot[j][1], error, line, count))
					{
						bool possible = true;
						for (int k = 0; k < count; k++)
						{
							if ((line[k][0] == com_dot[i][0] && line[k][1] == com_dot[i][1]) || (line[k][2] == com_dot[i][0] && line[k][3] == com_dot[i][1]) || 
								(line[k][0] == com_dot[j][0] && line[k][1] == com_dot[j][1]) || (line[k][2] == com_dot[j][0] && line[k][3] == com_dot[j][1]))
							{
								possible = false;
								break;
							}
						}
						if (possible)
						{
							int l = (com_dot[i][0] - com_dot[j][0])*(com_dot[i][0] - com_dot[j][0]) + (com_dot[i][1] - com_dot[j][1])*(com_dot[i][1] - com_dot[j][1]);
							if (longest[4] < l)
							{
								longest[0] = com_dot[i][0];
								longest[1] = com_dot[i][1];
								longest[2] = com_dot[j][0];
								longest[3] = com_dot[j][1];
								longest[4] = l;
							}
						}
					}
				}
			}
			line[count][0] = x_point1 = longest[0];
			line[count][1] = y_point1 = longest[1];
			line[count][2] = x_point2 = longest[2];
			line[count++][3] = y_point2 = longest[3];
			CString str;
			str.Format(_T("%s : (%d, %d), (%d, %d) 개수\r\n"), team_name, x_point1, y_point1, x_point2, y_point2);
			histo += str;
			turn++;
		}
		else
		{
			if (com_turn_cnt == 2)
			{
				start_node = new Node;
				start_node->level = count;
				start_node->LineCnt = count;
				start_node->node_line[0] = line[count - 1][0];
				start_node->node_line[1] = line[count - 1][1];
				start_node->node_line[2] = line[count - 1][2];
				start_node->node_line[3] = line[count - 1][3];
				cur_node = start_node;
				for (int i = 0; i < LIMIT; i++)
				{
					searchline = NULL;

					DFS_Traversal(cur_node);

					for (int j = 0; j < searchsize; j++)
						delete[] searchline[j];
					delete[] searchline;
					searchsize = 0;
				}
			}
			Com_turn();
			//변동 출력.
			CString str;
			str.Format(_T("%s : (%d, %d), (%d, %d) 개수\r\n"), team_name, x_point1, y_point1, x_point2, y_point2);
			histo += str;
			turn++;

		}
		com_turn_cnt++;
		if (com_turn_cnt == 5)
		{
			LIMIT = 3;
			searchline = NULL;

			DFS_Traversal(cur_node);

			for (int j = 0; j < searchsize; j++)
				delete[] searchline[j];
			delete[] searchline;
			searchsize = 0;
		}
		if (com_turn_cnt == 20)
		{
			LIMIT = 4;
			searchline = NULL;

			DFS_Traversal(cur_node);

			for (int j = 0; j < searchsize; j++)
				delete[] searchline[j];
			delete[] searchline;
			searchsize = 0;
		}
	}
	//점수 변경.
	ChangeScore(line, count, red_score, blue_score);
	//변동 출력.
	// 
	// i번째 입력된 X1 : line[i][0], Y1 : line[i][1], X2 : line[i][2], Y2 : line[i][3]
	// 현재 입력된 X1 : x_point1, Y1 : y_point1, X2 : x_point2, Y2 : y_point2;
	// map[가로][세로] : 7x7에 대한 매트릭스로 비었을 경우 0, 점일 경우 1
	// count 현재까지 입력된 직선의 수
	// team_name 현재 입력가능한 팀 이름
	// histo 지금까지 입력된 팀에 따른 좌표들
	// red_score red팀 점수
	// blue_score blue팀 점수
	// 

	if (count % 2 == 0)
		team_name = _T("RED");
	else
		team_name = _T("BLUE");

	x_point1 = 0;
	y_point1 = 0;
	x_point2 = 0;
	y_point2 = 0;


	if (cur_node && cur_node->ChildCnt == 0)
	{
		AfxMessageBox(_T("게임종료"));
	}

	UpdateData(FALSE);
	chk = true; 
}


void CDdangDdaMuckgiDlg::OnBnClickedRestart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	srand((unsigned int)time(NULL));

	for(int i =0; i<7; i++)
		for(int j =0; j<7; j++)
			map[i][j] = rand()%2;

	for(int i=0; i<700; i++){
		for(int j =0; j<700; j++){

			m_view->imageData[(i*m_view->width+j)*3+2] = (unsigned char)255;
			m_view->imageData[(i*m_view->width+j)*3+1] = (unsigned char)255;
			m_view->imageData[(i*m_view->width+j)*3] = (unsigned char)255;

			if(i%100==0 || j%100==0){
				m_view->imageData[(i*m_view->width+j)*3+2] = (unsigned char)200;
				m_view->imageData[(i*m_view->width+j)*3+1] = (unsigned char)200;
				m_view->imageData[(i*m_view->width+j)*3] = (unsigned char)200;
			}
			else if(i%50==0 && j%50==0)
				if(map[((j-50)/100)][((i-50)/100)] == 1){
					for(int y=-2; y<3; y++)
						for(int x=-2; x<3; x++)
						{
							m_view->imageData[((i+y)*m_view->width+(j+x))*3+2] = (unsigned char)0;
							m_view->imageData[((i+y)*m_view->width+(j+x))*3+1] = (unsigned char)0;
							m_view->imageData[((i+y)*m_view->width+(j+x))*3] = (unsigned char)0;
						}
				}
		}
	}

	red_score = 0;
	blue_score = 0;
	count = 0;
	team_name = _T("RED");
	histo = _T("");
	chk = true;

	UpdateData(FALSE);
}

bool CDdangDdaMuckgiDlg::CheckCross(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
	int t1 = (ax1 - ax2)* (by1 - ay1) + (ay1 - ay2) * (ax1 - bx1);
	int t2 = (ax1 - ax2) * (by2 - ay1) + (ay1 - ay2) * (ax1 - bx2);

	int t3 = (bx1 - bx2) * (ay1 - by1) + (by1 - by2) * (bx1 - ax1);
	int t4 = (bx1 - bx2) * (ay2 - by1) + (by1 - by2) * (bx1 - ax2);

	if (t1*t2 < 0 && t3*t4 < 0)
	{
		return true;
	}
	return false;
}

bool CDdangDdaMuckgiDlg::CheckPossibleLine(int x1, int y1, int x2, int y2, int& error,int **line,int count)
{
	int check = true;
	if (map[x1 - 1][y1 - 1] == 1 && map[x2 - 1][y2 - 1] == 1)//정점이 있는지 없는지 검사.
	{
		//이미 존재하는 선분인지 검사.
		for (int i = 0; i < count; i++)
		{
			if (line[i][0] == x1&&line[i][1] == y1&&line[i][2] == x2&&line[i][3] == y2)
			{
				check = false;
				error = 0;
				return check;
			}
			else if (line[i][0] == x2&&line[i][1] == y2&&line[i][2] == x1&&line[i][3] == y1)
			{
				check = false;
				error = 0;
				return check;
			}
		}
	}
	else
	{
		check = false;
		error = 1;
		return check;
	}
	//선분을 그려야 되는데 점을 그리는지 조사.
	if (x1 == x2 && y1 == y2)
	{
		check = false;
		error = 4;
		return check;
	}
	//교차 검사
	for (int i = 0; i < count; i++)
	{
		if (CheckCross(x1, y1, x2, y2, line[i][0], line[i][1], line[i][2], line[i][3]))//교차 검사.
		{
			check = false;
			error = 2;
			return check;
		}
	}

	/*선분위에 점이 여러개인지 검사.*/
	int dx=(x1-x2);
	int dy=(y1-y2);
	if (dx==0)
		dy=dy/abs(dy);
	else if(dy==0)
		dx=dx/abs(dx);
	else
	{
		int gcd=GCD(abs(x1-x2), abs(y1-y2));
		dx/= gcd;
		dy/= gcd;
	}
	for (;;)
	{
		x2 = x2 + dx;
		y2 = y2 + dy;
		if (x2==x1&&y2==y1)
			break;
		if (map[x2-1][y2-1]==1)
		{
			check=false;
			error=3;
		}
	}
	return check;
}

int CDdangDdaMuckgiDlg::GCD(int a, int b)
{
	if (MIN(a,b)!=0)
	{
		if (MAX(a,b)%MIN(a,b)==0)
			return MIN(a,b);
		return GCD(MIN(a,b),MAX(a,b)%MIN(a,b));
	}
}

int CDdangDdaMuckgiDlg::Com_turn()
{
	Node* next_turn = MinMax(cur_node, MIN_VALUE, MAX_VALUE);
	line[count][0]=x_point1=next_turn->node_line[0];
	line[count][1]=y_point1=next_turn->node_line[1];
	line[count][2]=x_point2=next_turn->node_line[2];
	line[count++][3]=y_point2=next_turn->node_line[3];

	
	Clear(cur_node, next_turn, true);
	cur_node = next_turn;

	searchline = NULL;
	DFS_Traversal(cur_node);
	for (int j = 0; j < searchsize; j++)
		delete[] searchline[j];
	delete[] searchline;
	searchsize = 0;

	return 0;
}

bool CDdangDdaMuckgiDlg::CheckInsideVtx(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	CPoint p[3];
	p[0].SetPoint(x1, y1);
	p[1].SetPoint(x2, y2);
	p[2].SetPoint(x3, y3);
	CRgn rgn;
	rgn.CreatePolygonRgn(p, 3, WINDING);
	return rgn.PtInRegion(CPoint(x4, y4));

}

void CDdangDdaMuckgiDlg::ChangeScore(int **line,int count, int &red_score, int &blue_score)
{
	//점을 저장
	red_score = 0;
	blue_score = 0;
	//선분을 3개그리는 모든 경우를 조사해서 지금 그려진 선분과 여태 그려진 선분 조사.
	for (int i = 0; i < count; i++)
	{
		for (int j = i + 1; j < count; j++)
		{
			int ix = -1; //선분 i와 선분 j가 만나는 점의 선분 i에서 x가 저장된 인덱스값.
			int jx = -1;
			if (line[i][0] == line[j][0] && line[i][1] == line[j][1])
			{
				ix = 0; jx = 0;
			}
			else if (line[i][2] == line[j][0] && line[i][3] == line[j][1])
			{
				ix = 2; jx = 0;
			}
			else if (line[i][0] == line[j][2] && line[i][1] == line[j][3])
			{
				ix = 0; jx = 2;
			}
			else if (line[i][2] == line[j][2] && line[i][3] == line[j][3])
			{
				ix = 2; jx = 2;
			}
			else
				continue;

			for (int k = j + 1; k < count; k++)
			{
				//이제 선분끼리 검사해서 이걸로 삼각형이 되는지 검사해야됨.. 
				//그리고 삼각형이 된다면 점수추가.
				//그리고 만약 그 삼각형이 다른 선분이 되는 점을 가지고 있다면 그 삼각형 점수 추가없음.

				//삼각형이 만들어 지는지 선분 k추가해서 검사.
				bool triangle = false;
				if (line[i][(ix + 2) % 4] == line[k][0] && line[i][(ix + 3) % 4] == line[k][1] && line[j][(jx + 2) % 4] == line[k][2] && line[j][(jx + 3) % 4] == line[k][3])
				{
					triangle = true;
				}
				else if (line[i][(ix + 2) % 4] == line[k][2] && line[i][(ix + 3) % 4] == line[k][3] && line[j][(jx + 2) % 4] == line[k][0] && line[j][(jx + 3) % 4] == line[k][1])
				{
					triangle = true;
				}
				else
					continue;

				//삼각형안에 다른 선분이 되는 점을 가지고 잇는가. 
				//선분중 한점이 삼각형 꼭지점중에 한 점위에 있나 검사->있으면 선분의 반대쪽점이 삼각형 내부에 있는지 검사.
				for (int l = 0; l < count; l++)
				{
					if (l != k && l != j && l != i)
					{
						if ((line[i][ix] == line[l][0] && line[i][ix + 1] == line[l][1]) || (line[j][(jx+2)%4] == line[l][0] && line[j][(jx + 3)%4] == line[l][1]) || (line[i][(ix + 2) % 4] == line[l][0] && line[i][(ix + 3)%4] == line[l][1]))
						{
							if (CheckInsideVtx(line[i][ix], line[i][ix + 1], line[i][(ix + 2) % 4], line[i][(ix + 3) % 4], line[j][(jx + 2) % 4], line[j][(jx + 3) % 4], line[l][2], line[l][3]))
							{
								triangle = false;
								continue;
							}
						}
						else if ((line[i][ix] == line[l][2] && line[i][ix + 1] == line[l][3]) || (line[j][(jx + 2) % 4] == line[l][2] && line[j][(jx + 3) % 4] == line[l][3]) || (line[i][(ix + 2) % 4] == line[l][2] && line[i][(ix + 3) % 4] == line[l][3]))
						{
							if (CheckInsideVtx(line[i][ix], line[i][ix + 1], line[i][(ix + 2) % 4], line[i][(ix + 3) % 4], line[j][(jx + 2) % 4], line[j][(jx + 3) % 4], line[l][0], line[l][1]))
							{
								triangle = false;
								continue;
							}
						}
						else
							continue;
					}
				}
				//삼각형이 된다면
				if (triangle)
				{
					int last = 0;
					if (MAX(i, j) != MAX(j, k))
						last = MAX(i, k);
					else
						last = j;
					if (last % 2 == 0)//그 삼각형의 마지막에 그은게 짝수일때: 레드팀
					{
						red_score++;
					}
					else //홀수 일때.
					{
						blue_score++;
					}
				}
			}
		}
	}
}

int CDdangDdaMuckgiDlg::evaluate(int **l,int c)
{
	//미래상황 만듬
	///////////////////////////////////여기까지하면 evaluation에 5개 랜덤선 들어감
	/*
	int red, blue;
	int score1,score2;
	ChangeScore(line,count, red, blue);
	if(turn%2==0)
		score1=red_score-blue_score;
	else
		score1=blue_score-red_score;
	ChangeScore(l, c, red, blue);
	if(turn%2==0)
		score2=red_score-blue_score;
	else
		score2=blue_score-red_score;
		*/
	//현재상태의 점수와 예측상황의 점수를 score1,2에 담음
	int fr,fb,lr,lb;
	ChangeScore(l, c, lr, lb);
	if (first == 0)
		return (lb - blue_score) - (lr - red_score);
	else
		return (lr - red_score) - (lb - blue_score);
	
}


void CDdangDdaMuckgiDlg::DFS_Traversal(Node *n)
{
	for(int i=0;i<n->ChildCnt;i++)
	{
		if (n->Child[i] == NULL)
			return;
		DFS_Traversal(n->Child[i]);
	}
	if(n->ChildCnt==0)
	{
		
		//computer가 놓을 점을 모아놓음
		int error;
		
		int c;
		if (n->LineCnt != 0)
		{
			c = n->LineCnt;
			if (searchsize == 0)
			{
				searchsize = n->LineCnt;
				searchline = new int*[n->LineCnt];
				
				for (int k = 0; k < count; k++)
				{
					searchline[k] = new int[4];
					searchline[k][0] = line[k][0];
					searchline[k][1] = line[k][1];
					searchline[k][2] = line[k][2];
					searchline[k][3] = line[k][3];
				}
				Node *temp_node = n;

				for (int k = n->LineCnt - count - 1; k >= 0; k--)
				{
					searchline[k + count] = new int[4];
					searchline[k + count][0] = temp_node->node_line[0];
					searchline[k + count][1] = temp_node->node_line[1];
					searchline[k + count][2] = temp_node->node_line[2];
					searchline[k + count][3] = temp_node->node_line[3];
					temp_node = temp_node->Parent;
				}
			}
			else
			{
				Node *temp_node = n;

				for (int k = n->LineCnt - count - 1; k >= 0; k--)
				{
					if (searchline[k + count][0] == temp_node->node_line[0] &&
						searchline[k + count][1] == temp_node->node_line[1] &&
						searchline[k + count][2] == temp_node->node_line[2] &&
						searchline[k + count][3] == temp_node->node_line[3])
					{
						break;
					}
					searchline[k + count][0] = temp_node->node_line[0];
					searchline[k + count][1] = temp_node->node_line[1];
					searchline[k + count][2] = temp_node->node_line[2];
					searchline[k + count][3] = temp_node->node_line[3];
					temp_node = temp_node->Parent;
				}
			}
		}
		else
		{
			c = 0;
		}

		int** child_table;
		child_table = new int*[500];
		int k = 0;

		for (int i = 0; i < dot_count; i++)
		{
			for (int j = i + 1; j < dot_count; j++)
			{
				
				if (CheckPossibleLine(com_dot[i][0], com_dot[i][1], com_dot[j][0], com_dot[j][1], error, searchline, c) == 1)
				{
					child_table[k] = new int[4];
					child_table[k][0] = com_dot[i][0];
					child_table[k][1] = com_dot[i][1];
					child_table[k][2] = com_dot[j][0];
					child_table[k][3] = com_dot[j][1];
					k++;
				}
			}
		}
		//com_table이 세로개수는 k 개
		

		
		
		n->CreateChild(child_table,k);
		for (int i = 0; i < k; i++)
			delete[] child_table[i];
		delete[] child_table;
	}
	
}

void CDdangDdaMuckgiDlg::OnBnClickedfirst()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (first)
	{
	case 0:
		first = 1;
		GetDlgItem(IDC_first)->SetWindowText(_T("후공"));
		break;
	case 1:
		first = 0;
		GetDlgItem(IDC_first)->SetWindowText(_T("선공"));
		break;
	}
	
}
