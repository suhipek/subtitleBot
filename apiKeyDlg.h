#pragma once
#include "afxdialogex.h"


// apiKeyDlg 对话框

class apiKeyDlg : public CDialog
{
	DECLARE_DYNAMIC(apiKeyDlg)

public:
	apiKeyDlg(CWnd* _pParent = nullptr);   // 标准构造函数
	virtual ~apiKeyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APIKEY_DIALOG };
#endif
private:
	CWnd* pParent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox serviceProvider;
	CEdit apiKey;
	CEdit serviceRegion;
	afx_msg void OnBnClickedOk();
};
