
// anPfs_prDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "anPfs_pr.h"
#include "anPfs_prDlg.h"
#include "afxdialogex.h"
#include "anQue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CanPfs_prDlg �Ի���
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
	QUEUE node;//���� void*ָ�������
}student;

// CanPfs_prDlg ��Ϣ�������
BOOL CanPfs_prDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	size_t off = offsetof(student, node);

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
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

	//���� ָ���ת��
	QUEUE * pq = &second.node;//ָ������ void*ָ������顯��ָ��
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

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	auto cpath = this->GetDlgItem(IDC_EDIT_filepath);
	cpath->SetWindowText(R"(C:\ktXFS Config\FormsAnsiChinese_jpr)");

	auto cpostfix = this->GetDlgItem(IDC_EDIT_postfix);
	cpostfix->SetWindowText(R"(.def)");

	this->UpdateData();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CanPfs_prDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CanPfs_prDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CanPfs_prDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
