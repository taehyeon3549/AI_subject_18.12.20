// Dia_Histogram.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Test2.h"
#include "Dia_Histogram.h"
#include "afxdialogex.h"


// CDia_Histogram 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDia_Histogram, CDialogEx)

CDia_Histogram::CDia_Histogram(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDia_Histogram::IDD, pParent)
{

}

CDia_Histogram::~CDia_Histogram()
{
}

void CDia_Histogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDia_Histogram, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDia_Histogram 메시지 처리기입니다.
void CDia_Histogram::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	register int i;
	// 히스토그램 박스 출력
	dc.MoveTo(20, 30);
	dc.LineTo(20, 130);
	dc.LineTo(275, 130);
	dc.LineTo(275, 30);
	// 각 그레이스케일에 해당하는 히스토그램 출력
	for (i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 130);
		dc.LineTo(20 + i, 130 - m_Histogram[i]);
	}
	// 그레이스케일 레벨 출력
	for (i = 0; i < 256; i++)
	{
		dc.SelectStockObject(DC_PEN);
		dc.SetDCPenColor(RGB(i,i,i));
		dc.MoveTo(20 + i, 140);
		dc.LineTo(20 + i, 155);
	}
}

void CDia_Histogram::SetImage(float histo[256])
{
	// m_Histogram 배열의 최대값을 구한다.
	register int i;
	float max_value = histo[0];
	for (i = 1; i < 256; i++)
	{
		if (histo[i] > max_value)
			max_value = histo[i];
	}
	// m_Histogram 배열의 최대값이 100이 되도록 전체 배열의 값을 조절한다.
	for (i = 0; i < 256; i++)
	{
		m_Histogram[i] =
			(int)(histo[i] *100 / max_value);
		TRACE(_T("%f %f %f\n"), histo[i], max_value, histo[i] * 100);
	}
}