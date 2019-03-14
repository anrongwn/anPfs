
// anPfs_prDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "anPfs_pr.h"
#include "anPfs_prDlg.h"
#include "afxdialogex.h"
#include "anQue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CanPfs_prDlg 对话框
CanPfs_prDlg::CanPfs_prDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ANPFS_PR_DIALOG, pParent), uvfs_(nullptr),cmfs_(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CanPfs_prDlg::~CanPfs_prDlg() {
	if (uvfs_) {
		uvfs_->stop();

		delete uvfs_;
	}
		
	if (cmfs_)
	{
		delete cmfs_;
	}
	CDialog::~CDialog();

}
void CanPfs_prDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CanPfs_prDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CanPfs_prDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CanPfs_prDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


typedef struct {
	int data;
	QUEUE node;//两个 void*指针的数组
}student;

// CanPfs_prDlg 消息处理程序
BOOL CanPfs_prDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	size_t off = offsetof(student, node);

	// 将“关于...”菜单项添加到系统菜单中。
	student head,second,first;
	head.data = 0;
	head.node[0] = nullptr;
	head.node[1] = &second;

	second.data = 1;
	second.node[0] = &head;
	second.node[1] = &first;

	first.data = 2;
	first.node[0] = &second;
	first.node[1] = nullptr;

	//数组 指针的转换
	QUEUE * pq = &second.node;//指向‘两个 void*指针的数组’的指针
	void *p1 = ((*(pq))[0]);//((*pq)+0)
	void *p2 = ((*(pq))[1]);//((*pq)+1)

	student * ps = (student *)(p1);
	student * psps = (student *)((char*)pq - offsetof(student, node));

	student * p = &head;
	while (true) {
		//if (nullptr == p->node[0])break;
		//if (nullptr == p->node[1])break;
		if (p == nullptr)break;

		int data = p->data;
		p = (student*)p->node[1];
	}

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	auto cpath = this->GetDlgItem(IDC_EDIT_filepath);
	cpath->SetWindowText(R"(C:\ktXFS Config\FormsAnsiChinese_jpr)");

	auto cpostfix = this->GetDlgItem(IDC_EDIT_postfix);
	cpostfix->SetWindowText(R"(.def)");

	this->UpdateData();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CanPfs_prDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CanPfs_prDlg::OnPaint()
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
HCURSOR CanPfs_prDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CanPfs_prDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!cmfs_) {
		cmfs_ = new CanCMFS();
	}

	this->UpdateData(FALSE);

	CString path;
	this->GetDlgItem(IDC_EDIT_filepath)->GetWindowText(path);
	CString postfix;
	this->GetDlgItem(IDC_EDIT_postfix)->GetWindowText(postfix);

	cmfs_->start(path.operator LPCSTR(), postfix.operator LPCSTR());
}


void CanPfs_prDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!uvfs_) {
		uvfs_ = new CanUVFS();
	}

	this->UpdateData(FALSE);

	CString path;
	this->GetDlgItem(IDC_EDIT_filepath)->GetWindowText(path);
	CString postfix;
	this->GetDlgItem(IDC_EDIT_postfix)->GetWindowText(postfix);

	uvfs_->start(path.operator LPCSTR(), postfix.operator LPCSTR());
}
