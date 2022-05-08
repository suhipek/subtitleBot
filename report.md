# 高级语言程序设计实验报告

老师/助教您好，为防止信息泄露，请您使用[这个飞书链接](https://nankai.feishu.cn/file/boxcnMeqlx8riUcKGFEceVvN7Vf)获得带署名和标题页面的实验报告，造成不便，十分抱歉。

<div STYLE="page-break-after: always;"></div>

## 目录

[toc]

<div STYLE="page-break-after: always;"></div>

## 作业题目

实时语音同传翻译软件的设计。

## 开发平台

Visual Studio 2022 + Azure Cognitive Services SDK 1.21.0

## 技术选型

要实现一款同传翻译软件，主要的难点有两个：实现语音识别和翻译算法，以及实现软件的交互。

鉴于手动实现语音识别和翻译算法难度较大，因此作者选择了使用云服务来完成语音识别和翻译的步骤。在比较市面上提供语音识别或翻译服务的公有云厂商后，作者发现微软公司的Azure云提供了较丰富的学生使用额度，并且只需要教育邮箱即可申请。

Azure的语音翻译服务属于认知服务的范畴，可以通过REST API的方式或者SDK包的方式进行开发，这里作者选择使用了C++平台的SDK包。而Azure认知服务的SDK包和微软的Visual Studio是深度耦合的，因此在图形界面的实现方面，我们选择了微软的MFC库。

## 功能规划

经过对联想预装的智能会议助手中的同传功能的研究，作者认为，一款可用的同传翻译软件至少需要有如下几个功能：

- 自动翻页显示
- 随时开始和停止翻译
- 修改翻译的语言
- 将窗口锁定在顶层

此外，由于使用了公有云，对于访问凭据的管理功能也是必不可少的

## 实现过程

要实现如上功能，我们需要三个MFC中的窗口（Dialog）。其中有一个主窗口，负责呈现翻译的文本，放置开始和结束翻译的开关，以及将窗口锁定在顶层的复选框。我们还需要两个窗口，分别用于编辑语言和公有云的访问凭据。

### 主窗口类

主窗口的类声明如下：

```c++
class CsubtitleBotDlg : public CDialogEx
{
public:
	CsubtitleBotDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CsubtitleBotDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

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
```

主窗口中最关键的控件，是用于输出翻译结果的Edit Control（编辑框）控件，以及用于开始结束翻译的按钮。但是翻译的过程是一个需要持续时间很长的过程，如果我们将这一过程的代码放在开始和结束翻译的按钮事件处理程序中，当我们按下按钮后，整个程序便会进入“卡死”的状态，这是因为翻译的过程将用于处理界面的程序阻塞住了，因此这里的实现我们需要用的MFC的线程，具体实现会在下文中讲解。

除了翻译功能之外，我们还需要一个用于将窗口置顶的Check（复选框）控件，在MFC中，`SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);`这句命令可以实现当前窗口的置顶操作，至于取消置顶，我们可以使用`SetWindowPos(&this->wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);`

### 主窗口与子窗口

此外，主窗口也是进入子菜单以及设置界面的门户，为了实现这一点，我们使用了MFC的Menu功能。在我们的实现中，子窗口是父窗口的成员之一，会在。这里需要注意的一点是，在子窗口关闭后，需要调用其`DestroyWindow`方法，否则重复打开子窗口是会出现内存访问冲突。而当父窗口关闭时，我们也需要在析构函数中加入析构子窗口的语句，以防止内存泄漏。

主窗口当然还需要接受子窗口中设置的值。为了实现这一点，我们可以将子窗口设置为主窗口的友元函数，并在创建子窗口的过程中将this指针传参。这样一来，子窗口中的设置被编辑后，事件处理函数便可以直接访问和修改父窗口中的设置值。

### 翻译线程

在Azure认知服务SDK中，进行一次实时翻译的基本流程如下：

- 创建config，并调用其方法为其添加访问凭据和语言设置
- 用之前创建的config初始化recognizer对象
- 指定回调函数，在这里我们通过C++11的特性lambda表达式实现
- 调用recognizer对象的`StartContinuousRecognitionAsync`方法，开始翻译
- 阻塞，直到翻译结束，调用`StopContinuousRecognitionAsync`方法

之前说到，翻译的过程会对处理图形的线程造成阻塞，导致程序卡死，因此当用户按下翻译按键后，我们便通过`AfxBeginThread`新开一个线程进行处理。我们将线程函数定义为主窗口类的静态函数，并将this指针转为`void*`类型传参给线程函数，这样它就可以访问主窗口类中的设置相关变量。

```
input:
	我觉得西南村那家脆皮肉夹馍挺不错的
recognizing:
	我觉得
	I think
recognizing:
	我觉得西南村
	I think that the Southwest Village
recognizing:
	我觉得西南村那家脆皮肉夹馍
	I think the crispy meat bun in the southwest village
recognized:
	我觉得西南村那家脆皮肉夹馍挺不错的
	I think the southwest village of the crispy meat bun is quite good
```

除此之外，线程中创建的回调函数也值得注意，这是因为我们需要创建两个函数-recognizing和recognized，分别用来接收正在翻译的内容和翻译完成的内容。比如说话人说了：“我觉得西南村那家脆皮肉夹馍挺不错的”这么一句话，这句话是不可能在一瞬间说完的，而当说话人说到“我觉得西南村”时，识别算法便会返回一次正在翻译的内容：“I think that the Southwest Village”，如果这句话过长，返回正在翻译内容的次数可能还会很多。当说话人说完后，翻译完成回调函数便会收到一次调用。

基于如上的实现机制，我们在主窗口对象中需要维护两个变量：`recognizedText`和`recognizingText`。每当recognizing回调被调用时，便会将`recognizingText`清空，并写入这次收到的内容。而当recognized回调被调用时，也会将`recognizingText`清空，并在`recognizedText`后拼接这次收到的内容，然后再将其写回。

除此之外，我们还需要注意到MFC的Edit控件是无法自动翻页的。为了将最新翻译出的文本始终保持在最下方，我们需要在翻译开始时输出足够清屏数量的空行，并在每次刷新Edit控件中的内容后调用`textArea.LineScroll(textArea.GetLineCount());`来将编辑框滑到最下方的位置。

翻译线程不可能一直运行下去，当用户需要结束翻译的的时候，它应该能够自行退出，为了实现这一点，我们使用了互斥锁的概念，通过主窗口类的一个成员变量，将线程锁（死循环）在结束前的某个地方，只有这个成员变量被编辑，锁才能解开，线程才能结束。

## 效果与收获

在这次课程设计中，我们实现了一个可用的、基于云端的实时语音同传翻译小工具，这使得我们对面对对象编程、用户界面编程、云服务的使用都有了更加深刻的理解。在编程方面，我们值得注意的收获有如下几点：

### 线程和锁

现代操作系统最大的特性之一便是多任务并行，程序员可以通过线程来实现这一目标。MFC中也有线程相关的类和方法。

```c++
UINT CsubtitleBotDlg::processingInterpreting(LPVOID params)
{
	window->recognizer->StartContinuousRecognitionAsync().get();
	while (window->translatingMutex)
		Sleep(500);
	window->recognizer->StopContinuousRecognitionAsync().get();
	window->logText.SetWindowTextW(_T("翻译已结束"));
	return 0;
}

void CsubtitleBotDlg::OnBnClickedtestb()
{
	if (translatingMutex)
	{
		translatingMutex = false;
		logText.SetWindowTextW(_T("正在结束翻译"));
		return;
	}
	translatingMutex = true;
	AfxBeginThread(processingInterpreting, this);
	logText.SetWindowTextW(_T("翻译已开始"));
}
```

### auto类型

auto类型是c++11中引入的新特性，可以实现对变量类型类自动推导，进而大大节省代码量：

```c++
auto config = SpeechTranslationConfig::FromSubscription(window->apiKey, window->region);
```

### lambda表达式

lambda表达式是c++11中引入的新特性，可以当作可以在函数中定义的没有名字的函数理解。在一些情况下，lambda表达式可以极大地方便程序的编写

```c++
CsubtitleBotDlg* window = (CsubtitleBotDlg*)params;
// &表示将所有变量传入，包括window
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
```

### 友类

友类可以访问该类的私有成员，这里用来实现父窗口和子窗口间的信息传递。

```c++
friend class editLangDlg;
friend class apiKeyDlg; // 将两个窗口生命为友元，方便他们编辑数据
```

### 空型指针传参的方式

线程函数，创建窗口函数中的参数被定义为了空型指针`void*`，使用其实现传参的代码如下：

```c++
AfxBeginThread(processingInterpreting, this);
logText.SetWindowTextW(_T("翻译已开始"));
// 线程函数
UINT CsubtitleBotDlg::processingInterpreting(LPVOID params)
{
	CsubtitleBotDlg* window = (CsubtitleBotDlg*)params;
	//...
}
```

### 宽字符类型

为了实现全球文字的显示，ISO组织推出了Unicode（万国码）的文本编码方式，和asiic不同，Unicode字符串的基本存储单位不是一个字节的char，而是两个字节wchar_t。MFC中大量使用了Unicode来做到全球语言支持的特性，然而在这两种类型之间的转换比较麻烦。我们可以使用`std::wstring_convert`来实现两者的转换。

```c++
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; // 宽字符和单字符转换器
converter.to_bytes(_region.GetString());
window->recognizingText += converter.from_bytes(it.second);
```



