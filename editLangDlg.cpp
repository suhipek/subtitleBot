// editLangDlg.cpp: 实现文件
//

#include "pch.h"
#include "subtitleBot.h"
#include "afxdialogex.h"
#include "editLangDlg.h"
#include "subtitleBotDlg.h"

// editLangDlg 对话框

IMPLEMENT_DYNAMIC(editLangDlg, CDialog)

editLangDlg::editLangDlg(CWnd* _pParent /*=nullptr*/)
	: CDialog(IDD_LANG_DIALOG, _pParent)
{
	pParent = (CsubtitleBotDlg*)_pParent;
}

editLangDlg::~editLangDlg()
{
}

void editLangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, recLang);
	DDX_Control(pDX, IDC_COMBO2, outLang1);
	DDX_Control(pDX, IDC_COMBO3, outLang2);
	DDX_Control(pDX, IDC_COMBO4, outLang3);
}


BEGIN_MESSAGE_MAP(editLangDlg, CDialog)
	ON_BN_CLICKED(IDOK, &editLangDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// editLangDlg 消息处理程序


void editLangDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CsubtitleBotDlg* _pParent = (CsubtitleBotDlg*)pParent;
	TRACE(to_wstring(recLang.GetCurSel()).c_str());
	_pParent->inLang = recLang.GetCurSel();
	_pParent->outLang1 = outLang1.GetCurSel();
	_pParent->outLang2 = outLang2.GetCurSel();
	_pParent->outLang3 = outLang3.GetCurSel();
	CDialog::OnOK();
}
