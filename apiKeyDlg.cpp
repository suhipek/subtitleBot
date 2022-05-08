// apiKeyDlg.cpp: 实现文件
//

#include "pch.h"
#include "subtitleBot.h"
#include "afxdialogex.h"
#include "apiKeyDlg.h"
#include "subtitleBotDlg.h"

// apiKeyDlg 对话框

IMPLEMENT_DYNAMIC(apiKeyDlg, CDialog)

apiKeyDlg::apiKeyDlg(CWnd* _pParent /*=nullptr*/)
	: CDialog(IDD_APIKEY_DIALOG, _pParent)
{
	pParent = (CsubtitleBotDlg*)_pParent;
}

apiKeyDlg::~apiKeyDlg()
{
}

void apiKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, serviceProvider);
	DDX_Control(pDX, IDC_EDIT1, apiKey);
	DDX_Control(pDX, IDC_EDIT2, serviceRegion);
}


BEGIN_MESSAGE_MAP(apiKeyDlg, CDialog)
	ON_BN_CLICKED(IDOK, &apiKeyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// apiKeyDlg 消息处理程序


void apiKeyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CsubtitleBotDlg* _pParent = (CsubtitleBotDlg*)pParent;
	CString _apikey, _region;
	apiKey.GetWindowTextW(_apikey);
	serviceRegion.GetWindowTextW(_region);
	_pParent->apiKey = _pParent->converter.to_bytes(_apikey.GetString());
	_pParent->region = _pParent->converter.to_bytes(_region.GetString());
	CDialog::OnOK();
}
