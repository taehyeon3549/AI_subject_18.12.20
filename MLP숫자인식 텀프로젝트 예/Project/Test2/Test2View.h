
// Test2View.h : CTest2View Ŭ������ �������̽�
//

#pragma once //�������⿡ ������ �Ǿ��־ �ѹ��� include
#include "RGBBYTE.h"
#include "Dib.h"
#include "resource.h"
#define PI = 3.14

class CTest2View : public CScrollView
{
protected: // serialization������ ��������ϴ�.
	CTest2View();
	DECLARE_DYNCREATE(CTest2View)

// Ư���Դϴ�.
public:
	CTest2Doc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CTest2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDibinverse();
	afx_msg void OnDibcontrast();
	afx_msg void OnCorrection();
	afx_msg void OnHistogram();
	afx_msg void OnEqual();
	afx_msg void Onmeanfilter();
	afx_msg void Onmedianfilter();
	void Sort(int*, int );
	float Sort_max(float a[]);
	void K_Mean_Clustering(BYTE** image, int* ME, int K, int T, int width, int heigt);
	void K_Mean_Clustering(BYTE** image, int* ME, int K, int T, int width, int heigt, int first, int end);
	afx_msg void OnEdgeroberts();
	afx_msg void OnEdgesovel();
	afx_msg void OnTranslation();
	afx_msg void OnResizeNearest();
	afx_msg void Onmeanfilter2();
	afx_msg void OnWMeanfilter();
	afx_msg void OnUnsharp();
	afx_msg void OnAddSaltPepper();
	afx_msg void OnDiaMeanfilter();
	afx_msg void OnUnsharpWm();
	afx_msg void OnUnsharpMef();
	afx_msg void OnRotate();
	afx_msg void OnKmeancluster();
	afx_msg void OnLbgclustering();
	afx_msg void OnInputFace();
	afx_msg void OnInputBack();
	afx_msg void OnMl();
	afx_msg void OnMap();
	afx_msg void OnFuzzy();
	afx_msg void OnFuzzyHistoR();
	afx_msg void OnFuzzyHistoG();
	afx_msg void OnFuzzyHistob();
	afx_msg void OnFuzzyInputFace();
	afx_msg void OnFuzzyHistogramRBack();
	afx_msg void OnFuzzyHistogramGBack();
	afx_msg void OnFuzzyHistogramBBack();
	afx_msg void OnFuzzyLarsen();
	afx_msg void OnLabling();
	afx_msg void OnNc();
	afx_msg void OnTrain();
	afx_msg void OnNcTest();
	afx_msg void OnNcBinary();
	afx_msg void OnTest();
};

#ifndef _DEBUG  // Test2View.cpp�� ����� ����
inline CTest2Doc* CTest2View::GetDocument() const
   { return reinterpret_cast<CTest2Doc*>(m_pDocument); }
#endif

