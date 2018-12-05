
// Test2View.h : CTest2View 클래스의 인터페이스
//

#pragma once //여기저기에 선언이 되어있어도 한번만 include
#include "RGBBYTE.h"
#include "Dib.h"
#include "resource.h"
#define PI = 3.14

class CTest2View : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CTest2View();
	DECLARE_DYNCREATE(CTest2View)

// 특성입니다.
public:
	CTest2Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CTest2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // Test2View.cpp의 디버그 버전
inline CTest2Doc* CTest2View::GetDocument() const
   { return reinterpret_cast<CTest2Doc*>(m_pDocument); }
#endif

