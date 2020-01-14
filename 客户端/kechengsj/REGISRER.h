#pragma once


// REGISRER 对话框

class REGISRER : public CDialogEx
{
	DECLARE_DYNAMIC(REGISRER)

public:
	REGISRER(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~REGISRER();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CIPAddressCtrl iptoServer;
	BOOL REGISRER::OnInitDialog();
	CString ipaddr;
	CString username;
	CString pwd;
};
