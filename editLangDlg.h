#pragma once
#include "afxdialogex.h"

// editLangDlg 对话框

class editLangDlg : public CDialog
{
	DECLARE_DYNAMIC(editLangDlg)

public:
	editLangDlg(CWnd* _pParent = nullptr);   // 标准构造函数
	virtual ~editLangDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LANG_DIALOG };
#endif
private:
	CWnd *pParent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox recLang;
	CComboBox outLang1;
	CComboBox outLang2;
	CComboBox outLang3;
	afx_msg void OnBnClickedOk();
};
