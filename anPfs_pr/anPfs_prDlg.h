
// anPfs_prDlg.h : ͷ�ļ�
//

#pragma once
#include "CanUVFS.h"

// CanPfs_prDlg �Ի���
class CanPfs_prDlg : public CDialogEx
{
// ����
public:
	CanPfs_prDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CanPfs_prDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANPFS_PR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

private:
	CanUVFS *uvfs_;
};
