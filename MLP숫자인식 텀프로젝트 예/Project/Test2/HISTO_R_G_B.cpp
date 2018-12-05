// HISTO_R_G_B.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Test2.h"
#include "HISTO_R_G_B.h"
#include "afxdialogex.h"


// HISTO_R_G_B ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(HISTO_R_G_B, CDialogEx)

HISTO_R_G_B::HISTO_R_G_B(CWnd* pParent /*=NULL*/)
	: CDialogEx(HISTO_R_G_B::IDD, pParent)
{

}

HISTO_R_G_B::~HISTO_R_G_B()
{
}

void HISTO_R_G_B::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HISTO_R_G_B, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// HISTO_R_G_B �޽��� ó�����Դϴ�.
void HISTO_R_G_B::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	register int i;
	// ������׷� �ڽ� ���
	dc.MoveTo(20, 30);
	dc.LineTo(20, 130);
	dc.LineTo(275, 130);
	dc.LineTo(275, 30);
	// �� �׷��̽����Ͽ� �ش��ϴ� ������׷� ���
	for (i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 130);
		dc.LineTo(20 + i, 130 - m_Histogram[i]);
	}
	// �׷��̽����� ���� ���
	for (i = 0; i < 256; i++)
	{
		dc.SelectStockObject(DC_PEN);
		dc.SetDCPenColor(RGB(i, i, i));
		dc.MoveTo(20 + i, 140);
		dc.LineTo(20 + i, 155);
	}
}

void HISTO_R_G_B::SetImage(float histo[256])
{
	// m_Histogram �迭�� �ִ밪�� ���Ѵ�.
	register int i;
	float max_value = histo[0];
	for (i = 1; i < 256; i++)
	{
		if (histo[i] > max_value)
			max_value = histo[i];
	}
	// m_Histogram �迭�� �ִ밪�� 100�� �ǵ��� ��ü �迭�� ���� �����Ѵ�.
	for (i = 0; i < 256; i++)
	{
		m_Histogram[i] =
			(int)(histo[i] * 100 / max_value);
		//TRACE(_T("%f %f %f\n"), histo[i], max_value, histo[i] * 100);
	}
}