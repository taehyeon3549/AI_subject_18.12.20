#pragma once
#include "Test2Doc.h"
#include "RGBBYTE.h"

// Dia_Meanfilter ��ȭ �����Դϴ�.

class Dia_Meanfilter : public CDialog
{
	DECLARE_DYNAMIC(Dia_Meanfilter)

public:
	Dia_Meanfilter(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~Dia_Meanfilter();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIA_MEANFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString filtersize;
	CTest2Doc *m_Doc;
};
