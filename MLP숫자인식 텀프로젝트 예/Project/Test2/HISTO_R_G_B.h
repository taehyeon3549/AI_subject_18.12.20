#pragma once


// HISTO_R_G_B ��ȭ �����Դϴ�.

class HISTO_R_G_B : public CDialogEx
{
	DECLARE_DYNAMIC(HISTO_R_G_B)

public:
	HISTO_R_G_B(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~HISTO_R_G_B();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_HISTO_R_G_B };

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
