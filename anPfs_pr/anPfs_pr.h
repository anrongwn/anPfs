
// anPfs_pr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CanPfs_prApp: 
// �йش����ʵ�֣������ anPfs_pr.cpp
//

class CanPfs_prApp : public CWinApp
{
public:
	CanPfs_prApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CanPfs_prApp theApp;