
// subtitleBotDlg.cpp: 实现文件
//

#include <speechapi_cxx.h>
#include <memory>
#include "pch.h"
#include "framework.h"
#include "subtitleBot.h"
#include "subtitleBotDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;
using namespace Microsoft::CognitiveServices::Speech::Translation;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsubtitleBotDlg 对话框



CsubtitleBotDlg::CsubtitleBotDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUBTITLEBOT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CsubtitleBotDlg::~CsubtitleBotDlg()
{
	delete editLangW;
	delete editApiW;
}

void CsubtitleBotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, textArea);
	DDX_Control(pDX, IDC_EDIT3, logText);
	DDX_Control(pDX, testB, beginOrEndTrans);
	DDX_Control(pDX, IDC_CHECK3, alwaysTop);
}

BEGIN_MESSAGE_MAP(CsubtitleBotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(testB, &CsubtitleBotDlg::OnBnClickedtestb)
	ON_BN_CLICKED(testB, &CsubtitleBotDlg::OnBnClickedtestb)
	ON_COMMAND(ID_32771, &CsubtitleBotDlg::editLang)
	ON_COMMAND(ID_32772, &CsubtitleBotDlg::openApiKeyMenu)
	ON_BN_CLICKED(IDC_CHECK3, &CsubtitleBotDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CsubtitleBotDlg 消息处理程序

BOOL CsubtitleBotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	editLangW = new editLangDlg(this);
	editApiW = new apiKeyDlg(this);

	apiKey = "";
	region = "japaneast";

	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	alwaysTop.SetCheck(1);

	inLang = 1;
	outLang1 = 2;
	outLang2 = 1;
	outLang3 = 0;

	ModifyStyleEx(0, WS_EX_LAYERED);
	SetLayeredWindowAttributes(RGB(0, 0, 0), 200, LWA_ALPHA);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsubtitleBotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsubtitleBotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsubtitleBotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT CsubtitleBotDlg::processingInterpreting(LPVOID params)
{
	CsubtitleBotDlg* window = (CsubtitleBotDlg*)params;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	auto config = SpeechTranslationConfig::FromSubscription(window->apiKey, window->region);

	string inLangList[] = { "en-US", "zh-CN", "en-US", "ja-JP", "ko-KR", "fr-FR", "de-DE", "es-ES" };
	string outLangList[] = { "", "zh-Hans", "en", "ja", "ko", "fr", "de", "es" };
	auto autoDetectSourceLanguageConfig = AutoDetectSourceLanguageConfig::FromLanguages({ "en-US", "zh-CN" });

	config->SetSpeechRecognitionLanguage(inLangList[window->inLang]);
	if (window->outLang1) config->AddTargetLanguage(outLangList[window->outLang1]);
	if (window->outLang2) config->AddTargetLanguage(outLangList[window->outLang2]);
	if (window->outLang3) config->AddTargetLanguage(outLangList[window->outLang3]);

	if (window->inLang == 0)
	{
		window->recognizer = TranslationRecognizer::FromConfig(config, autoDetectSourceLanguageConfig);
	}
	else
	{
		window->recognizer = TranslationRecognizer::FromConfig(config);
	}

	//Subscribes to events.
	window->recognizer->Recognizing.Connect([&](const TranslationRecognitionEventArgs& e)
		{
			window->recognizingText.clear();
			window->recognizingText += _T("\r\n");
			//window->recognizingText += converter.from_bytes(e.Result->Text);
			for (const auto& it : e.Result->Translations)
			{
				window->recognizingText += _T("\r\n");
				window->recognizingText += converter.from_bytes(it.second);
			}
			window->textArea.SetWindowTextW((window->recognizedText + window->recognizingText).c_str());
			window->textArea.LineScroll(window->textArea.GetLineCount());
		});

	window->recognizer->Recognized.Connect([&](const TranslationRecognitionEventArgs& e)
		{
			window->recognizingText.clear();
			window->recognizedText += _T("\r\n");
			//window->recognizedText += converter.from_bytes(e.Result->Text);
			for (const auto& it : e.Result->Translations)
			{
				window->recognizedText += _T("\r\n");
				window->recognizedText += converter.from_bytes(it.second);
			}
			window->textArea.SetWindowTextW((window->recognizedText + window->recognizingText).c_str());
			window->textArea.LineScroll(window->textArea.GetLineCount());
		});

	for (int i = 0; i < 30; i++)
	{
		window->recognizedText += _T("\r\n");
	}
	window->recognizer->StartContinuousRecognitionAsync().get();
	while (window->translatingMutex)
		Sleep(500);
	window->recognizer->StopContinuousRecognitionAsync().get();
	window->logText.SetWindowTextW(_T("翻译已结束"));
	window->beginOrEndTrans.SetWindowTextW(_T("开始翻译"));
	return 0;
}

void CsubtitleBotDlg::OnBnClickedtestb()
{
	if (apiKey == "")
	{
		logText.SetWindowTextW(_T("请先填入APIKey"));
		openApiKeyMenu();
		return;
	}
	if (translatingMutex)
	{
		translatingMutex = false;
		logText.SetWindowTextW(_T("正在结束翻译"));
		return;
	}
	translatingMutex = true;
	AfxBeginThread(processingInterpreting, this);
	beginOrEndTrans.SetWindowTextW(_T("结束翻译"));
	logText.SetWindowTextW(_T("翻译已开始"));
}

void CsubtitleBotDlg::editLang()
{
	// TODO: 在此添加命令处理程序代码
	editLangW->DestroyWindow();
	editLangW->Create(IDD_LANG_DIALOG, this);
	editLangW->recLang.SetCurSel(inLang);
	editLangW->outLang1.SetCurSel(outLang1);
	editLangW->outLang2.SetCurSel(outLang2);
	editLangW->outLang3.SetCurSel(outLang3);
}


void CsubtitleBotDlg::openApiKeyMenu()
{
	// TODO: 在此添加命令处理程序代码
	editApiW->DestroyWindow();
	editApiW->Create(IDD_APIKEY_DIALOG, this);
	editApiW->serviceProvider.SetCurSel(0);
	editApiW->serviceRegion.SetWindowTextW(converter.from_bytes(region).c_str());
	editApiW->apiKey.SetWindowTextW(converter.from_bytes(apiKey).c_str());
}


void CsubtitleBotDlg::OnBnClickedCheck3()
{
	if(alwaysTop.GetCheck())
		SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(&this->wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
