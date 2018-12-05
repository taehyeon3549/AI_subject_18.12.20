
// Test2View.cpp : CTest2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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
#define RGB2GRAY(r,g,b)(0.299*(r) + 0.587*(g)+ 0.114*(b)) // 정밀한 gray값.
// CTest2View

using namespace std;

IMPLEMENT_DYNCREATE(CTest2View, CScrollView)

BEGIN_MESSAGE_MAP(CTest2View, CScrollView)
	// 표준 인쇄 명령입니다.
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

// CTest2View 생성/소멸
CTest2View::CTest2View()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CTest2View::~CTest2View()
{
}

BOOL CTest2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CTest2View 그리기

void CTest2View::OnDraw(CDC* pDC)
{
	CTest2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->m_Dib.IsValid())
		pDoc->m_Dib.Draw(pDC->m_hDC);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
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

	// TODO: 이 뷰의 전체 크기를 계산합니다.
	//sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	ResizeParentToFit(true); // false로 하면 고정크기
}


// CTest2View 인쇄

BOOL CTest2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CTest2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CTest2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CTest2View 진단

#ifdef _DEBUG
void CTest2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTest2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTest2Doc* CTest2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTest2Doc)));
	return (CTest2Doc*)m_pDocument;
}
#endif //_DEBUG


// CTest2View 메시지 처리기


void CTest2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDib dib;
	dib.Load(_T("lenna.bmp"));

	CClientDC dc(this);
	dib.Draw(dc.m_hDC, point.x, point.y);

	CScrollView::OnLButtonDown(nFlags, point);
}


BOOL CTest2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CBrush br;
	br.CreateHatchBrush(HS_DIAGCROSS, RGB(200, 200, 200));
	FillOutsideRect(pDC, &br);

	return TRUE;
}


void CTest2View::OnDibinverse() // 색반전
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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


void CTest2View::OnDibcontrast() // 대비 증가
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	register int i, j, w, h, n = -40; //n = 그레이 스케일 값
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
	Invalidate(); // 적용값 바로 나오게
}


void CTest2View::OnCorrection() // 감마보정
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	register int i, j, w, h;
	float value;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	float gamma = 2.5f; // LCD 모니터경우 대략 2.5
	float invgamma = 1.f / gamma; //gamma의 역수

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3.0f;
			ptr[i][j].r = ptr[i][j].g = ptr[i][j].b =
				(BYTE)limit(pow(value / 255.f, invgamma) * 255 + 0.5f); //pow쓸려면 math.h include
		}
	}
	Invalidate(); // 적용값 바로 나오게
}

void CTest2View::OnHistogram() // 히스토그램
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	register int i, j, w, h;
	BYTE value;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	//히스토그램 계산
	int temp[256];
	float histo[256] = { 0.f, };
	memset(temp, 0, sizeof(int) * 256);
	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3.0f;
			temp[value]++;
		}
	}

	//히스토그램 정규화, 그레이 스케일값을 가지는 픽셀이 나타날 확률
	float area = (float)w*h; //전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = temp[i] / area;

	CDia_Histogram dig;
	dig.SetImage(histo);
	dig.DoModal();
}


void CTest2View::OnEqual() // 히스토그램 평활화
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	register int i, j, w, h;
	BYTE value;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	//히스토그램 계산
	int temp[256];
	float histo[256] = { 0.f, };
	memset(temp, 0, sizeof(int) * 256);

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3.0f;
			temp[value]++;
		}
	}

	//히스토그램 정규화
	float area = (float)w*h; //전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = temp[i] / area;

	//히스토그램 누적 함수 계산
	double cdf[256] = { 0.0, };
	cdf[0] = histo[0]; // histo[0]는 확률분포. 0~1사이의 실수.
	for (i = 1; i < 256; i++)
		cdf[i] = cdf[i - 1] + histo[i]; // 계속 누적.

	//히스토그램 균등화
	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			value = (ptr[i][j].r + ptr[i][j].g + ptr[i][j].b) / 3;
			ptr[i][j].r = ptr[i][j].g = ptr[i][j].b = (BYTE)limit(cdf[value] * 255); // rgb값으로 바꿔주기 위해
		}
	}
	Invalidate();
}

void CTest2View::Onmedianfilter() // 중간자 필터
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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

	int NW = 3, MW = 3, NW2, MW2; // NW * MW = 필터 크기
	int *tr = new int[NW*MW];
	memset(tr, 0, sizeof(int)*NW*MW);
	int *tg = new int[NW*MW];
	memset(tg, 0, sizeof(int)*NW*MW);
	int *tb = new int[NW*MW];
	memset(tb, 0, sizeof(int)*NW*MW);

	NW2 = NW / 2;
	MW2 = MW / 2;
	for (k = 0 + NW2; k < h - NW2; k++) // 필터를 y축으로 밑으로 이동.
		for (l = 0 + MW2; l < w - MW2; l++){ // 필터를 x축으로 밑으로 이동
			for (i = 0; i < NW; i++) // 필터 내부의 픽셀들 계산. y축으로 이동
				for (j = 0; j < MW; j++){ // 필터내부 픽셀 계산. x축으로 이동
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

void CTest2View::Sort(int * t, int NW){ // 정렬
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


void CTest2View::OnEdgeroberts() // 엣지 로버트
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
	int h1, h2; // h1 = x축 미분, h2 = y축 미분. 90도 회전
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


void CTest2View::OnEdgesovel() // 테두리만 표시
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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




void CTest2View::OnTranslation() //이동
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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


void CTest2View::OnResizeNearest() // 크기조정
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetRGBPtr();
	}

	CDib Dib1;
	double nw = w *10, nh = h * 10; // 크기 작게 해보기? w*0.5, h*0.5
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

void CTest2View::Onmeanfilter2() // 평균값 필터
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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



void CTest2View::OnWMeanfilter() // 가중치 필터 (2 / 4 / 2)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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


void CTest2View::OnUnsharp() // 언샤프 마스크, Mean-Filter
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
			ptr2[j][i].r = (BYTE)limit(temp / 9. + 0.5); // 0.5를 더하는것은 실수를 정수로 만들기 위함이다.

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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
			salt = rand() % 255; // 소금의 값을 0~255사이값으로 지정
			pepper = rand() % 255; // 후추의 값을 0~255 사이값으로 지정
			if (salt > 250){ // 생각보다 많이 찍힘
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CTest2Doc* pDoc = GetDocument();
	Dia_Meanfilter dlg;
	dlg.m_Doc = pDoc;
	dlg.DoModal(); // 다이어로그 창 띄움

	Invalidate();
}


void CTest2View::OnUnsharpWm()// 언샤프마스크, WEIGHTED-MEAN-FILTER
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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


void CTest2View::OnUnsharpMef() //언샤프마스크, Median-Filter
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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

	//회전 수행
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

	int K = 10, T = 1; // K = 군집의 갯수. T = 임계 값.
	int *ME = new int[K]; // 군집 중심값 저장 메모리

	//초기 군집 중심값을 위한 난수 발생.
	srand((unsigned)time(NULL));
	for (i = 0; i < K; i++) ME[i] = rand() % (255);

	//군집 함수 호출
	K_Mean_Clustering(image, ME, K, T, w, h);

	//영상 출력 부분
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
	float* C_mean = new float[K]; //각 군집의 중심 값 기억 메모리
	float* distance = new float[K]; // 각 군집간의 중심 거리 차
	int* Count = new int[K]; // 각 군집의 원소 개수
	int* Cluster = new int[K]; // 각 군집 원소 픽셀의 총 합을 위한 메모리
	int i, j, k, s;

	//군집 번호를 기억할 메모리
	int** label = new int*[h];
	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	float min, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) // 초기화
	{
		C_mean[k] = distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}

	Sum_mean = min = temp_mean = 0.0f;

	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //초기 평균 값을 넣음
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			//한 픽셀과 각 군집 중심 값들과의 거리를 구하기
			for (k = 0; k < K; k++) distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float형 절대값.
			min = distance[0];
			s = 0;

			//거리값이 가장 작은 평균값을 찾는다
			for (k = 0; k < K; k++)
			{
				if (distance[k] <= min)
				{
					min = distance[k];
					s = k;
				}
			}
			//해당 픽셀값을 합산( 나중에 새로운 클러스터의 평균 계산을 위해)
			Cluster[s] += image[i][j];

			//해당 픽셀을 거리가 가장 가까운 클러스터 번호로 변경.
			label[i][j] = s;

			//새로운 평균(중심) 계산을 위한 개수 카운트
			Count[s]++;
		}
	}//end of for

	for (k = 0; k < K; k++)
	{
		temp_mean = C_mean[k];
		if (Count[k] == 0) C_mean[k] = 0.0;
		else
		{
			// 새 군집의 평균 구하기
			C_mean[k] = (float)(Cluster[k] / Count[k]);
		}

		//각 군집의 중심값과 이전 중심값과의 차이값 구하기.
		Sum_mean += (float)fabs(C_mean[k] - temp_mean);
		temp_mean = 0.0;
	}

	if (Sum_mean <= T) // 이전 중심 값의 합과 현재 중심 값의 합의 수렴 여부
	{
		for (k = 0; k < K; k++)//각 군집을 새로운 중심값으로 채워 줌
		{
			for (i = 0; i < h; i++)
				for (j = 0; j < w; j++)
				{
					if (label[i][j] == k)image[i][j] = (unsigned char)C_mean[k];
				}
		}
		return; // 차이가 거의 없다면
	}

	else  //차이가 크다면 다시 클러스터 작업
	{
		Sum_mean = 0.0;
		for (k = 0; k < K; k++)ME[k] = (int)C_mean[k];
		K_Mean_Clustering(image, ME, K, T, w, h);
	}//원래 재귀 문

	////반복으로 바꿈
	//for (int iter = 0; iter < T; iter++)
	//{
	//	//for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //초기 평균 값을 넣음
	//	for (i = 0; i < h; i++)
	//	{
	//		for (j = 0; j < w; j++)
	//		{
	//			//한 픽셀과 각 군집 중심 값들과의 거리를 구하기
	//			for (k = 0; k < K; k++) distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float형 절대값.

	//			min = distance[0];
	//			s = 0;

	//			//거리값이 가장 작은 평균값을 찾는다
	//			for (k = 0; k < K; k++)
	//			{
	//				if (distance[k] <= min)
	//				{
	//					min = distance[k];
	//					s = k;
	//				}
	//			}
	//			//해당 픽셀값을 합산( 나중에 새로운 클러스터의 평균 계산을 위해)
	//			Cluster[s] += image[i][j];

	//			//해당 픽셀을 거리가 가장 가까운 클러스터 번호로 변경.
	//			label[i][j] = s;

	//			//새로운 평균(중심) 계산을 위한 개수 카운트
	//			Count[s]++;
	//		}
	//	}//end of for

	//	for (k = 0; k < K; k++)
	//	{
	//		if (Count[k] == 0) C_mean[k] = 0.0;
	//		else
	//		{
	//			// 새 군집의 평균 구하기
	//			C_mean[k] = (float)(Cluster[k] / Count[k]);
	//		}
	//	}

	//	for (k = 0; k < K; k++) ME[k] = (int)C_mean[k];
	//	for (k = 0; k < K; k++)//각 군집을 새로운 중심값으로 채워 줌
	//			{
	//				for (i = 0; i < h; i++)
	//					for (j = 0; j < w; j++)
	//					{
	//						if (label[i][j] == k)image[i][j] = (unsigned char)C_mean[k];
	//					}
	//			}
	//}

	//메모리 해제
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
	float* C_mean = new float[K]; //각 군집의 중심 값 기억 메모리
	float* distance = new float[K]; // 각 군집간의 중심 거리 차
	int* Count = new int[K]; // 각 군집의 원소 개수
	int* Cluster = new int[K]; // 각 군집 원소 픽셀의 총 합을 위한 메모리
	int i, j, k, s;

	//군집 번호를 기억할 메모리
	int** label = new int*[h];
	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	float min, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) // 초기화
	{
		C_mean[k] = distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}

	Sum_mean = min = temp_mean = 0.0f;

	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //초기 평균 값을 넣음
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			//한 픽셀과 각 군집 중심 값들과의 거리를 구하기
			for (k = 0; k < K; k++)
			{
				if (first <= image[i][j] && end >= image[i][j]) // 왜곡이 큰 구간에 있다면
					distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float형 절대값.
			}

			min = distance[0];
			s = 0;

			//거리값이 가장 작은 평균값을 찾는다
			for (k = 1; k < K; k++)
			{
				if (distance[k] <= min)
				{
					min = distance[k];
					s = k;
				}
			}
			//해당 픽셀값을 합산( 나중에 새로운 클러스터의 평균 계산을 위해)
			Cluster[s] += image[i][j];

			//해당 픽셀을 거리가 가장 가까운 클러스터 번호로 변경.
			label[i][j] = s;

			//새로운 평균(중심) 계산을 위한 개수 카운트
			Count[s]++;
		}
	}//end of for

	for (k = 0; k < K; k++)
	{
		temp_mean = C_mean[k];
		if (Count[k] == 0) C_mean[k] = 0.0;
		else
		{
			// 새 군집의 평균 구하기
			C_mean[k] = (float)(Cluster[k] / Count[k]);
		}

		//각 군집의 중심값과 이전 중심값과의 차이값 구하기.
		Sum_mean += (float)fabs(C_mean[k] - temp_mean);
		temp_mean = 0.0;
	}

	if (Sum_mean < T) // 이전 중심 값의 합과 현재 중심 값의 합의 수렴 여부
	{
		return; // 차이가 거의 없다면
	}

	else  //차이가 크다면 다시 클러스터 작업
	{
		Sum_mean = 0.0;
		for (k = 0; k < K; k++)ME[k] = (int)C_mean[k];
		K_Mean_Clustering(image, ME, K, T, w, h, first, end);
	} 
	 //원래 재귀문


	////반복으로 바꿈
	//for (int iter = 0; iter < T; iter++)
	//{
	//	//for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //초기 평균 값을 넣음
	//	for (i = 0; i < h; i++)
	//	{
	//		for (j = 0; j < w; j++)
	//		{
	//			//한 픽셀과 각 군집 중심 값들과의 거리를 구하기
	//			for (k = 0; k < K; k++)
	//			{
	//				if (first <= image[i][j] && image[i][j] <= end) // 왜곡이 큰 구간에 있다면
	//					distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float형 절대값.
	//			}

	//			min = distance[0];
	//			s = 0;

	//			//거리값이 가장 작은 평균값을 찾는다
	//			for (k = 0; k < K; k++)
	//			{
	//				if (distance[k] <= min)
	//				{
	//					min = distance[k];
	//					s = k;
	//				}
	//			}
	//			//해당 픽셀값을 합산( 나중에 새로운 클러스터의 평균 계산을 위해)
	//			Cluster[s] += image[i][j];

	//			//해당 픽셀을 거리가 가장 가까운 클러스터 번호로 변경.
	//			label[i][j] = s;

	//			//새로운 평균(중심) 계산을 위한 개수 카운트
	//			Count[s]++;
	//		}
	//	}//end of for

	//	for (k = 0; k < K; k++)
	//	{
	//		if (Count[k] == 0) C_mean[k] = 0.0;
	//		else
	//		{
	//			// 새 군집의 평균 구하기
	//			C_mean[k] = (float)(Cluster[k] / Count[k]);
	//		}
	//	}

	//	for (k = 0; k < K; k++) ME[k] = (int)C_mean[k];
	//}



	//메모리 해제
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
			image[j][i] = (ptr[j][i].r + ptr[j][i].g + ptr[j][i].b) / 3.0f; // 그레이 값 저장
		}

	int K = 10, T = 10; // K = 군집의 갯수. T = 임계 값. 임계 값을 조금 줘야 함.
	int *ME = new int[K]; // 군집 중심값 저장 메모리

	//랜덤으로 중점을 찍어줌
	memset(ME, 0, sizeof(int)*K); // 초기 중점을 0으로 초기화

	// 초기 중점 2개 랜덤 값.
	srand((unsigned)time(NULL));
	ME[0] = rand() % (255);
	ME[1] = rand() % (255);

	float* C_mean = new float[K]; //각 군집의 중심 값 기억 메모리
	float* distance = new float[K]; // 각 군집간의 중심 거리 차
	int* Count = new int[K]; // 각 군집의 원소 개수
	int* Cluster = new int[K]; // 각 군집 원소 픽셀의 총 합을 위한 메모리
	int k, s;
	float* sum_distance = new float[K]; // 각 군집의 중심에서의 거리 합.

	//군집 번호를 기억할 메모리
	int** label = new int*[h];
	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	float min, max, temp_mean, Sum_mean;
	for (k = 0; k < K; k++) // 초기화
	{
		C_mean[k] = distance[k] = sum_distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}

	for (i = 0; i < h; i++)
	{
		label[i] = new int[w];
		memset(label[i], 0, sizeof(int)*w);
	}

	for (k = 0; k < K; k++) // 초기화
	{
		distance[k] = sum_distance[k] = 0.0f;
		Count[k] = Cluster[k] = 0;
	}
	for (k = 0; k < K; k++) C_mean[k] = (float)ME[k]; //중점 값을 넣음

	Sum_mean = min = max = temp_mean = 0.0f;

	float distort = 0.0; // 더 큰 왜곡이 있는 중점 저장.
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
				//한 픽셀과 각 군집 중심 값들과의 거리를 구하기
				for (k = 0; k < K; k++)
					if (im_min <= image[i][j] && image[i][j] < im_max)
						distance[k] = (float)fabs(C_mean[k] - (float)image[i][j]); // fabs = float형 절대값.
				min = distance[0];
				s = 0;

				//거리값이 가장 작은 평균값을 찾는다
				for (k = 1; k < iter; k++)
				{
					if (distance[k] <= min)
					{
						min = distance[k];
						s = k;
					}
				}
				//해당 픽셀값을 합산( 나중에 새로운 클러스터의 평균 계산을 위해)
				Cluster[s] += image[i][j];

				//해당 픽셀을 거리가 가장 가까운 클러스터 번호로 변경.
				label[i][j] = s;

				//새로운 평균(중심) 계산을 위한 개수 카운트
				Count[s]++;

				sum_distance[s] += distance[s]; // 거리 합.
			}
		}//end of for
		for (k = 0; k < iter; k++)
			sum_distance[k] = sum_distance[k] / Count[k]; // 평균 거리를 구한다.

		distort = sum_distance[0]; // 초기 중점 저장
		for (k = 1; k < iter; k++)
		{
			if (distort < sum_distance[k])
			{
				distort = sum_distance[k]; // 가장 왜곡이 큰 중점.
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
						im_min = image[i][j]; // 왜곡이 가장 큰 군집의 최소 픽셀 값
					

					if (im_max < image[i][j])
						im_max = image[i][j]; // 왜곡이 가장 큰 군집의 최대 픽셀 값

					
				}
			}
		}

		Cluster[temp_k] = 0;
		Count[temp_k] = 0;

		srand((unsigned)time(NULL));
		temp_ME[0] = (rand() % (im_max - im_min)) + im_min;
		temp_ME[1] = (rand() % (im_max - im_min)) + im_min;

		//군집 함수호출
		K_Mean_Clustering(image, temp_ME, 2, T, w, h, im_min, im_max);

		ME[temp_k] = temp_ME[0];
		ME[iter + 1] = temp_ME[1];

		for (int f = 0; f <= iter + 1; f++){
			for (int e = 0; e <= iter + 1; e++){
				if (ME[f] == ME[e + 1]){
					if (f != e + 1)
						iter--; break;
				} // 중점이 같으면 반복횟수를 -1하고 다시 반복시킨다.
			}
		}
	}

	K_Mean_Clustering(image, ME, K, T, w, h); //중점을 다 정하고 난 뒤에 K-Mean 클러스터링을 한다.

	for (j = 0; j < h; j++)
		for (i = 0; i < w; i++){
			ptr[j][i].r = image[j][i];
			ptr[j][i].g = image[j][i];
			ptr[j][i].b = image[j][i];
		}

	//메모리 해제
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

int face_r[256]; // 얼굴 R값 
int face_g[256]; // 얼굴 G값 
int face_b[256]; // 얼굴 B값 
int Nface_r[256]; // 배경 R값
int Nface_g[256]; // 배경 G값
int Nface_b[256]; // 배경 B값

int face_count = 0; // 얼굴 픽셋 갯수
int Nface_count = 0; // 배경 픽셀 갯수

void CTest2View::OnInputFace()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
			if (ptr[i][j].r != 255 && ptr[i][j].g != 255 && ptr[i][j].b != 255){ // 흰색요소가 하나라도 없을 경우.
				//face_r,g,b가 전역 변수이기 때문에 영상을 넣을 때 마다 해당 위치 픽셀 값은 증가한다.
				face_r[ptr[i][j].r]++; //해당 위치의 R값 증가, 
				face_g[ptr[i][j].g]++; //해당 위치의 G값 증가
				face_b[ptr[i][j].b]++; //해당 위치의 B값 증가
				
				face_count++; // 얼굴 픽셀 수 카운트
			}
		}
}

void CTest2View::OnInputBack()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
				//face_r, g, b가 전역 변수이기 때문에 영상을 넣을 때 마다 해당 위치 얼굴 값은 증가한다.
				Nface_r[ptr[i][j].r]++; //해당 위치의 R값 증가
				Nface_g[ptr[i][j].g]++; //해당 위치의 G값 증가
				Nface_b[ptr[i][j].b]++; //해당 위치의 B값 증가

				Nface_count++; // 배경 픽셀 갯수 카운트
			}
		}
}

void CTest2View::OnMl()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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

	float fr, fg, fb, Nfr, Nfg, Nfb; // 얼굴인데 RGB일 확률, 아닌데 RGB일 확률.
	float pro_face, pro_Nface; // 얼굴 확률, 아닐 확률
	fr = fg = fb = Nfr = Nfg = Nfb = pro_face = pro_Nface = 0.0;

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			fr = (float)face_r[ptr[i][j].r] / (float)face_count; // 얼굴인데 R값일 확률
			fg = (float)face_g[ptr[i][j].g] / (float)face_count; // 얼굴인데 G값일 확률
			fb = (float)face_b[ptr[i][j].b] / (float)face_count; // 얼굴인데 B값일 확률

			Nfr = (float)Nface_r[ptr[i][j].r] / (float)Nface_count; // 배경인데 R값일 확률
			Nfg = (float)Nface_g[ptr[i][j].g] / (float)Nface_count; // 배경인데 G값일 확률
			Nfb = (float)Nface_b[ptr[i][j].b] / (float)Nface_count; // 배경인데 B값일 확률

			// 베이즈 추론 식. ML이용. 얼굴일 확률과 아닐확률이 같다고 생각.
			pro_face = fr*fg*fb / (fr*fg*fb + Nfr*Nfg*Nfb); 
			pro_Nface = Nfr*Nfg*Nfb/ (fr*fg*fb + Nfr*Nfg*Nfb);

			//얼굴일 확률이 더 크다면 해당 위치 픽셀의 r,g,b값을 255로 변경.
			//그렇지 않다면 0으로 변경
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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

	float fr, fg, fb, Nfr, Nfg, Nfb; // 얼굴인데 RGB일 확률, 아닌데 RGB일 확률.
	float pro_face, pro_Nface; // 얼굴 확률, 아닐 확률
	fr = fg = fb = Nfr = Nfg = Nfb = pro_face = pro_Nface = 0.0;

	for (i = 0; i < h; i++){
		for (j = 0; j < w; j++){
			fr = (float)face_r[ptr[i][j].r] / (float)face_count; // 얼굴인데 R값일 확률
			fg = (float)face_g[ptr[i][j].g] / (float)face_count; // 얼굴인데 G값일 확률
			fb = (float)face_b[ptr[i][j].b] / (float)face_count; // 얼굴인데 B값일 확률

			Nfr = (float)Nface_r[ptr[i][j].r] / (float)Nface_count; // 배경인데 R값일 확률
			Nfg = (float)Nface_g[ptr[i][j].g] / (float)Nface_count; // 배경인데 G값일 확률
			Nfb = (float)Nface_b[ptr[i][j].b] / (float)Nface_count; // 배경인데 B값일 확률

			// 베이즈 추론 식. MAP이용. 얼굴일 확률과 아닐확률이 같다고 생각.
			pro_face = fr * fg * fb * 0.5 / (fr * fg * fb * 0.5 + Nfr * Nfg * Nfb * 0.5); 
			pro_Nface = Nfr * Nfg * Nfb * 0.5 / (fr * fg * fb * 0.5 + Nfr * Nfg * Nfb * 0.5);

			//얼굴일 확률이 더 크다면 해당 위치 픽셀의 r,g,b값을 255로 변경.
			//그렇지 않다면 0으로 변경
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

//여기부터 퍼지 코드
void CTest2View::OnFuzzyHistoR() // 얼굴의 R값을 가지는 히스토그램
{
	//히스토그램 계산
	int i;
	float histo[256] = { 0.f, };
	
	float area = (float)(face_count + Nface_count); //입력된 영상 전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = face_r[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistoG() // 얼굴의 G값을 가지는 히스토그램
{
	//히스토그램 계산
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //입력된 영상 전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = face_g[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistob() // 얼굴의 B값을 가지는 히스토그램
{
	//히스토그램 계산
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //입력된 영상 전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = face_b[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistogramRBack() // 배경의 R값을 가지는 히스토그램
{
	//히스토그램 계산
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //입력된 영상 전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = Nface_r[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistogramGBack()// 배경의 G값을 가지는 히스토그램
{
	//히스토그램 계산
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //입력된 영상 전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = Nface_g[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

void CTest2View::OnFuzzyHistogramBBack()// 배경의 B값을 가지는 히스토그램
{
	//히스토그램 계산
	int i;
	float histo[256] = { 0.f, };

	float area = (float)(face_count + Nface_count); //입력된 영상 전체 픽셀
	for (i = 0; i < 256; i++)
		histo[i] = Nface_b[i] / area;

	HISTO_R_G_B dig;
	dig.SetImage(histo);
	dig.DoModal();
}

float R_HIGH(int pixel_r) // 반환은 확률값. 입력은 현재 R의 pixel값.
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

float R_LOW(int pixel_r) // R값이 LOW일때
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

float G_HIGH(int pixel_g) // 반환은 확률값. 입력은 현재 G의 pixel값.
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

float G_LOW(int pixel_g) // G값이 LOW일때
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

float B_HIGH(int pixel_b) // 반환은 확률값. 입력은 현재 B의 pixel값.
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

float B_LOW(int pixel_b) // B값이 LOW일때
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

float FACE_PROB(int prob) // 얼굴일 확률
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

float NFACE_PROB(int prob) // 배경일 확률
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

float CTest2View::Sort_max(float t[]){ // 정렬
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

	float res_F_NF[101]; // 그래프를 합친 결과
	int centroid=0; // 무게중심
	float temp_res[9] = { 0 ,}; // 결과 값 임시저장


	CDib Dib1;
	Dib1.CreateRGBImage(w, h, 0x00FFFFFF);
	RGBBYTE** ptr2 = Dib1.GetRGBPtr();

	float sum_bottom = 0, sum_top = 0; // 분자와 분모의 합

	for (z = 0; z < h; z++){
		for (k = 0; k < w; k++){
			sum_bottom = 0, sum_top = 0;
			for (j = 0; j < 101; j++){
				temp_res[0] = R_HIGH(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * FACE_PROB(j); // 다 HIGH일 경우 페이스
				temp_res[1] = R_HIGH(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j); // 노란색일 경우 N페이스
				temp_res[2] = R_HIGH(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * FACE_PROB(j);
				temp_res[3] = R_HIGH(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j);  // 갈색은 페이스
				temp_res[4] = R_LOW(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * FACE_PROB(j);
				temp_res[5] = R_LOW(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_HIGH(ptr[z][k].b) * NFACE_PROB(j); // LOW 2개 이상 N 페이스
				temp_res[6] = R_LOW(ptr[z][k].r) * G_HIGH(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j); // LOW 2개 이상 N 페이스
				temp_res[7] = R_LOW(ptr[z][k].r) * G_LOW(ptr[z][k].g) * B_LOW(ptr[z][k].b) * NFACE_PROB(j); // LOW 2개 이상 N 페이스

				res_F_NF[j] = Sort_max(temp_res); // 각 조건 그래프를 MAX
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

#define MAX_LABEL 10000 // 영상의 최대 갯수 10000개.
void CTest2View::OnLabling()
{
	register int i, j, w, h;
	RGBBYTE** ptr = nullptr;
	int nConnNumThre = 100; // 일정개수 이하를 가지는 레이블리은 삭제하기위한 임계값

	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()) {
		w = pDoc->m_Dib.GetWidth(); // 영상의 가로
		h = pDoc->m_Dib.GetHeight(); // 영상의 세로
		ptr = pDoc->m_Dib.GetRGBPtr(); // 주소값 할당
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
	// 영상 이진화
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (ptr2[y][x].r < 128) Label[y][x] = -1; // 볼필요없는 애들
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
			if (y>0 && x > 0) { // 스캔하여 0보다 큰 값이 나오면
				if (Label[y][x] >= 0) {
					left = Label[y][x - 1]; // 왼쪽체크
					top = Label[y - 1][x]; // 위쪽체크
					if (left == -1 && top != -1) {
						Label[y][x] = r[top];
					}// 위쪽에 값이 있다면 탑값으로
					else if (left != -1 && top == -1) { // 왼쪽에 값이 있다면 왼쪽값 대입
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
					else if (left != -1 && top != -1) { // 둘다 값이 있으면.. 
						if (r[left] == r[top]) { // 둘다 같다면 왼쪽
							Label[y][x] = r[left];
						}
						else if (r[left] > r[top]) { // 왼쪽값이 크다면 위쪽 값으로.. ( 적은 값으로 대체 )
							Label[y][x] = r[top];
							r[left] = r[top];
						}
						else { // 위쪽값이 크다면 왼쪽값으로.. (적은 값으로 대체)
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

	str.Format(_T("레이블 개수 = %d"), cnt + 1);
	AfxMessageBox(str);
}

void DirectoryFind(char* filename, CDib& dib) {
	_finddata_t fd;
	long handle;
	int result = 1;
	//handle = _findfirst(".\\*.*", &fd);  //현재 폴더 내 모든 파일을 찾는다.     
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

void CTest2View::OnTrain() // 학습데이터 학습. 함부로 누르지말자;;;
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	register int i, j, w, h;
	BYTE** ptr = nullptr;
	CTest2Doc* pDoc = GetDocument();
	if (pDoc->m_Dib.IsValid()){
		w = pDoc->m_Dib.GetWidth();
		h = pDoc->m_Dib.GetHeight();
		ptr = pDoc->m_Dib.GetPtr();
	}

	CDib Dib1;		//입력 영상 받아오고
	Dib1.CreateGrayImage(w, h, 0);		//gray 이미지로 만들고
	BYTE** ptr2 = Dib1.GetPtr();		//byte 이중 포인터 ptr2 만들고

	//데이터 이진화
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			ptr2[i][j] = (ptr[i][j] > 128) ? 0 : 255;
		}
	}

	//파일 출력
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

void CTest2View::OnNcBinary() // 영상 이진화. 테스트 할 데이터를 이진화한다.
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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

	//데이터 이진화
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


void CTest2View::OnTest() // 테스트. MLP에 넣어보자.
{
	int layers1[] = { 729, 15, 10 }; //input layer, hidden layer, output layer
	MultiLayerPerceptron mlp1(3, layers1); //3= number of layer
	mlp1.Actual_Test("test_dat.txt");
}
