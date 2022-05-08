
// subtitleBotDlg.h: 头文件
//

#pragma once

#include <speechapi_cxx.h>
#include "editLangDlg.h"
#include "apiKeyDlg.h"

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;
using namespace Microsoft::CognitiveServices::Speech::Translation;

// CsubtitleBotDlg 对话框
class CsubtitleBotDlg : public CDialogEx
{
// 构造
public:
	CsubtitleBotDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CsubtitleBotDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUBTITLEBOT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedtestb();
	static UINT processingInterpreting(LPVOID params);
private:
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	editLangDlg* editLangW;
	apiKeyDlg* editApiW;
	CEdit textArea;
	CEdit logText;
	CButton beginOrEndTrans;
	bool translatingMutex;
	std::shared_ptr<TranslationRecognizer> recognizer;
	wstring recognizedText;
	wstring recognizingText;
	int inLang, outLang1, outLang2, outLang3;
	string apiKey, region;
public:
	afx_msg void OnEnChangeEdit3();
	afx_msg void editLang();
	friend class editLangDlg;
	friend class apiKeyDlg;
	afx_msg void openApiKeyMenu();
	CButton alwaysTop;
};
