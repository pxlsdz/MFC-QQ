#pragma once


// CHAT 对话框

class CHAT : public CDialogEx
{
	DECLARE_DYNAMIC(CHAT)

public:
	CHAT(CWnd* pParent = nullptr);   // 标准构造函数
	
	CHAT( CHAT &a)
	{
		this->addr = a.addr;
		for (int i = 0; i < a.m_list.GetCount(); i++)
		{
			CString str;
			a.m_list.GetText(0, str);
			this->m_list.AddString(str);
	}
		this->name = a.name;
		this->chat_sock = a.chat_sock;
		this->addr = a.addr;
		this->IP = a.IP;
		this->m_Edit = a.m_Edit;
	
	}
	virtual ~CHAT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	CString username;
	CString name;
	SOCKET chat_sock;
	sockaddr_in addr;
	BOOL OnInitDialog();
	void CreateSock();

	afx_msg void OnBnClickedOk();
	CString IP;
	CString m_Edit;
	CListBox m_list;
	CString msg;
	short port,mport;
	afx_msg void OnLbnSelchangeList1();
	CHAT &operator = (const CHAT &a)
	{
		this->addr = a.addr;
		for (int i = 0; i < a.m_list.GetCount(); i++)
		{
			CString str;
			a.m_list.GetText(0, str);
			this->m_list.AddString(str);
		}
		this->name = a.name;
		this->chat_sock = a.chat_sock;
		this->addr = a.addr;
		this->IP = a.IP;
		this->m_Edit = a.m_Edit;
		return *this;
	}

//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
protected:
	afx_msg LRESULT OnChatrecv(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
};
