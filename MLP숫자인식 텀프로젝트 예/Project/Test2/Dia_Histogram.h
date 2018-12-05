#pragma once


// CDia_Histogram 대화 상자입니다.

class CDia_Histogram : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Histogram)

public:
	CDia_Histogram(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDia_Histogram();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_HISTO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnHistogram();
	void SetImage(float[]);
	int m_Histogram[256];
	afx_msg void OnPaint();
//	afx_msg void OnHistogram();
};
