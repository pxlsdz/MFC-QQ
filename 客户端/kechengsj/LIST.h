#pragma once

#include<vector>
#include"CHAT.h"
using namespace std;

// LIST 对话框
const int maxn = 1000;
const int MAXM = 1000;
class LIST : public CDialogEx
{
	DECLARE_DYNAMIC(LIST)

public:
	LIST(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LIST();
	CString username;
	BOOL OnInitDialog();
	 

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	int listlen;
	char onlinelist[500][maxn];
	void LIST::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void relist();
	void CreateSock();
	short find_port();
	vector<short> port_v;
	SOCKET c_sock;
	struct sockaddr_in addr,fromaddr;
	SOCKET DGramSock;
	
	afx_msg void OnBnClickedButton1();
	
	
protected:
	//afx_msg LRESULT OnRecvmsg(WPARAM wParam, LPARAM lParam);
public:
    vector<CHAT> chat_v;
	
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
protected:
	afx_msg LRESULT OnRecvmsg(WPARAM wParam, LPARAM lParam);
};
