// CHAT.cpp: 实现文件
// CHAT.cpp: 实现文件
//

#include "stdafx.h"
#include "kechengsj.h"
#include "CHAT.h"
#include "afxdialogex.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include <vector>
using namespace std;
struct ChatMsg
{
	int op;
	short port;
	char username[100];
	char msg[1000];
};

// CHAT 对话框

IMPLEMENT_DYNAMIC(CHAT, CDialogEx)

CHAT::CHAT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHAT, pParent)
	, m_Edit(_T(""))
	
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CHAT::~CHAT()
{
}

void CHAT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_LIST1, m_list);
}
BOOL CHAT::OnInitDialog()
{
	

	UpdateData(false);
	CreateSock();
	sockaddr_in toaddr;
	toaddr.sin_family = AF_INET;
	toaddr.sin_addr.s_addr = inet_addr(IP);
	toaddr.sin_port = htons(mport);

	ChatMsg chatmsg;
	chatmsg.op = htonl(0);
	chatmsg.port = htons(port);

	sendto(chat_sock, (char *)&chatmsg, sizeof(chatmsg), 0, (sockaddr*)&toaddr, sizeof(toaddr));

	m_list.AddString(msg);
	GetDlgItem(IDC_STATIC)->SetWindowText(name);
	return 1;
}


BEGIN_MESSAGE_MAP(CHAT, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHAT::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &CHAT::OnLbnSelchangeList1)
//	ON_WM_ACTIVATE()
	ON_MESSAGE(ChatRecv, &CHAT::OnChatrecv)
	ON_MESSAGE(WM_PAINT, &CHAT::OnPaint)
END_MESSAGE_MAP()


// CHAT 消息处理程序
void CHAT::CreateSock()
{
	if ((chat_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		CString str1("创建套接字失败");
		MessageBox(str1);
		CDialogEx::OnOK();
	}

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(chat_sock, (LPSOCKADDR)&addr, sizeof(addr)) != 0)
	{
		MessageBox("绑定失败！");
		closesocket(chat_sock);
		CDialogEx::OnOK();
	}


	if (WSAAsyncSelect(chat_sock, m_hWnd, ChatRecv, FD_READ) != 0)
	{
		CString str;
		str.Format("异步套接字失败!!!!!!!！错误代码:%d", WSAGetLastError());
		MessageBox(str);
		closesocket(chat_sock);
	}
}

void CHAT::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	struct sockaddr_in toaddr;	
	memset((void *)&toaddr, 0, sizeof(addr));


	toaddr.sin_family = AF_INET;
	toaddr.sin_addr.s_addr = inet_addr(IP);
	toaddr.sin_port = htons(mport);
	m_list.AddString("我说:" + m_Edit);
	ChatMsg chatmsg;
	strcpy(chatmsg.msg, m_Edit.GetBuffer());
	strcpy(chatmsg.username, username);
	chatmsg.op = htonl(1);
	chatmsg.port = htons(port);
	sendto(chat_sock, (char *)&chatmsg,sizeof(chatmsg), 0, (sockaddr*)&toaddr, sizeof(toaddr));

	//CDialogEx::OnOK();
}


void CHAT::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	closesocket(chat_sock);

}


//void CHAT::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: 在此处添加消息处理程序代码
//}


afx_msg LRESULT CHAT::OnChatrecv(WPARAM wParam, LPARAM lParam)
{

	
	sockaddr_in fromaddr;
	int len = sizeof(fromaddr);
	ChatMsg chatmsg;
	int size = recvfrom(chat_sock, (char *)&chatmsg, sizeof(chatmsg), 0, (sockaddr *)&fromaddr, &len);
	if (size > 0)
	{

		int flag = ntohl(chatmsg.op);

		if (flag==0)
			mport = ntohs(chatmsg.port);
		else
		{
			char *t = strcat(chatmsg.username, "说：");
			CString msg1 = strcat(t, chatmsg.msg);
			m_list.AddString(msg1);
			UpdateData(false);
		}
		
	}
	return 0;
}


afx_msg LRESULT CHAT::OnPaint(WPARAM wParam, LPARAM lParam)
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
		CDialogEx::OnPaint();
		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP10);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	}

	return 0;
}
