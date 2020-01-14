// LIST.cpp: 实现文件
//

#include "stdafx.h"
#include "kechengsj.h"
#include "LIST.h"
#include "afxdialogex.h"
#include "cstring"
#include "cmath"
#include "math.h"
#include "algorithm"
#include <map>
#include <string>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#define PORT1 65433
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
struct ChatMsg
{
	int op;
	short port;
	char username[100];
	char msg[1000];
};
short LIST::find_port( )
{
	short MAX = 65434;
	for (int i = 0; i < port_v.size(); i++)
	{
		MAX = max(MAX, port_v[i]);

	}
	return MAX+1;
}



// LIST 对话框

IMPLEMENT_DYNAMIC(LIST, CDialogEx)

LIST::LIST(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIST, pParent)
{

 
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
}


LIST::~LIST()
{
}
BOOL LIST::OnInitDialog()
{
	this->ModifyStyleEx(0, WS_EX_APPWINDOW);
	UpdateData(true);
	CreateSock();
	GetDlgItem(IDC_STATIC)->SetWindowText(username);
	m_list.DeleteAllItems();
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 为列表视图控件添加全行选中和栅格风格

	m_list.InsertColumn(0, "用户名", LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, "状态", LVCFMT_LEFT, 120);
	CString id;
	CString a;
	a.Format("列表长度 %d", listlen);
	//MessageBox(a);
	for (int i = 0; i < listlen; i++)
	{
		id.Format("%d", i);
		if (strcmp(onlinelist[i], username.GetBuffer()) == 0) continue;
		m_list.InsertItem(i, onlinelist[i]);
		m_list.SetItemText(i, 0, onlinelist[i]);
		m_list.SetItemText(i, 1, "在线");
	}

	//UpdateData(false);
	return 1;
}
void LIST::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	return;
}


BEGIN_MESSAGE_MAP(LIST, CDialogEx)
	
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &LIST::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &LIST::OnBnClickedButton1)
//	ON_MESSAGE(RecvMsg, &LIST::OnRecvmsg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &LIST::OnLvnItemchangedList1)
//	ON_MESSAGE(OnPaint, &LIST::OnOnpaint)
ON_MESSAGE(WM_PAINT, &LIST::OnPaint)
ON_BN_CLICKED(IDC_BUTTON2, &LIST::OnBnClickedButton2)
ON_MESSAGE(RecvMsg, &LIST::OnRecvmsg)
END_MESSAGE_MAP()


// LIST 消息处理程序




void LIST::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	int index = -1;
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate != NULL)
	{
		index = pNMItemActivate->iItem;
	}
	LV_ITEM lvitem = { 0 };
	lvitem.iItem = index;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

	m_list.GetItem(&lvitem);
	m_list.GetItemText(index, 0);
	CString name = m_list.GetItemText(index, 0);
	
	UserInfo userInfo;
	userInfo.op = 4;
	strcpy(userInfo.name,name.GetBuffer());
	send(c_sock, (char *)&userInfo, sizeof(userInfo), 0);
	
	//MessageBox(m_list.GetItemText(index, 0));
	*pResult = 0;

}
void LIST::relist()
{
		m_list.DeleteAllItems();
		m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 为列表视图控件添加全行选中和栅格风格
		
		/*m_list.InsertColumn(0, "用户名", LVCFMT_LEFT, 100);
		m_list.InsertColumn(1, "状态", LVCFMT_LEFT, 120);*/
		CString id;
		CString a;
		a.Format("列表长度 %d", listlen);
		//MessageBox(a);
		for (int i = 0; i < listlen; i++)
		{
			id.Format("%d", i);
			if (strcmp(onlinelist[i], username.GetBuffer()) == 0) continue;
			m_list.InsertItem(i, onlinelist[i]);
			m_list.SetItemText(i, 0, onlinelist[i]);
			m_list.SetItemText(i, 1, "在线");
		}

	UpdateData(false);
}
void LIST::OnBnClickedButton1()
{

	if (MessageBox("确定要退出吗？", "提示", MB_YESNO) == IDYES)
	{
		UserInfo userinfo;
		strcpy(userinfo.name, username);
		userinfo.op = 3;
		send(c_sock, (char *)&userinfo, sizeof(userinfo), 0);
		//closesocket(c_sock);
		//CDialogEx::OnOK();
		PostQuitMessage(0);
	}
	else
	{
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
}
void LIST::CreateSock()
{
	//MessageBox("创建UDP套接字");
	if ((DGramSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		CString str1("创建套接字失败");
		MessageBox(str1);
		CDialogEx::OnOK();
	}

	memset((void *)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT1);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(DGramSock, (LPSOCKADDR)&addr, sizeof(addr)) != 0)
	{
		MessageBox("绑定失败！");
		closesocket(DGramSock);
		CDialogEx::OnOK();
	}


	if (WSAAsyncSelect(DGramSock, m_hWnd, RecvMsg, FD_READ) != 0)
	{
		CString str;
		str.Format("异步套接字失败！错误代码:%d", WSAGetLastError());
		MessageBox(str);
		closesocket(DGramSock);
	}
}
/*afx_msg LRESULT LIST::OnRecvmsg(WPARAM wParam, LPARAM lParam)
{
	
}*/

void LIST::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
/*SSSSSSSSSSSSSSS*/

//afx_msg LRESULT LIST::OnOnpaint(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


afx_msg LRESULT LIST::OnPaint(WPARAM wParam, LPARAM lParam)
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
		bmpBackground.LoadBitmap(IDB_BITMAP9);

		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	}

	return 0;

}


void LIST::OnBnClickedButton2()
{
	PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	// TODO: 在此添加控件通知处理程序代码
}


afx_msg LRESULT LIST::OnRecvmsg(WPARAM wParam, LPARAM lParam)
{
	//MessageBox("good lianjie!");
	ChatMsg chatMsg;
	int len = sizeof(fromaddr);
	memset((char *)&chatMsg, '\0', sizeof(chatMsg));
	int size = recvfrom(DGramSock, (char *)&chatMsg, sizeof(chatMsg), 0, (sockaddr *)&fromaddr, &len);

	if (size > 0)
	{
		//recvBuffer[size] = '\0';
		//str.Format("来自于%s:%d的消息：%s", inet_ntoa(fromaddr.sin_addr), ntohs(fromaddr.sin_port), recvBuffer);
		if (ntohl(chatMsg.op) != 0)
		{
			CString temp = chatMsg.username;
			CHAT chat;
			chat.name = chatMsg.username;
			chat.port = find_port();
			chat.mport = ntohs(chatMsg.port);
			chat.username = username;
			chat.IP = inet_ntoa(fromaddr.sin_addr);
			port_v.push_back(chat.port);


			char name[100];
			strcpy(name, chat.name);
			//char *t = strcat(chatMsg.username, "说：");
			CString msg;// = strcat(t, chatMsg.msg);
			msg.Format("%s 说：%s", chatMsg.username, chatMsg.msg);
			chat.msg = msg;

			CString str;
			str.Format("有来自于%s的消息", chatMsg.username);
			if (MessageBox(str, "提示", MB_YESNO | MB_DEFBUTTON2) == IDYES)
			{
				chat.DoModal();
			}
		}
	}
	return 0;
}
