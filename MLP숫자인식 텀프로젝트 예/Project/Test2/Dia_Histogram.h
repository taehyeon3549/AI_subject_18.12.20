#pragma once


// CDia_Histogram ��ȭ �����Դϴ�.

class CDia_Histogram : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Histogram)

public:
	CDia_Histogram(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDia_Histogram();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_HISTO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnHistogram();
	void SetImage(float[]);
	int m_Histogram[256];
	afx_msg void OnPaint();
//	afx_msg void OnHistogram();
};
