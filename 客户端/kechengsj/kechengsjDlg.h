
// kechengsjDlg.h: 头文件
//

#pragma once


#include "LIST.h"
// CkechengsjDlg 对话框
class CkechengsjDlg : public CDialogEx
{
// 构造
public:
	CkechengsjDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KECHENGSJ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CRect m_largeRect;
	CRect m_smallRect;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	SOCKET socktoServer;
	DWORD server_ip;
	CIPAddressCtrl iptoServer;
	CString username;
	CString userpwd;
	afx_msg void OnBnClickedButton2();
	LIST list;
	int flaglog=0;
protected:
	afx_msg LRESULT OnMsgrecv(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedBtnSet();
};
