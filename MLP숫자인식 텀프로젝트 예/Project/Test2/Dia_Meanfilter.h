#pragma once
#include "Test2Doc.h"
#include "RGBBYTE.h"

// Dia_Meanfilter 대화 상자입니다.

class Dia_Meanfilter : public CDialog
{
	DECLARE_DYNAMIC(Dia_Meanfilter)

public:
	Dia_Meanfilter(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dia_Meanfilter();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIA_MEANFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString filtersize;
	CTest2Doc *m_Doc;
};
