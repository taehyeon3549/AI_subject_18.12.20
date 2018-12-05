
// Test2View.cpp : CTest2View Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Test2.h"
#endif

#include "Dia_Histogram.h"
#include "Dia_Meanfilter.h"
#include "Test2Doc.h"
#include "Test2View.h"
#include "Dib.h"
#include <math.h>
#include "resource.h"
#include "HISTO_R_G_B.h"
#include <ostream>
#include <fstream>
#include <io.h>
#include "mlp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14
#define RGB2GRAY(r,g,b)(0.299*(r) + 0.587*(g)+ 0.114*(b)) // ������ gray��.
// CTest2View

using namespace std;

IMPLEMENT_DYNCREATE(CTest2View, CScrollView)

BEGIN_MESSAGE_MAP(CTest2View, CScrollView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(Dibinverse, &CTest2View::OnDibinverse)
	ON_COMMAND(Dibcontrast, &CTest2View::OnDibcontrast)
	ON_COMMAND(Gamma_Correction, &CTest2View::OnCorrection)
	ON_COMMAND(Histogram, &CTest2View::OnHistogram)
	ON_COMMAND(Histrogram_Equal, &CTest2View::OnEqual)
	ON_COMMAND(ID_medianFilter, &CTest2View::Onmedianfilter)
	ON_COMMAND(ID_EdgeRoberts, &CTest2View::OnEdgeroberts)
	ON_COMMAND(ID_EdgeSovel, &CTest2View::OnEdgesovel)
	ON_COMMAND(ID_Translation, &CTest2View::OnTranslation)
	ON_COMMAND(ID_RESIZE_NEAREST, &CTest2View::OnResizeNearest)
	ON_COMMAND(ID_meanfilter, &CTest2View::Onmeanfilter2)
	ON_COMMAND(ID_W_MEANFILTER, &CTest2View::OnWMeanfilter)
	ON_COMMAND(ID_UNSHARP, &CTest2View::OnUnsharp)
	ON_COMMAND(ID_ADD_SALT_PEPPER, &CTest2View::OnAddSaltPepper)
	ON_COMMAND(ID_DIA_MEANFILTER, &CTest2View::OnDiaMeanfilter)
	ON_COMMAND(ID_UNSHARP_WM, &CTest2View::OnUnsharpWm)
	ON_COMMAND(ID_UNSHARP_MEF, &CTest2View::OnUnsharpMef)
	ON_COMMAND(ID_ROTATE, &CTest2View::OnRotate)
	ON_COMMAND(ID_KMEANCLUSTER, &CTest2View::OnKmeancluster)
	ON_COMMAND(ID_LBGCLUSTERING, &CTest2View::OnLbgclustering)
	ON_COMMAND(ID_INPUT_FACE, &CTest2View::OnInputFace)
	ON_COMMAND(ID_INPUT_BACK, &CTest2View::OnInputBack)
	ON_COMMAND(ID_ML, &CTest2View::OnMl)
	ON_COMMAND(ID_MAP, &CTest2View::OnMap)
	ON_COMMAND(ID_FUZZY_HISTO_R, &CTest2View::OnFuzzyHistoR)
	ON_COMMAND(ID_FUZZY_HISTO_G, &CTest2View::OnFuzzyHistoG)
	ON_COMMAND(ID_FUZZY_HISTO_b, &CTest2View::OnFuzzyHistob)
	ON_COMMAND(ID_FUZZY_HISTOGRAM_R_BACK, &CTest2View::OnFuzzyHistogramRBack)
	ON_COMMAND(ID_FUZZY_HISTOGRAM_G_BACK, &CTest2View::OnFuzzyHistogramGBack)
	ON_COMMAND(ID_FUZZY_HISTOGRAM_B_BACK, &CTest2View::OnFuzzyHistogramBBack)
	ON_COMMAND(ID_FUZZY_LARSEN, &CTest2View::OnFuzzyLarsen)
	ON_COMMAND(ID_LABLING, &CTest2View::OnLabling)
	//ON_COMMAND(ID_MLP_Nc, &CTest2View::OnMlpNc)
	ON_COMMAND(ID_TRAIN, &CTest2View::OnTrain)
	ON_COMMAND(ID_NC_BINARY, &CTest2View::OnNcBinary)
	ON_COMMAND(ID_TEST, &CTest2View::OnTest)
END_MESSAGE_MAP()

// CTest2View ����/�Ҹ�
CTest2View::CTest2View()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CTest2View::~CTest2View()
{
}

BOOL CTest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CTest2View �׸���

void CTest2View::OnDraw(CDC* pDC)
{
	CTest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_Dib.IsValid())
		pDoc->m_Dib.Draw(pDC->m_hDC);

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}

void CTest2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	CTest2Doc*pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid())
	{
		sizeTotal.cx = pDoc->m_Dib.GetWidth();
		sizeTotal.cy = pDoc->m_Dib.GetHeight();
	}
	else
	{
		sizeTotal.cy = sizeTotal.cx = 100;
	}

	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	//sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(true); // false�� �ϸ� ����ũ��
}


// CTest2View �μ�

BOOL CTest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CTest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CTest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CTest2View ����

#ifdef _DEBUG
void CTest2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTest2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTest2Doc* CTest2View::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest2Doc)));
	return (CTest2Doc*)m_pDocument;
}
#endif //_DEBUG


// CTest2View �޽��� ó����


void CTest2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDib dib;
	dib.Load(_T("lenna.bmp"));

	CClientDC dc(this);
	dib.Draw(dc.m_hDC, point.x, point.y);

	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CTest2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);

	return TRUE;
}


void CTest2View::OnDibinverse() // ������
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;

	RGBBYTE** ptr = NULL;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()) {
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			ptr[i][j].r = 255 - ptr[i][j].r;
			ptr[i][j].g = 255 - ptr[i][j].g;
			ptr[i][j].b = 255 - ptr[i][j].b;
		}
	}
	Invalidate();
}


void CTest2View::OnDibcontrast() // ��� ����
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h, n = -40; //n = �׷��� ������ ��
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	BYTE value;

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3;
			ptr[i][j].r = ptr[i][j].g = ptr[i][j].b =
				limit(value + ((value - 128) * n / 100));
		}
	}
	Invalidate(); // ���밪 �ٷ� ������
}


void CTest2View::OnCorrection() // ��������
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	float value;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	float gamma = 2.5f; // LCD ����Ͱ�� �뷫 2.5
	float invgamma = 1.f / gamma; //gamma�� ����

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3.0f;
			ptr[i][j].r = ptr[i][j].g = ptr[i][j].b =
				(BYTE)limit(pow(value / 255.f, invgamma) * 255 + 0.5f); //pow������ math.h include
		}
	}
	Invalidate(); // ���밪 �ٷ� ������
}

void CTest2View::OnHistogram() // ������׷�
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	BYTE value;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	//������׷� ���
	int temp[256];
	float histo[256] = { 0.f, };
	memset(temp, 0, sizeof(int) * 256);
	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3.0f;
			temp[value]++;
		}
	}

	//������׷� ����ȭ, �׷��� �����ϰ��� ������ �ȼ��� ��Ÿ�� Ȯ��
	float area = (float)w*h; //��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = temp[i] / area;

	CDia_Histogram dig;
	dig.SetImage(histo);
	dig.DoModal();
}


void CTest2View::OnEqual() // ������׷� ��Ȱȭ
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	BYTE value;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	//������׷� ���
	int temp[256];
	float histo[256] = { 0.f, };
	memset(temp, 0, sizeof(int) * 256);

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3.0f;
			temp[value]++;
		}
	}

	//������׷� ����ȭ
	float area = (float)w*h; //��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = temp[i] / area;

	//������׷� ���� �Լ� ���
	double cdf[256] = { 0.0, };
	cdf[0] = histo[0]; // histo[0]�� Ȯ������. 0~1������ �Ǽ�.
	for (i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + histo[i]; // ��� ����.

	//������׷� �յ�ȭ
	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3;
			ptr[i][j].r = ptr[i][j].g = ptr[i][j].b = (BYTE)limit(cdf[value] * 255); // rgb������ �ٲ��ֱ� ����
		}
	}
	Invalidate();
}

void CTest2View::Onmedianfilter() // �߰��� ����
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, RGB(255, 255, 255));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int k, l;

	int NW = 3, MW = 3, NW2, MW2; // NW * MW = ���� ũ��
	int *tr = new int[NW*MW];
	memset(tr, 0, sizeof(int)*NW*MW);
	int *tg = new int[NW*MW];
	memset(tg, 0, sizeof(int)*NW*MW);
	int *tb = new int[NW*MW];
	memset(tb, 0, sizeof(int)*NW*MW);

	NW2 = NW / 2;
	MW2 = MW / 2;
	for (k = 0 + NW2; k < h - NW2; k++) // ���͸� y������ ������ �̵�.
		for (l = 0 + MW2; l < w - MW2; l++){ // ���͸� x������ ������ �̵�
			for (i = 0; i < NW; i++) // ���� ������ �ȼ��� ���. y������ �̵�
				for (j = 0; j < MW; j++){ // ���ͳ��� �ȼ� ���. x������ �̵�
					tr[MW*i + j] = ptr[k + i - NW2][l + j - MW2].r;
					tg[MW*i + j] = ptr[k + i - NW2][l + j - MW2].g;
					tb[MW*i + j] = ptr[k + i - NW2][l + j - MW2].b;
				}
			Sort(tr, NW*MW);
			Sort(tg, NW*MW);
			Sort(tb, NW*MW);
			ptr2[k][l].r = (BYTE)limit(tr[NW*MW / 2]);
			ptr2[k][l].g = (BYTE)limit(tg[NW*MW / 2]);
			ptr2[k][l].b = (BYTE)limit(tb[NW*MW / 2]);
		}

	delete[]tr;
	delete[]tg;
	delete[]tb;

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}

void CTest2View::Sort(int * t, int NW){ // ����
	int i, k, r;
	i = 0;
	while (i == 0){
		i = 1;
		for (k = 0; k < NW - 1; k++)
			if (t[k]>t[k + 1]){
				i = 0; r = t[k + 1]; t[k + 1] = t[k]; t[k] = r;
			}
	}
}


void CTest2View::OnEdgeroberts() // ���� �ι�Ʈ
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, RGB(255, 255, 255));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();
	int h1, h2; // h1 = x�� �̺�, h2 = y�� �̺�. 90�� ȸ��
	double hval, th = 20;
	for (i = 1; i < h - 1; i++)
		for (j = 1; j < w - 1; j++){
			h1 = ptr[i - 1][j - 1].r - ptr[i][j].r;
			h2 = ptr[i - 1][j + 1].r - ptr[i][j - 1].r;
			hval = sqrt((double)h1*h1 + h2*h2);
			ptr2[i][j].r = (BYTE)limit(hval);

			//if ((BYTE)limit(hval)>th) ptr2[i][j].r = 255;
			//else ptr2[i][j].r = 0;

			h1 = ptr[i - 1][j - 1].g - ptr[i][j].g;
			h2 = ptr[i - 1][j + 1].g - ptr[i][j - 1].g;
			hval = sqrt((double)h1*h1 + h2*h2);
			ptr2[i][j].g = (BYTE)limit(hval);

			//if ((BYTE)limit(hval)>th) ptr2[i][j].g = 255;
			//else ptr2[i][j].g = 0;

			h1 = ptr[i - 1][j - 1].b - ptr[i][j].b;
			h2 = ptr[i - 1][j + 1].b - ptr[i][j - 1].b;
			hval = sqrt((double)h1*h1 + h2*h2);
			ptr2[i][j].b = (BYTE)limit(hval);

			//if ((BYTE)limit(hval)>th) ptr2[i][j].b = 255;
			//else ptr2[i][j].b = 0;
		}

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}


void CTest2View::OnEdgesovel() // �׵θ��� ǥ��
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, RGB(255, 255, 255));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();
	int h1, h2;
	h1 = h2 = 0;
	double hval, th = 10;
	for (i = 1; i < h - 1; i++)
		for (j = 1; j < w - 1; j++){
			h1 = ptr[i - 1][j - 1].r + 2 * ptr[i - 1][j].r + ptr[i - 1][j + 1].r
				- ptr[i + 1][j - 1].r - 2 * ptr[i + 1][j].r - ptr[i + 1][j + 1].r;

			h2 = -ptr[i - 1][j - 1].r - 2 * ptr[i][j - 1].r - ptr[i + 1][j - 1].r
				+ ptr[i - 1][j + 1].r + 2 * ptr[i][j + 1].r + ptr[i + 1][j + 1].r;
			hval = sqrt((double)(h1*h1 + h2*h2));
			ptr2[i][j].r = (BYTE)limit(hval);

			h1 = ptr[i - 1][j - 1].g + 2 * ptr[i - 1][j].g + ptr[i - 1][j + 1].g
				- ptr[i + 1][j - 1].g - 2 * ptr[i + 1][j].g - ptr[i + 1][j + 1].g;

			h2 = -ptr[i - 1][j - 1].g - 2 * ptr[i][j - 1].g - ptr[i + 1][j - 1].g
				+ ptr[i - 1][j + 1].g + 2 * ptr[i][j + 1].g + ptr[i + 1][j + 1].g;
			hval = sqrt((double)(h1*h1 + h2*h2));
			ptr2[i][j].g = (BYTE)limit(hval);

			h1 = ptr[i - 1][j - 1].b + 2 * ptr[i - 1][j].b + ptr[i - 1][j + 1].b
				- ptr[i + 1][j - 1].b - 2 * ptr[i + 1][j].b - ptr[i + 1][j + 1].b;

			h2 = -ptr[i - 1][j - 1].b - 2 * ptr[i][j - 1].b - ptr[i + 1][j - 1].b
				+ ptr[i - 1][j + 1].b + 2 * ptr[i][j + 1].b + ptr[i + 1][j + 1].b;
			hval = sqrt((double)(h1*h1 + h2*h2));
			ptr2[i][j].b = (BYTE)limit(hval);
		}
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}




void CTest2View::OnTranslation() //�̵�
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, RGB(255, 255, 255));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int x, y, sx = -32, sy = -32;

	for (i = 1; i < h - 1; i++)
		for (j = 1; j - w; j++){
			x = j - sx;
			y = i - sy;
			if (x >= 0 && x < w && y >= 0 && y < h){
				ptr2[i][j].r = ptr[y][x].r;
				ptr2[i][j].g = ptr[y][x].g;
				ptr2[i][j].b = ptr[y][x].b;
			}
		}
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}


void CTest2View::OnResizeNearest() // ũ������
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	double nw = w *10, nh = h * 10; // ũ�� �۰� �غ���? w*0.5, h*0.5
	Dib1.CreateRGBImage((int)nw, (int)nh, RGB(255, 255, 255));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int x, y;
	x = y = 0;
	// Forwarding mapping
	/*for (i = 0; i < h; i++)
	for (j = 0; j < w; j++){
	x = (int)nw / w*j;
	y = (int)nh / h*i;

	ptr2[y][x].r = ptr[i][j].r;
	ptr2[y][x].g = ptr[i][j].g;
	ptr2[y][x].b = ptr[i][j].b;
	}*/

	//Backward mapping
	for (j = 0; j < (int)nh; j++){
		for (i = 0; i < (int)nw; i++){
			x = (int)(w / nw *i);
			y = (int)(h / nh *j);
			ptr2[j][i].r = ptr[y][x].r;
			ptr2[j][i].g = ptr[y][x].g;
			ptr2[j][i].b = ptr[y][x].b;

		}
	}

	CClientDC dc(this);
	Dib1.Draw(dc.m_hDC, 0, 0, nw, nh);
}

void CTest2View::Onmeanfilter2() // ��հ� ����
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int temp;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			temp =
				1 * ptr[j - 1][i - 1].r + 1 * ptr[j - 1][i].r + 1 * ptr[j - 1][i + 1].r
				+ 1 * ptr[j][i - 1].r + 1 * ptr[j][i].r + 1 * ptr[j][i + 1].r
				+ 1 * ptr[j + 1][i - 1].r + 1 * ptr[j + 1][i].r + 1 * ptr[j + 1][i + 1].r;
			ptr2[j][i].r = (BYTE)limit(temp / 9. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].g + 1 * ptr[j - 1][i].g + 1 * ptr[j - 1][i + 1].g
				+ 1 * ptr[j][i - 1].g + 1 * ptr[j][i].g + 1 * ptr[j][i + 1].g
				+ 1 * ptr[j + 1][i - 1].g + 1 * ptr[j + 1][i].g + 1 * ptr[j + 1][i + 1].g;
			ptr2[j][i].g = (BYTE)limit(temp / 9. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].b + 1 * ptr[j - 1][i].b + 1 * ptr[j - 1][i + 1].b
				+ 1 * ptr[j][i - 1].b + 1 * ptr[j][i].b + 1 * ptr[j][i + 1].b
				+ 1 * ptr[j + 1][i - 1].b + 1 * ptr[j + 1][i].b + 1 * ptr[j + 1][i + 1].b;
			ptr2[j][i].b = (BYTE)limit(temp / 9. + 0.5);
		}
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}



void CTest2View::OnWMeanfilter() // ����ġ ���� (2 / 4 / 2)
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int temp;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			temp =
				1 * ptr[j - 1][i - 1].r + 2 * ptr[j - 1][i].r + 1 * ptr[j - 1][i + 1].r
				+ 2 * ptr[j][i - 1].r + 4 * ptr[j][i].r + 2 * ptr[j][i + 1].r
				+ 1 * ptr[j + 1][i - 1].r + 2 * ptr[j + 1][i].r + 1 * ptr[j + 1][i + 1].r;
			ptr2[j][i].r = (BYTE)limit(temp / 16. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].g + 2 * ptr[j - 1][i].g + 1 * ptr[j - 1][i + 1].g
				+ 2 * ptr[j][i - 1].g + 4 * ptr[j][i].g + 2 * ptr[j][i + 1].g
				+ 1 * ptr[j + 1][i - 1].g + 2 * ptr[j + 1][i].g + 1 * ptr[j + 1][i + 1].g;
			ptr2[j][i].g = (BYTE)limit(temp / 16. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].b + 2 * ptr[j - 1][i].b + 1 * ptr[j - 1][i + 1].b
				+ 2 * ptr[j][i - 1].b + 4 * ptr[j][i].b + 2 * ptr[j][i + 1].b
				+ 1 * ptr[j + 1][i - 1].b + 2 * ptr[j + 1][i].b + 1 * ptr[j + 1][i + 1].b;
			ptr2[j][i].b = (BYTE)limit(temp / 16. + 0.5);
		}
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}


void CTest2View::OnUnsharp() // ����� ����ũ, Mean-Filter
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();


	int temp;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			temp =
				1 * ptr[j - 1][i - 1].r + 1 * ptr[j - 1][i].r + 1 * ptr[j - 1][i + 1].r
				+ 1 * ptr[j][i - 1].r + 1 * ptr[j][i].r + 1 * ptr[j][i + 1].r
				+ 1 * ptr[j + 1][i - 1].r + 1 * ptr[j + 1][i].r + 1 * ptr[j + 1][i + 1].r;
			ptr2[j][i].r = (BYTE)limit(temp / 9. + 0.5); // 0.5�� ���ϴ°��� �Ǽ��� ������ ����� �����̴�.

			temp =
				1 * ptr[j - 1][i - 1].g + 1 * ptr[j - 1][i].g + 1 * ptr[j - 1][i + 1].g
				+ 1 * ptr[j][i - 1].g + 1 * ptr[j][i].g + 1 * ptr[j][i + 1].g
				+ 1 * ptr[j + 1][i - 1].g + 1 * ptr[j + 1][i].g + 1 * ptr[j + 1][i + 1].g;
			ptr2[j][i].g = (BYTE)limit(temp / 9. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].b + 1 * ptr[j - 1][i].b + 1 * ptr[j - 1][i + 1].b
				+ 1 * ptr[j][i - 1].b + 1 * ptr[j][i].b + 1 * ptr[j][i + 1].b
				+ 1 * ptr[j + 1][i - 1].b + 1 * ptr[j + 1][i].b + 1 * ptr[j + 1][i + 1].b;
			ptr2[j][i].b = (BYTE)limit(temp / 9. + 0.5);
		}
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = limit(ptr[j][i].r + (ptr[j][i].r - ptr2[j][i].r));
			ptr[j][i].g = limit(ptr[j][i].g + (ptr[j][i].g - ptr2[j][i].g));
			ptr[j][i].b = limit(ptr[j][i].b + (ptr[j][i].b - ptr2[j][i].b));
		}
	Invalidate();
}


void CTest2View::OnAddSaltPepper() //Salt&Pepper Noise 
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	//RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int salt, pepper;

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			salt = rand() % 255; // �ұ��� ���� 0~255���̰����� ����
			pepper = rand() % 255; // ������ ���� 0~255 ���̰����� ����
			if (salt > 250){ // �������� ���� ����
				ptr[j][i].r = 0;
				ptr[j][i].g = 0;
				ptr[j][i].b = 0;
			}

			if (pepper > 250){
				ptr[j][i].r = 255;
				ptr[j][i].g = 255;
				ptr[j][i].b = 255;
			}
		}
	Invalidate();
}


void CTest2View::OnDiaMeanfilter()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CTest2Doc* pDoc = GetDocument();
	Dia_Meanfilter dlg;
	dlg.m_Doc = pDoc;
	dlg.DoModal(); // ���̾�α� â ���

	Invalidate();
}


void CTest2View::OnUnsharpWm()// ���������ũ, WEIGHTED-MEAN-FILTER
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int temp;
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			temp =
				1 * ptr[j - 1][i - 1].r + 2 * ptr[j - 1][i].r + 1 * ptr[j - 1][i + 1].r
				+ 2 * ptr[j][i - 1].r + 4 * ptr[j][i].r + 2 * ptr[j][i + 1].r
				+ 1 * ptr[j + 1][i - 1].r + 2 * ptr[j + 1][i].r + 1 * ptr[j + 1][i + 1].r;
			ptr2[j][i].r = (BYTE)limit(temp / 16. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].g + 2 * ptr[j - 1][i].g + 1 * ptr[j - 1][i + 1].g
				+ 2 * ptr[j][i - 1].g + 4 * ptr[j][i].g + 2 * ptr[j][i + 1].g
				+ 1 * ptr[j + 1][i - 1].g + 2 * ptr[j + 1][i].g + 1 * ptr[j + 1][i + 1].g;
			ptr2[j][i].g = (BYTE)limit(temp / 16. + 0.5);

			temp =
				1 * ptr[j - 1][i - 1].b + 2 * ptr[j - 1][i].b + 1 * ptr[j - 1][i + 1].b
				+ 2 * ptr[j][i - 1].b + 4 * ptr[j][i].b + 2 * ptr[j][i + 1].b
				+ 1 * ptr[j + 1][i - 1].b + 2 * ptr[j + 1][i].b + 1 * ptr[j + 1][i + 1].b;
			ptr2[j][i].b = (BYTE)limit(temp / 16. + 0.5);
		}
	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = limit(ptr[j][i].r + (ptr[j][i].r - ptr2[j][i].r));
			ptr[j][i].g = limit(ptr[j][i].g + (ptr[j][i].g - ptr2[j][i].g));
			ptr[j][i].b = limit(ptr[j][i].b + (ptr[j][i].b - ptr2[j][i].b));
		}
	Invalidate();
}


void CTest2View::OnUnsharpMef() //���������ũ, Median-Filter
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, RGB(255, 255, 255));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int k, l;

	int NW = 3, MW = 3, NW2, MW2;
	int *tr = new int[NW*MW];
	memset(tr, 0, sizeof(int)*NW*MW);
	int *tg = new int[NW*MW];
	memset(tg, 0, sizeof(int)*NW*MW);
	int *tb = new int[NW*MW];
	memset(tb, 0, sizeof(int)*NW*MW);

	NW2 = NW / 2;
	MW2 = MW / 2;
	for (k = 0 + NW2; k < h - NW2; k++)
		for (l = 0 + MW2; l < w - MW2; l++){
			for (i = 0; i < NW; i++)
				for (j = 0; j < MW; j++){
					tr[MW*i + j] = ptr[k + i - NW2][l + j - MW2].r;
					tg[MW*i + j] = ptr[k + i - NW2][l + j - MW2].g;
					tb[MW*i + j] = ptr[k + i - NW2][l + j - MW2].b;
				}
			Sort(tr, NW*MW);
			Sort(tg, NW*MW);
			Sort(tb, NW*MW);
			ptr2[k][l].r = (BYTE)limit(tr[NW*MW / 2]);
			ptr2[k][l].g = (BYTE)limit(tg[NW*MW / 2]);
			ptr2[k][l].b = (BYTE)limit(tb[NW*MW / 2]);
		}

	delete[]tr;
	delete[]tg;
	delete[]tb;

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = limit(ptr[j][i].r + (ptr[j][i].r - ptr2[j][i].r));
			ptr[j][i].g = limit(ptr[j][i].g + (ptr[j][i].g - ptr2[j][i].g));
			ptr[j][i].b = limit(ptr[j][i].b + (ptr[j][i].b - ptr2[j][i].b));
		}
	Invalidate();
}




void CTest2View::OnRotate()
{
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	int x, y, x1, y1, x2, y2;
	double rx, ry;

	double angle = 45.;
	double rad = (angle * PI / 180);
	double cos_value = cos(-rad);
	double sin_value = sin(-rad);

	//ȸ�� ����
	int Yhalf = h / 2;
	int Xhalf = w / 2;
	for (y = 0; y < h; y++)
		for (x = 0; x < w; x++){
			rx = ((x - Xhalf) * cos_value - (y - Yhalf)*sin_value) + Xhalf;
			ry = ((y - Yhalf) * cos_value + (x - Xhalf)*sin_value) + Yhalf;

			if (rx < 0 || rx > w - 1 || ry < 0 || ry > h - 1)
				continue;

			ptr2[y][x].r = (BYTE)limit(ptr[(int)ry][(int)rx].r);
			ptr2[y][x].g = (BYTE)limit(ptr[(int)ry][(int)rx].g);
			ptr2[y][x].b = (BYTE)limit(ptr[(int)ry][(int)rx].b);
		}

	for (j = 1; j < h - 1; j++)
		for (i = 1; i < w - 1; i++){
			ptr[j][i].r = ptr2[j][i].r;
			ptr[j][i].g = ptr2[j][i].g;
			ptr[j][i].b = ptr2[j][i].b;
		}
	Invalidate();
}


void CTest2View::OnKmeancluster()
{
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	BYTE** image = new BYTE*[h];

	for (i = 0; i < h; i++){
		image[i] = new BYTE[w];
	}

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			image[j][i] = (ptr[j][i].r + ptr[j][i].g + ptr[j][i].b) / 3.0f;
		}

	int K = 10, T = 1; // K = ������ ����. T = �Ӱ� ��.
	int *ME = new int[K]; // ���� �߽ɰ� ���� �޸�

	//�ʱ� ���� �߽ɰ��� ���� ���� �߻�.
	srand((unsigned)time(NULL));
	for (i = 0; i < K; i++) ME[i] = rand() % (255);

	//���� �Լ� ȣ��
	K_Mean_Clustering(image, ME, K, T, w, h);

	//���� ��� �κ�
	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			ptr[j][i].r = image[j][i];
			ptr[j][i].g = image[j][i];
			ptr[j][i].b = image[j][i];
		}

	delete[] image;
	delete[] ME;
	Invalidate();
}

void CTest2View::K_Mean_Clustering(BYTE** image, int* ME, int K, int T, int w, int h){
	float* C_mean = new float[K]; //�� ������ �߽� �� ��� �޸�
	float* distance = new float[K]; // �� �������� �߽� �Ÿ� ��
	int* Count = new int[K]; // �� ������ ���� ����
	int* Cluster = new int[K]; // �� ���� ���� �ȼ��� �� ���� ���� �޸�
	int i, j, k, s;

	//���� ��ȣ�� ����� �޸�
	int** label = new int*[h];
	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	float min, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) // �ʱ�ȭ
	{
		C_mean[k] = distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}

	Sum_mean = min = temp_mean = 0.0f;

	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //�ʱ� ��� ���� ����
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			//�� �ȼ��� �� ���� �߽� ������� �Ÿ��� ���ϱ�
			for (k = 0; k < K; k++) distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float�� ���밪.
			min = distance[0];
			s = 0;

			//�Ÿ����� ���� ���� ��հ��� ã�´�
			for (k = 0; k < K; k++)
			{
				if (distance[k] <= min)
				{
					min = distance[k];
					s = k;
				}
			}
			//�ش� �ȼ����� �ջ�( ���߿� ���ο� Ŭ�������� ��� ����� ����)
			Cluster[s] += image[i][j];

			//�ش� �ȼ��� �Ÿ��� ���� ����� Ŭ������ ��ȣ�� ����.
			label[i][j] = s;

			//���ο� ���(�߽�) ����� ���� ���� ī��Ʈ
			Count[s]++;
		}
	}//end of for

	for (k = 0; k < K; k++)
	{
		temp_mean = C_mean[k];
		if (Count[k] == 0) C_mean[k] = 0.0;
		else
		{
			// �� ������ ��� ���ϱ�
			C_mean[k] = (float)(Cluster[k] / Count[k]);
		}

		//�� ������ �߽ɰ��� ���� �߽ɰ����� ���̰� ���ϱ�.
		Sum_mean += (float)fabs(C_mean[k] - temp_mean);
		temp_mean = 0.0;
	}

	if (Sum_mean <= T) // ���� �߽� ���� �հ� ���� �߽� ���� ���� ���� ����
	{
		for (k = 0; k < K; k++)//�� ������ ���ο� �߽ɰ����� ä�� ��
		{
			for (i = 0; i < h; i++)
				for (j = 0; j < w; j++)
				{
					if (label[i][j] == k)image[i][j] = (unsigned char)C_mean[k];
				}
		}
		return; // ���̰� ���� ���ٸ�
	}

	else  //���̰� ũ�ٸ� �ٽ� Ŭ������ �۾�
	{
		Sum_mean = 0.0;
		for (k = 0; k < K; k++)ME[k] = (int)C_mean[k];
		K_Mean_Clustering(image, ME, K, T, w, h);
	}//���� ��� ��

	////�ݺ����� �ٲ�
	//for (int iter = 0; iter < T; iter++)
	//{
	//	//for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //�ʱ� ��� ���� ����
	//	for (i = 0; i < h; i++)
	//	{
	//		for (j = 0; j < w; j++)
	//		{
	//			//�� �ȼ��� �� ���� �߽� ������� �Ÿ��� ���ϱ�
	//			for (k = 0; k < K; k++) distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float�� ���밪.

	//			min = distance[0];
	//			s = 0;

	//			//�Ÿ����� ���� ���� ��հ��� ã�´�
	//			for (k = 0; k < K; k++)
	//			{
	//				if (distance[k] <= min)
	//				{
	//					min = distance[k];
	//					s = k;
	//				}
	//			}
	//			//�ش� �ȼ����� �ջ�( ���߿� ���ο� Ŭ�������� ��� ����� ����)
	//			Cluster[s] += image[i][j];

	//			//�ش� �ȼ��� �Ÿ��� ���� ����� Ŭ������ ��ȣ�� ����.
	//			label[i][j] = s;

	//			//���ο� ���(�߽�) ����� ���� ���� ī��Ʈ
	//			Count[s]++;
	//		}
	//	}//end of for

	//	for (k = 0; k < K; k++)
	//	{
	//		if (Count[k] == 0) C_mean[k] = 0.0;
	//		else
	//		{
	//			// �� ������ ��� ���ϱ�
	//			C_mean[k] = (float)(Cluster[k] / Count[k]);
	//		}
	//	}

	//	for (k = 0; k < K; k++) ME[k] = (int)C_mean[k];
	//	for (k = 0; k < K; k++)//�� ������ ���ο� �߽ɰ����� ä�� ��
	//			{
	//				for (i = 0; i < h; i++)
	//					for (j = 0; j < w; j++)
	//					{
	//						if (label[i][j] == k)image[i][j] = (unsigned char)C_mean[k];
	//					}
	//			}
	//}

	//�޸� ����
	for (i = 0; i < h; i++)
	{
		delete[] label[i];
	}
	delete[] label;
	delete[] C_mean;
	delete[] distance;
	delete[] Count;
	delete[] Cluster;
}

void CTest2View::K_Mean_Clustering(BYTE** image, int* ME, int K, int T, int w, int h, int first, int end){
	float* C_mean = new float[K]; //�� ������ �߽� �� ��� �޸�
	float* distance = new float[K]; // �� �������� �߽� �Ÿ� ��
	int* Count = new int[K]; // �� ������ ���� ����
	int* Cluster = new int[K]; // �� ���� ���� �ȼ��� �� ���� ���� �޸�
	int i, j, k, s;

	//���� ��ȣ�� ����� �޸�
	int** label = new int*[h];
	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	float min, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) // �ʱ�ȭ
	{
		C_mean[k] = distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}

	Sum_mean = min = temp_mean = 0.0f;

	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //�ʱ� ��� ���� ����
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			//�� �ȼ��� �� ���� �߽� ������� �Ÿ��� ���ϱ�
			for (k = 0; k < K; k++)
			{
				if (first <= image[i][j] && end >= image[i][j]) // �ְ��� ū ������ �ִٸ�
					distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float�� ���밪.
			}

			min = distance[0];
			s = 0;

			//�Ÿ����� ���� ���� ��հ��� ã�´�
			for (k = 1; k < K; k++)
			{
				if (distance[k] <= min)
				{
					min = distance[k];
					s = k;
				}
			}
			//�ش� �ȼ����� �ջ�( ���߿� ���ο� Ŭ�������� ��� ����� ����)
			Cluster[s] += image[i][j];

			//�ش� �ȼ��� �Ÿ��� ���� ����� Ŭ������ ��ȣ�� ����.
			label[i][j] = s;

			//���ο� ���(�߽�) ����� ���� ���� ī��Ʈ
			Count[s]++;
		}
	}//end of for

	for (k = 0; k < K; k++)
	{
		temp_mean = C_mean[k];
		if (Count[k] == 0) C_mean[k] = 0.0;
		else
		{
			// �� ������ ��� ���ϱ�
			C_mean[k] = (float)(Cluster[k] / Count[k]);
		}

		//�� ������ �߽ɰ��� ���� �߽ɰ����� ���̰� ���ϱ�.
		Sum_mean += (float)fabs(C_mean[k] - temp_mean);
		temp_mean = 0.0;
	}

	if (Sum_mean < T) // ���� �߽� ���� �հ� ���� �߽� ���� ���� ���� ����
	{
		return; // ���̰� ���� ���ٸ�
	}

	else  //���̰� ũ�ٸ� �ٽ� Ŭ������ �۾�
	{
		Sum_mean = 0.0;
		for (k = 0; k < K; k++)ME[k] = (int)C_mean[k];
		K_Mean_Clustering(image, ME, K, T, w, h, first, end);
	} 
	 //���� ��͹�


	////�ݺ����� �ٲ�
	//for (int iter = 0; iter < T; iter++)
	//{
	//	//for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //�ʱ� ��� ���� ����
	//	for (i = 0; i < h; i++)
	//	{
	//		for (j = 0; j < w; j++)
	//		{
	//			//�� �ȼ��� �� ���� �߽� ������� �Ÿ��� ���ϱ�
	//			for (k = 0; k < K; k++)
	//			{
	//				if (first <= image[i][j] && image[i][j] <= end) // �ְ��� ū ������ �ִٸ�
	//					distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float�� ���밪.
	//			}

	//			min = distance[0];
	//			s = 0;

	//			//�Ÿ����� ���� ���� ��հ��� ã�´�
	//			for (k = 0; k < K; k++)
	//			{
	//				if (distance[k] <= min)
	//				{
	//					min = distance[k];
	//					s = k;
	//				}
	//			}
	//			//�ش� �ȼ����� �ջ�( ���߿� ���ο� Ŭ�������� ��� ����� ����)
	//			Cluster[s] += image[i][j];

	//			//�ش� �ȼ��� �Ÿ��� ���� ����� Ŭ������ ��ȣ�� ����.
	//			label[i][j] = s;

	//			//���ο� ���(�߽�) ����� ���� ���� ī��Ʈ
	//			Count[s]++;
	//		}
	//	}//end of for

	//	for (k = 0; k < K; k++)
	//	{
	//		if (Count[k] == 0) C_mean[k] = 0.0;
	//		else
	//		{
	//			// �� ������ ��� ���ϱ�
	//			C_mean[k] = (float)(Cluster[k] / Count[k]);
	//		}
	//	}

	//	for (k = 0; k < K; k++) ME[k] = (int)C_mean[k];
	//}



	//�޸� ����
	for (i = 0; i < h; i++)
	{
		delete[] label[i];
	}
	delete[] label;
	delete[] C_mean;
	delete[] distance;
	delete[] Count;
	delete[] Cluster;
}

void CTest2View::OnLbgclustering()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	BYTE** image = new BYTE*[h];

	for (i = 0; i < h; i++){
		image[i] = new BYTE[w];
	}

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			image[j][i] = (ptr[j][i].r + ptr[j][i].g + ptr[j][i].b) / 3.0f; // �׷��� �� ����
		}

	int K = 10, T = 10; // K = ������ ����. T = �Ӱ� ��. �Ӱ� ���� ���� ��� ��.
	int *ME = new int[K]; // ���� �߽ɰ� ���� �޸�

	//�������� ������ �����
	memset(ME, 0, sizeof(int)*K); // �ʱ� ������ 0���� �ʱ�ȭ

	// �ʱ� ���� 2�� ���� ��.
	srand((unsigned)time(NULL));
	ME[0] = rand() % (255);
	ME[1] = rand() % (255);

	float* C_mean = new float[K]; //�� ������ �߽� �� ��� �޸�
	float* distance = new float[K]; // �� �������� �߽� �Ÿ� ��
	int* Count = new int[K]; // �� ������ ���� ����
	int* Cluster = new int[K]; // �� ���� ���� �ȼ��� �� ���� ���� �޸�
	int k, s;
	float* sum_distance = new float[K]; // �� ������ �߽ɿ����� �Ÿ� ��.

	//���� ��ȣ�� ����� �޸�
	int** label = new int*[h];
	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	float min, max, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) // �ʱ�ȭ
	{
		C_mean[k] = distance[k] = sum_distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}

	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	for (k = 0; k < K; k++) // �ʱ�ȭ
	{
		distance[k] = sum_distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}
	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //���� ���� ����

	Sum_mean = min = max = temp_mean = 0.0f;

	float distort = 0.0; // �� ū �ְ��� �ִ� ���� ����.
	int temp_k = 0;
	int im_min = 0, im_max = 255;
	int iter = 2;
	int *temp_ME = new int[2];
	memset(temp_ME, 0, sizeof(int) * 2);

	for (iter = 1; iter < K - 1; iter++)
	{
		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				//�� �ȼ��� �� ���� �߽� ������� �Ÿ��� ���ϱ�
				for (k = 0; k < K; k++)
					if (im_min <= image[i][j] && image[i][j] < im_max)
						distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float�� ���밪.
				min = distance[0];
				s = 0;

				//�Ÿ����� ���� ���� ��հ��� ã�´�
				for (k = 1; k < iter; k++)
				{
					if (distance[k] <= min)
					{
						min = distance[k];
						s = k;
					}
				}
				//�ش� �ȼ����� �ջ�( ���߿� ���ο� Ŭ�������� ��� ����� ����)
				Cluster[s] += image[i][j];

				//�ش� �ȼ��� �Ÿ��� ���� ����� Ŭ������ ��ȣ�� ����.
				label[i][j] = s;

				//���ο� ���(�߽�) ����� ���� ���� ī��Ʈ
				Count[s]++;

				sum_distance[s] += distance[s]; // �Ÿ� ��.
			}
		}//end of for
		for (k = 0; k < iter; k++)
			sum_distance[k] = sum_distance[k] / Count[k]; // ��� �Ÿ��� ���Ѵ�.

		distort = sum_distance[0]; // �ʱ� ���� ����
		for (k = 1; k < iter; k++)
		{
			if (distort < sum_distance[k])
			{
				distort = sum_distance[k]; // ���� �ְ��� ū ����.
				temp_k = k;
			}
		}

		sum_distance[temp_k] = 0;
		im_max = 0;
		im_min = 255;

		for (i = 0; i < h; i++)
		{
			for (j = 0; j < w; j++)
			{
				if (label[i][j] == temp_k)
				{
					if (image[i][j] < im_min)
						im_min = image[i][j]; // �ְ��� ���� ū ������ �ּ� �ȼ� ��
					

					if (im_max < image[i][j])
						im_max = image[i][j]; // �ְ��� ���� ū ������ �ִ� �ȼ� ��

					
				}
			}
		}

		Cluster[temp_k] = 0;
		Count[temp_k] = 0;

		srand((unsigned)time(NULL));
		temp_ME[0] = (rand() % (im_max - im_min)) + im_min;
		temp_ME[1] = (rand() % (im_max - im_min)) + im_min;

		//���� �Լ�ȣ��
		K_Mean_Clustering(image, temp_ME, 2, T, w, h, im_min, im_max);

		ME[temp_k] = temp_ME[0];
		ME[iter + 1] = temp_ME[1];

		for (int f = 0; f <= iter + 1; f++){
			for (int e = 0; e <= iter + 1; e++){
				if (ME[f] == ME[e + 1]){
					if (f != e + 1)
						iter--; break;
				} // ������ ������ �ݺ�Ƚ���� -1�ϰ� �ٽ� �ݺ���Ų��.
			}
		}
	}

	K_Mean_Clustering(image, ME, K, T, w, h); //������ �� ���ϰ� �� �ڿ� K-Mean Ŭ�����͸��� �Ѵ�.

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			ptr[j][i].r = image[j][i];
			ptr[j][i].g = image[j][i];
			ptr[j][i].b = image[j][i];
		}

	//�޸� ����
	for (i = 0; i < h; i++)
	{
		delete[] label[i];
	}
	delete[] label;
	delete[] C_mean;
	delete[] distance;
	delete[] Count;
	delete[] Cluster;
	delete[] image;
	delete[] ME;
	Invalidate();
}

int face_r[256]; // �� R�� 
int face_g[256]; // �� G�� 
int face_b[256]; // �� B�� 
int Nface_r[256]; // ��� R��
int Nface_g[256]; // ��� G��
int Nface_b[256]; // ��� B��

int face_count = 0; // �� �ȼ� ����
int Nface_count = 0; // ��� �ȼ� ����

void CTest2View::OnInputFace()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++){
			if (ptr[i][j].r != 255 && ptr[i][j].g != 255 && ptr[i][j].b != 255){ // �����Ұ� �ϳ��� ���� ���.
				//face_r,g,b�� ���� �����̱� ������ ������ ���� �� ���� �ش� ��ġ �ȼ� ���� �����Ѵ�.
				face_r[ptr[i][j].r]++; //�ش� ��ġ�� R�� ����, 
				face_g[ptr[i][j].g]++; //�ش� ��ġ�� G�� ����
				face_b[ptr[i][j].b]++; //�ش� ��ġ�� B�� ����
				
				face_count++; // �� �ȼ� �� ī��Ʈ
			}
		}
}

void CTest2View::OnInputBack()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++){
			if (ptr[i][j].r != 255 && ptr[i][j].g != 255 && ptr[i][j].b != 255){
				//face_r, g, b�� ���� �����̱� ������ ������ ���� �� ���� �ش� ��ġ �� ���� �����Ѵ�.
				Nface_r[ptr[i][j].r]++; //�ش� ��ġ�� R�� ����
				Nface_g[ptr[i][j].g]++; //�ش� ��ġ�� G�� ����
				Nface_b[ptr[i][j].b]++; //�ش� ��ġ�� B�� ����

				Nface_count++; // ��� �ȼ� ���� ī��Ʈ
			}
		}
}

void CTest2View::OnMl()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	float fr, fg, fb, Nfr, Nfg, Nfb; // ���ε� RGB�� Ȯ��, �ƴѵ� RGB�� Ȯ��.
	float pro_face, pro_Nface; // �� Ȯ��, �ƴ� Ȯ��
	fr = fg = fb = Nfr = Nfg = Nfb = pro_face = pro_Nface = 0.0;

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			fr = (float)face_r[ptr[i][j].r] / (float)face_count; // ���ε� R���� Ȯ��
			fg = (float)face_g[ptr[i][j].g] / (float)face_count; // ���ε� G���� Ȯ��
			fb = (float)face_b[ptr[i][j].b] / (float)face_count; // ���ε� B���� Ȯ��

			Nfr = (float)Nface_r[ptr[i][j].r] / (float)Nface_count; // ����ε� R���� Ȯ��
			Nfg = (float)Nface_g[ptr[i][j].g] / (float)Nface_count; // ����ε� G���� Ȯ��
			Nfb = (float)Nface_b[ptr[i][j].b] / (float)Nface_count; // ����ε� B���� Ȯ��

			// ������ �߷� ��. ML�̿�. ���� Ȯ���� �ƴ�Ȯ���� ���ٰ� ����.
			pro_face = fr*fg*fb / (fr*fg*fb + Nfr*Nfg*Nfb); 
			pro_Nface = Nfr*Nfg*Nfb/ (fr*fg*fb + Nfr*Nfg*Nfb);

			//���� Ȯ���� �� ũ�ٸ� �ش� ��ġ �ȼ��� r,g,b���� 255�� ����.
			//�׷��� �ʴٸ� 0���� ����
			(pro_face > pro_Nface) ? ptr2[i][j].r = ptr2[i][j].g = ptr2[i][j].b = 255 : ptr2[i][j].r = ptr2[i][j].g = ptr2[i][j].b = 0;
		}
	}

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++){
			ptr[i][j].r = (BYTE)limit(ptr2[i][j].r);
			ptr[i][j].g = (BYTE)limit(ptr2[i][j].g);
			ptr[i][j].b = (BYTE)limit(ptr2[i][j].b);
		}
	Invalidate();
}


void CTest2View::OnMap()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	float fr, fg, fb, Nfr, Nfg, Nfb; // ���ε� RGB�� Ȯ��, �ƴѵ� RGB�� Ȯ��.
	float pro_face, pro_Nface; // �� Ȯ��, �ƴ� Ȯ��
	fr = fg = fb = Nfr = Nfg = Nfb = pro_face = pro_Nface = 0.0;

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			fr = (float)face_r[ptr[i][j].r] / (float)face_count; // ���ε� R���� Ȯ��
			fg = (float)face_g[ptr[i][j].g] / (float)face_count; // ���ε� G���� Ȯ��
			fb = (float)face_b[ptr[i][j].b] / (float)face_count; // ���ε� B���� Ȯ��

			Nfr = (float)Nface_r[ptr[i][j].r] / (float)Nface_count; // ����ε� R���� Ȯ��
			Nfg = (float)Nface_g[ptr[i][j].g] / (float)Nface_count; // ����ε� G���� Ȯ��
			Nfb = (float)Nface_b[ptr[i][j].b] / (float)Nface_count; // ����ε� B���� Ȯ��

			// ������ �߷� ��. MAP�̿�. ���� Ȯ���� �ƴ�Ȯ���� ���ٰ� ����.
			pro_face = fr * fg * fb * 0.5 / (fr * fg * fb * 0.5 + Nfr * Nfg * Nfb * 0.5); 
			pro_Nface = Nfr * Nfg * Nfb * 0.5 / (fr * fg * fb * 0.5 + Nfr * Nfg * Nfb * 0.5);

			//���� Ȯ���� �� ũ�ٸ� �ش� ��ġ �ȼ��� r,g,b���� 255�� ����.
			//�׷��� �ʴٸ� 0���� ����
			(pro_face > pro_Nface) ? ptr2[i][j].r = ptr2[i][j].g = ptr2[i][j].b = 255 : ptr2[i][j].r = ptr2[i][j].g = ptr2[i][j].b = 0;
		}
	}

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++){
			ptr[i][j].r = (BYTE)limit(ptr2[i][j].r);
			ptr[i][j].g = (BYTE)limit(ptr2[i][j].g);
			ptr[i][j].b = (BYTE)limit(ptr2[i][j].b);
		}
	Invalidate();
}

//������� ���� �ڵ�
void CTest2View::OnFuzzyHistoR() // ���� R���� ������ ������׷�
{
	//������׷� ���
	int i;
	float histo[256] = { 0.f, };
	
	float area = (float)(face_count + Nface_count); //�Էµ� ���� ��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = face_r[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistoG() // ���� G���� ������ ������׷�
{
	//������׷� ���
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //�Էµ� ���� ��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = face_g[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistob() // ���� B���� ������ ������׷�
{
	//������׷� ���
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //�Էµ� ���� ��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = face_b[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistogramRBack() // ����� R���� ������ ������׷�
{
	//������׷� ���
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //�Էµ� ���� ��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = Nface_r[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistogramGBack()// ����� G���� ������ ������׷�
{
	//������׷� ���
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //�Էµ� ���� ��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = Nface_g[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistogramBBack()// ����� B���� ������ ������׷�
{
	//������׷� ���
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //�Էµ� ���� ��ü �ȼ�
	for (i = 0; i < 256; i++)
		histo[i] = Nface_b[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

float R_HIGH(int pixel_r) // ��ȯ�� Ȯ����. �Է��� ���� R�� pixel��.
{
	float result = 0.0;
	int max = 250, min = 150;

	if (pixel_r < min)
		return result = 0;

	else if (min <= pixel_r && pixel_r <= max)
	{
		result = (pixel_r - min) / (max - min);
		return result;
	}

	else
	{
		result = (255 - pixel_r) / (255 - max);
		return result;
	}

}

float R_LOW(int pixel_r) // R���� LOW�϶�
{
	float result = 0.0;
	int max = 190, min = 160;
	if (pixel_r < min)
		return result = 1;

	else if (min <= pixel_r && pixel_r <= max){
		result = (max - pixel_r) / (max - min);
		return result;
	}

	else
		return result = 0;
}

float G_HIGH(int pixel_g) // ��ȯ�� Ȯ����. �Է��� ���� G�� pixel��.
{
	float result = 0.0;
	int max = 240, min = 160;
	if (pixel_g < min)
		return result = 0;

	else if (min <= pixel_g && pixel_g <= max)
	{
		result = (pixel_g - min) / (max - min);
		return result;
	}

	else
	{
		result = (255 - pixel_g) / (255 - max);
		return result;
	}
}

float G_LOW(int pixel_g) // G���� LOW�϶�
{
	float result = 0.0;
	int max = 190, min = 170;
	if (pixel_g < min)
		return result = 1;

	else if (min <= pixel_g && pixel_g <= max){
		result = (max - pixel_g) / (max - min);
		return result;
	}

	else
		return result = 0;
}

float B_HIGH(int pixel_b) // ��ȯ�� Ȯ����. �Է��� ���� B�� pixel��.
{
	float result = 0.0;
	int max = 230, min = 150;
	if (pixel_b < max)
		return 0;

	else if (min <= pixel_b && pixel_b <= max)
	{
		result = (pixel_b - min) / (max - min);
		return result;
	}

	else
	{
		result = (255 - pixel_b) / (255 - max);
		return result;
	}
}

float B_LOW(int pixel_b) // B���� LOW�϶�
{
	float result = 0.0;
	int max = 160, min = 100;
	if (pixel_b < min)
		return result = 1;

	else if (min <= pixel_b && pixel_b <= max){
		result = (max - pixel_b) / (max - min);
		return result;
	}

	else
		return result = 0;
}

float FACE_PROB(int prob) // ���� Ȯ��
{
	float result = 0.0;
	if (prob < 40)
		return result = 0.0;
	else if (40 <= prob && prob <= 60){
		result = (float)(prob - 40) / 20;
			return result;
	}
	else
		return result = 1.0;
}

float NFACE_PROB(int prob) // ����� Ȯ��
{
	float result = 0.0;
	if (prob < 40)
		return result = 1;

	else if (40 <= prob && prob <= 60){
		result = (float)(60 - prob) / 20;
		return result;
	}
	else
		return result = 0;
}

float CTest2View::Sort_max(float t[]){ // ����
	int i, k, r,max;
	i = 0;
	while (i == 0){
		i = 1;
		for (k = 0; k < 8; k++)
			if (t[k]>t[k + 1]){
				i = 0; r = t[k + 1]; t[k + 1] = t[k]; t[k] = r;
			}
	}
	max = t[7];
	return max;
}

void CTest2View::OnFuzzyLarsen()
{
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	int k, z;

	float res_F_NF[101]; // �׷����� ��ģ ���
	int centroid=0; // �����߽�
	float temp_res[9] = { 0 ,}; // ��� �� �ӽ�����


	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	float sum_bottom = 0, sum_top = 0; // ���ڿ� �и��� ��

	for (z = 0; z < h; z++){
		for (k = 0; k < w; k++){
			sum_bottom = 0, sum_top = 0;
			for (j = 0; j < 101; j++){
				temp_res[0] = R_HIGH(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * FACE_PROB(j); // �� HIGH�� ��� ���̽�
				temp_res[1] = R_HIGH(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j); // ������� ��� N���̽�
				temp_res[2] = R_HIGH(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * FACE_PROB(j);
				temp_res[3] = R_HIGH(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j);  // ������ ���̽�
				temp_res[4] = R_LOW(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * FACE_PROB(j);
				temp_res[5] = R_LOW(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * NFACE_PROB(j); // LOW 2�� �̻� N ���̽�
				temp_res[6] = R_LOW(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j); // LOW 2�� �̻� N ���̽�
				temp_res[7] = R_LOW(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j); // LOW 2�� �̻� N ���̽�

				res_F_NF[j] = Sort_max(temp_res); // �� ���� �׷����� MAX
				sum_top += (res_F_NF[j] * j);
				sum_bottom += res_F_NF[j];
			}

			centroid = sum_top / sum_bottom;

			if (res_F_NF[centroid] < 0.5) ptr2[z][k].r = ptr2[z][k].g = ptr2[z][k].b = 255;
			else  ptr2[z][k].r = ptr2[z][k].g = ptr2[z][k].b = 0;
		}
	}

	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++){
			ptr[i][j].r = (BYTE)limit(ptr2[i][j].r);
			ptr[i][j].g = (BYTE)limit(ptr2[i][j].g);
			ptr[i][j].b = (BYTE)limit(ptr2[i][j].b);
		}
	Invalidate();
}

#define MAX_LABEL 10000 // ������ �ִ� ���� 10000��.
void CTest2View::OnLabling()
{
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	int nConnNumThre = 100; // �������� ���ϸ� ������ ���̺��� �����ϱ����� �Ӱ谪

	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()) {
		w = pDoc->m_Dib.GetWidth(); // ������ ����
		h = pDoc->m_Dib.GetHeight(); // ������ ����
		ptr = pDoc->m_Dib.GetRGBPtr(); // �ּҰ� �Ҵ�
	}
	int** Label = new int*[h];
	for (i = 0; i < h; i++) {
		Label[i] = new int[w];
		memset(Label[i], 0, sizeof(int)*w);
	}

	CDib Dib1;
	Dib1.CreateRGBImage(w, h, RGB(0, 0, 0));
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	BYTE rr, gg, bb;
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			rr = ptr[i][j].r;
			gg = ptr[i][j].g;
			bb = ptr[i][j].b;

			ptr2[i][j] = (BYTE)limit(RGB2GRAY(rr, gg, bb));
		}
	}

	int x, y, num, left, top, k;
	int *r, *area;

	r = new int[h*w];
	area = new int[h*w];
	// ���� ����ȭ
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (ptr2[y][x].r < 128) Label[y][x] = -1; // ���ʿ���� �ֵ�
			else Label[y][x] = 0;
		}
	}


	r = new int[w*h];
	area = new int[w*h];

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (ptr2[y][x].r < 128) Label[y][x] = 0;
			else Label[y][x] = -1;
		}
	}

	for (x = 0; x < w; x++) {
		Label[0][x] = -1;
		Label[h - 1][x] = -1;
	}
	for (y = 0; y < h; y++) {
		Label[y][0] = -1;
		Label[y][w - 1] - 1;
	}

	num = -1;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (y>0 && x > 0) { // ��ĵ�Ͽ� 0���� ū ���� ������
				if (Label[y][x] >= 0) {
					left = Label[y][x - 1]; // ����üũ
					top = Label[y - 1][x]; // ����üũ
					if (left == -1 && top != -1) {
						Label[y][x] = r[top];
					}// ���ʿ� ���� �ִٸ� ž������
					else if (left != -1 && top == -1) { // ���ʿ� ���� �ִٸ� ���ʰ� ����
						Label[y][x] = r[left];
					}
					else if (left == -1 && top == -1) {
						num++;
						if (num >= w*h) {
							delete[] r;
							delete[] area;
						}

						r[num] = num;
						Label[y][x] = r[num];
					}
					else if (left != -1 && top != -1) { // �Ѵ� ���� ������.. 
						if (r[left] == r[top]) { // �Ѵ� ���ٸ� ����
							Label[y][x] = r[left];
						}
						else if (r[left] > r[top]) { // ���ʰ��� ũ�ٸ� ���� ������.. ( ���� ������ ��ü )
							Label[y][x] = r[top];
							r[left] = r[top];
						}
						else { // ���ʰ��� ũ�ٸ� ���ʰ�����.. (���� ������ ��ü)
							Label[y][x] = r[left];
							r[top] = r[left];
						}
					}

				}

			}
		}
	}

	for (k = 0; k <= num; k++) {
		if (k != r[k]) r[k] = r[r[k]];
		area[k] = 0;
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (Label[y][x] > -1) {
				Label[y][x] = r[Label[y][x]];
				area[Label[y][x]]++;
			}
		}
	}

	int cnt = 0;
	for (k = 0; k <= num; k++) {
		if (area[k] > nConnNumThre) r[k] = cnt++;
		else r[k] = -1;
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (Label[y][x] >= 0) {
				Label[y][x] = r[Label[y][x]];
			}
		}
	}

	delete[] r;
	delete[] area;

	for (i = 0; i < h; i++) {
		delete[] Label[i];
	}
	delete[] Label;

	CString str;

	str.Format(_T("���̺� ���� = %d"), cnt + 1);
	AfxMessageBox(str);
}

void DirectoryFind(char* filename, CDib& dib) {
	_finddata_t fd;
	long handle;
	int result = 1;
	//handle = _findfirst(".\\*.*", &fd);  //���� ���� �� ��� ������ ã�´�.     
	handle = _findfirst(filename, &fd);
	if (handle == -1)     {
		printf("There were no files.\n");
		return;
	}

	while (result != -1)     {
		printf("File: %s\n", fd.name);
		//fd.name;
		result = _findnext(handle, &fd);
	}

	_findclose(handle);
}

void CTest2View::OnTrain() // �н������� �н�. �Ժη� ����������;;;
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	BYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetPtr();
	}

	CDib Dib1;		//�Է� ���� �޾ƿ���
	Dib1.CreateGrayImage(w, h, 0);		//gray �̹����� �����
	BYTE** ptr2 = Dib1.GetPtr();		//byte ���� ������ ptr2 �����

	//������ ����ȭ
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			ptr2[i][j] = (ptr[i][j] > 128) ? 0 : 255;
		}
	}

	//���� ���
	ofstream outfile("test.txt", ios::app);
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			ptr2[i][j] = (ptr2[i][j] == 255) ? 0 : 1;

			outfile << (int)ptr2[i][j] << " ";
		}
	}
	outfile << 9 << endl;
	outfile.close();
}

void CTest2View::OnNcBinary() // ���� ����ȭ. �׽�Ʈ �� �����͸� ����ȭ�Ѵ�.
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	register int i, j, w, h;
	BYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetPtr();
	}

	CDib Dib1;
	Dib1.CreateGrayImage(w, h, 0);
	BYTE** ptr2 = Dib1.GetPtr();

	//������ ����ȭ
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			ptr2[i][j] = (ptr[i][j] > 128) ? 0 : 255;
		}
	}

	ofstream outfile("test_dat.txt");
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			ptr2[i][j] = (ptr2[i][j] == 255) ? 0 : 1;

			outfile << (int)ptr2[i][j] << " ";
		}
	}
	outfile.close();
}


void CTest2View::OnTest() // �׽�Ʈ. MLP�� �־��.
{
	int layers1[] = { 729, 15, 10 }; //input layer, hidden layer, output layer
	MultiLayerPerceptron mlp1(3, layers1); //3= number of layer
	mlp1.Actual_Test("test_dat.txt");
}
