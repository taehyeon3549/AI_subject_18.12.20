// Dia_Meanfilter.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Test2.h"
#include "Dia_Meanfilter.h"
#include "afxdialogex.h"


// Dia_Meanfilter ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(Dia_Meanfilter, CDialog)

Dia_Meanfilter::Dia_Meanfilter(CWnd* pParent /*=NULL*/)
	: CDialog(Dia_Meanfilter::IDD, pParent)
	, filtersize(_T(""))
{

}

Dia_Meanfilter::~Dia_Meanfilter()
{
}

void Dia_Meanfilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, filtersize);
}


BEGIN_MESSAGE_MAP(Dia_Meanfilter, CDialog)
	ON_BN_CLICKED(IDOK, &Dia_Meanfilter::OnBnClickedOk)
END_MESSAGE_MAP()


// Dia_Meanfilter �޽��� ó�����Դϴ�.


void Dia_Meanfilter::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialog::OnOK();
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	if (m_Doc->m_Dib.IsValid()){
		w = m_Doc->m_Dib.GetWidth();
		h = m_Doc->m_Dib.GetHeight();
		ptr = m_Doc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	CString size; 
	GetDlgItem(IDC_EDIT1)->GetWindowText(size); // �ؽ�Ʈ�ڽ����� ���ڸ� size�� ����

	int n = _ttoi(size); // ���ڿ��� ���ڷ�
	int k, l;
	int nw, mw; //index�� ����Ŵ.
	nw = mw = n / 2; //�ʱ�ȭ
	int temp_r, temp_g, temp_b; //RGB�� �ӽ������ϴ� ����

	for (k = 0 + nw; k < h - nw; k++){
		for (l = 0 + mw; l < w - mw; l++){
			temp_r = temp_g = temp_b = 0; // temp �ʱ�ȭ
			for (i = 0; i < n; i++){
				for (j = 0; j < n; j++){
					temp_r += ptr[k + i - nw][l + j - mw].r;
					temp_g += ptr[k + i - nw][l + j - mw].g;
					temp_b += ptr[k + i - nw][l + j - mw].b;
				}
			}
			ptr2[k][l].r = (BYTE)limit(temp_r/(n*n)); 
			ptr2[k][l].g = (BYTE)limit(temp_g/(n*n));
			ptr2[k][l].b = (BYTE)limit(temp_b/(n*n));
		}
	}

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			ptr[i][j].r = ptr2[i][j].r;
			ptr[i][j].g = ptr2[i][j].g;
			ptr[i][j].b = ptr2[i][j].b;
		}
	}
}
