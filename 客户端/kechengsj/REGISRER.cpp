// REGISRER.cpp: 实现文件
//

#include "stdafx.h"
#include "kechengsj.h"
#include "REGISRER.h"
#include "afxdialogex.h"
#include "LIST.h"
#include<cstring>
#include<string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#define PORT 65432
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




// REGISRER 对话框

IMPLEMENT_DYNAMIC(REGISRER, CDialogEx)

REGISRER::REGISRER(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REGISTER, pParent)
	, username(_T(""))
	, pwd(_T(""))
{

}

REGISRER::~REGISRER()
{
}

void REGISRER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, iptoServer);
	DDX_Text(pDX, IDC_EDIT1, username);

	DDX_Text(pDX, IDC_EDIT2, pwd);
}


BOOL REGISRER::OnInitDialog()
{
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText(ipaddr);
	return 1;
}

BEGIN_MESSAGE_MAP(REGISRER, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &REGISRER::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &REGISRER::OnBnClickedButton3)
END_MESSAGE_MAP()


// REGISRER 消息处理程序


void REGISRER::OnBnClickedButton2()
{
	
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void REGISRER::OnBnClickedButton3()
{

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	SOCKET regisock;       //注册套接字

	//MessageBox(userinfo.name);
	//MessageBox(userinfo.pwd);
	struct sockaddr_in server_addr; //定义存放服务器端地址的结构变量
	int addr_len = sizeof(struct sockaddr_in); //地址结构变量长度
	//char msgbuffer[1000]; //接收/发送信息的缓冲区
	/***初始化winsock DLL***/

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);  //生成版本号2.2
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		MessageBox("加载winsock.dll失败！\n");
		return;
	}
	/***创建套接字与服务器端交互***/
	if ((regisock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		CString str = "创建套接字失败！错误代码:" + WSAGetLastError();
		MessageBox(str);
		WSACleanup();
	}
	CString iptoserver;
	iptoServer.GetWindowTextA(iptoserver);
	//MessageBox(iptoserver);
	memset((void *)&server_addr, 0, addr_len);//地址结构清0
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(iptoserver);//填写服务器IP地址
	if (connect(regisock, (struct sockaddr *)&server_addr, addr_len) != 0)
	{
		//cout << "连接失败！错误代码:" << WSAGetLastError() << endl;
		MessageBox("连接失败！错误代码:" + WSAGetLastError());
		closesocket(regisock);
		WSACleanup();
		return;
	}




	UserInfo userinfo, reinfo;
	strcpy(userinfo.name, username);
	strcpy(userinfo.pwd, pwd);
	userinfo.op = 2;

	send(regisock, (char *)&userinfo, sizeof(userinfo), 0);
	recv(regisock, (char*)&reinfo, sizeof(reinfo), 0);
	MessageBox(reinfo.remsg);
	if (strcmp(reinfo.remsg, "注册成功") == 0)
	{
		//closesocket(regisock);
		CDialogEx::OnOK();
	}

	// TODO: 在此添加控件通知处理程序代码
}
