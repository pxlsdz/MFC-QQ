
// kechengsjDlg.cpp: 实现文件
//

#include "stdafx.h"
#include"resource.h"
#include "kechengsj.h"
#include "kechengsjDlg.h"
#include "afxdialogex.h"
#include "REGISRER.h"
#include<string>
#include<cstring>
#include<iostream>

#include "LIST.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#define PORT 65432
#define PORT1 65433
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
struct UserInfo
{
	int op;
	char name[maxn], pwd[maxn];
	char ip[maxn];
	char online[50][maxn];
	char remsg[maxn];
	int listlen;
	/*void getonline()
	{
		memset(online, '\0', sizeof(online));
		for (int i = 0; i < userip.size(); i++)
		{
			strcpy(online[i], onlinelist[i]);
		}
	}*/
};



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT OnMsgrecv(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_MESSAGE(MsgRecv, &CAboutDlg::OnMsgrecv)
ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CkechengsjDlg 对话框



CkechengsjDlg::CkechengsjDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KECHENGSJ_DIALOG, pParent)
	, server_ip(0)
	, username(_T(""))
	, userpwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CkechengsjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, server_ip);
	DDX_Control(pDX, IDC_IPADDRESS1, iptoServer);
	DDX_CBString(pDX, IDC_COMBO2, username);
	DDX_Text(pDX, IDC_EDIT1, userpwd);
}

BEGIN_MESSAGE_MAP(CkechengsjDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CkechengsjDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CkechengsjDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CkechengsjDlg::OnBnClickedButton2)
	ON_MESSAGE(MsgRecv, &CkechengsjDlg::OnMsgrecv)
	ON_BN_CLICKED(IDC_BUTTON3, &CkechengsjDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_SET, &CkechengsjDlg::OnBnClickedBtnSet)
END_MESSAGE_MAP()


// CkechengsjDlg 消息处理程序

BOOL CkechengsjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);


	if (m_largeRect.IsRectNull())
	{
		CRect sepRect;
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&sepRect);
		GetWindowRect(&m_largeRect);
		m_smallRect.left = m_largeRect.left;
		m_smallRect.top = m_largeRect.top;
		m_smallRect.right = m_largeRect.right;
		m_smallRect.bottom = sepRect.bottom;
	}
	//IDC_SEPARATOR
	SetWindowPos(NULL, 0, 0,
		m_smallRect.Width(), m_smallRect.Height(),
		SWP_NOMOVE | SWP_NOZORDER);
 

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	iptoServer.SetWindowTextA("127.0.0.1");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CkechengsjDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CkechengsjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP8);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CkechengsjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CkechengsjDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CkechengsjDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CkechengsjDlg::OnBnClickedButton1()
{
	UpdateData(true);

	
	

	struct sockaddr_in server_addr; //定义存放服务器端地址的结构变量
	int addr_len = sizeof(struct sockaddr_in); //地址结构变量长度
	
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);  //生成版本号2.2
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		MessageBox("加载winsock.dll失败！\n");
		return ;
	}
	/***创建套接字与服务器端交互***/
	if ((socktoServer = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		CString str = "创建套接字失败！错误代码:" + WSAGetLastError();
		MessageBox(str);
		WSACleanup();
	}
	CString iptoserver;


	iptoServer.GetWindowTextA(iptoserver);
	
	memset((void *)&server_addr, 0, addr_len);//地址结构清0
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(iptoserver);//填写服务器IP地址
	if (connect(socktoServer, (struct sockaddr *)&server_addr, addr_len) != 0)
	{
		MessageBox("连接失败！错误代码:"+ WSAGetLastError());
		closesocket(socktoServer);
		WSACleanup();
		return;
	}
	if (WSAAsyncSelect(socktoServer, m_hWnd, MsgRecv, FD_READ) != 0)
	{
		MessageBox("套接字异步事件注册失败");
		closesocket(socktoServer);
	}
	UserInfo userinfo;
	strcpy(userinfo.name, username);
	strcpy(userinfo.pwd , userpwd);
	userinfo.op = 1;
	send(socktoServer, (char *)&userinfo, sizeof(userinfo),0);
}


void CkechengsjDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	REGISRER re;
	

	CString strIP ;
	iptoServer.GetWindowTextA(strIP);
	re.ipaddr = strIP;
	re.DoModal();
	
}

afx_msg LRESULT CkechengsjDlg::OnMsgrecv(WPARAM wParam, LPARAM lParam)
{	
	int size;
	UserInfo reinfo;
	memset(reinfo.remsg,'\0', sizeof(reinfo.remsg));

	Sleep(10);
	//MessageBox(reinfo.remsg);
	
	
	if ((size = recv(socktoServer, (char *)&reinfo, sizeof(reinfo), 0)) < 0)
	{
		CString str = "接收信息失败！错误代码:" + WSAGetLastError();
		MessageBox(str);
		closesocket(socktoServer);//关闭已连接套接字
		WSACleanup(); //注销WinSock动态链接库
		return 0;
	}

	CString t1;
	t1.Format("op:%d", reinfo.op);
	//MessageBox(t1);
	CString a;
	a.Format("%s", reinfo.remsg);
	reinfo.remsg[3] = '\0';
	reinfo.name[strlen(reinfo.name)] = '\0';

	if (reinfo.op == 1&&flaglog==0)
	{
		if (strcmp(reinfo.remsg, "yes") == 0)
		{
			flaglog = 1;
			MessageBox("登陆成功！");
			CString a ;
			list.username = reinfo.name;
			list.listlen = reinfo.listlen;
			for (int i = 0; i < reinfo.listlen; i++)
			{
				strcpy(list.onlinelist[i], reinfo.online[i]);
			}
			ShowWindow(SW_HIDE);
			
			list.c_sock = socktoServer;
			list.DoModal();
		}
		else
			MessageBox("登陆失败！");
	}
	else if (reinfo.op == 4)
	{
		list.listlen = reinfo.listlen;
		for (int i = 0; i < reinfo.listlen; i++)
		{
			strcpy(list.onlinelist[i], reinfo.online[i]);
		}

		list.relist();
	}
	else if (reinfo.op == 5)
	{
		CString temp= reinfo.name;
		short port = list.find_port();
		CHAT chat;
		chat.name = temp;
		chat.port = port;
		chat.username = username;
		chat.IP = reinfo.ip;
		chat.mport = PORT1;
		list.port_v.emplace_back(port);
		chat.DoModal();
	}
	//MessageBox(reinfo.remsg);
	
	


}


void CkechengsjDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szSetStr;
	if (GetDlgItemText(IDC_BTN_SET, szSetStr), szSetStr == _T("设置↓↓"))
	{
		SetDlgItemText(IDC_BTN_SET, _T("隐藏设置↑↑"));
	}
	else
	{
		SetDlgItemText(IDC_BTN_SET, _T("设置↓↓"));
	}

	if (szSetStr == _T("隐藏设置↑↑"))
	{
		SetWindowPos(NULL, 0, 0,
			m_smallRect.Width(), m_smallRect.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL, 0, 0,
			m_largeRect.Width(), m_largeRect.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
}


void CkechengsjDlg::OnBnClickedBtnSet()
{
	// TODO: 在此添加控件通知处理程序代码
	CString szSetStr;
	if (GetDlgItemText(IDC_BTN_SET, szSetStr), szSetStr == _T("设置↓↓"))
	{
		SetDlgItemText(IDC_BTN_SET, _T("隐藏设置↑↑"));
	}
	else
	{
		SetDlgItemText(IDC_BTN_SET, _T("设置↓↓"));
	}

	if (szSetStr == _T("隐藏设置↑↑"))
	{
		SetWindowPos(NULL, 0, 0,
			m_smallRect.Width(), m_smallRect.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL, 0, 0,
			m_largeRect.Width(), m_largeRect.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
