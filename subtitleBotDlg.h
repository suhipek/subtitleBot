
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


private:
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; // 宽字符和单字符转换器

	editLangDlg* editLangW; // 编辑语言子窗口
	apiKeyDlg* editApiW; // 编辑ApiKey子窗口

	CEdit textArea; // 翻译结果输出
	CEdit logText; // 程序运行状态提示
	CButton beginOrEndTrans; // 开始/结束翻译按钮
	CButton alwaysTop; // 是否置顶窗口按钮

	bool translatingMutex; // 线程锁
	static UINT processingInterpreting(LPVOID params); // 处理翻译线程

	std::shared_ptr<TranslationRecognizer> recognizer; // SDK数据
	wstring recognizedText; // 已完成识别的文本
	wstring recognizingText; // 正在识别的文本
	int inLang, outLang1, outLang2, outLang3; // 输入输出语言
	string apiKey, region; // 云服务访问

	afx_msg void OnBnClickedtestb(); // 开始/结束翻译按钮
	afx_msg void editLang(); // 编辑语言子窗口
	afx_msg void openApiKeyMenu(); // 编辑ApiKey子窗口
	afx_msg void OnBnClickedCheck3(); // 是否置顶窗口按钮

	friend class editLangDlg;
	friend class apiKeyDlg; // 将两个窗口生命为友元，方便他们编辑数据
};
