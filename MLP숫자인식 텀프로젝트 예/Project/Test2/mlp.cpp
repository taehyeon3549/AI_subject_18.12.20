#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS
#define D_MAX_ARRAY_SIZE 12000
#define MAX_SIZE 20000


#include "mlp.h"

using namespace std;

//git test
//문자열  자르는 코드
string* StringSplit(string strTarget, string strTok)
{
	int     nCutPos;
	int     nIndex = 0;
	string* strResult = new string[D_MAX_ARRAY_SIZE];

	while ((nCutPos = strTarget.find_first_of(strTok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			strResult[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		strResult[nIndex++] = strTarget.substr(0, nCutPos);
	}

	return strResult;
}

void InitializeRandoms()
{
	//  srand( (unsigned)time( NULL ) );
	srand(4711);
}

// 입력값이 상수일때, Low와 High사이의 입의값을  정함.
int RandomEqualINT(int Low, int High)
{
	return rand() % (High - Low + 1) + Low;
}

// 입력값이 실수일때, Low와 High사이의 입의값을  정함.
double RandomEqualREAL(double Low, double High)
{
	return ((double)rand() / RAND_MAX) * (High - Low) + Low;
}

// 다층 퍼셉트론. 
MultiLayerPerceptron::MultiLayerPerceptron(int nl, int npl[]) :
nNumLayers(0),
pLayers(0),
dEta(0.25),
dAlpha(0.9),
dGain(1.0),
dMSE(0.0),
dMAE(0.0),
dAvgTestError(0.0)
{
	int i, j;

	/* nl = 레이어의 갯수, pLayers = 각 레이어의 집합 */
	nNumLayers = nl;
	pLayers = new Layer[nl];

	/* 초기화 과정. i는 0 = 입력층, 1 = 은닉층, 2 = 출력층 을 나타낸다.  */
	for (i = 0; i < nl; i++)
	{

		/* 메인에서 설정해준 각 층들의 뉴런들을 생성한다. */
		pLayers[i].nNumNeurons = npl[i];
		pLayers[i].pNeurons = new Neuron[npl[i]];

		/* 입력층의 x값을 1로 초기화. 오차율을 0으로 초기화. j는 각 층들의 뉴런 갯수를 나타낸다.  */
		for (j = 0; j < npl[i]; j++)
		{
			pLayers[i].pNeurons[j].x = 1.0;
			pLayers[i].pNeurons[j].e = 0.0;
			if (i>0)
			{
				pLayers[i].pNeurons[j].w = new double[npl[i - 1]]; // 은닉층과, 출력층에는 가중치가 존재하므로 배열을 지정해 준다.
				pLayers[i].pNeurons[j].dw = new double[npl[i - 1]];
				pLayers[i].pNeurons[j].wsave = new double[npl[i - 1]];
			}
			else
			{
				pLayers[i].pNeurons[j].w = NULL; // 입력층에는 가중치가 없으므로 null로 나타낸다.
				pLayers[i].pNeurons[j].dw = NULL;
				pLayers[i].pNeurons[j].wsave = NULL;
			}
		}

	}


}

MultiLayerPerceptron::~MultiLayerPerceptron()
{
	int i, j;
	for (i = 0; i < nNumLayers; i++)
	{
		if (pLayers[i].pNeurons)
		{
			for (j = 0; j < pLayers[i].nNumNeurons; j++)
			{
				if (pLayers[i].pNeurons[j].w)
					delete[] pLayers[i].pNeurons[j].w;
				if (pLayers[i].pNeurons[j].dw)
					delete[] pLayers[i].pNeurons[j].dw;
				if (pLayers[i].pNeurons[j].wsave)
					delete[] pLayers[i].pNeurons[j].wsave;
			}
		}
		delete[] pLayers[i].pNeurons;
	}
	delete[] pLayers;
}

//초기 가중치는 랜덤하게

void MultiLayerPerceptron::RandomWeights()
{
	int i, j, k, count = 0;

	FILE * fp;
	char c[100];
	string s;
	if ((fp = fopen("weight5.txt", "r")) != NULL)
	{
		int i = 0;
		while (fgets(c, 10, fp) != NULL)
		{
			s += c;
		}
	}

	char* toki = "\t";
	string * tok = StringSplit(s, toki);

	for (i = 1; i < nNumLayers; i++)
	{
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
			{
				if (tok[count] != "") // 저장되어있는 가중치가 있다면
				{
					pLayers[i].pNeurons[j].w[k] = stod(tok[count]);
				}
				else // 없으면 -0.5 ~ 0.5로 초기화
				{
					pLayers[i].pNeurons[j].w[k] = RandomEqualREAL(-0.5, 0.5);
				}

				pLayers[i].pNeurons[j].dw[k] = 0.0;
				pLayers[i].pNeurons[j].wsave[k] = 0.0;
				count++;
			}
		}
	}
	fclose(fp);
}

//입력층에 값 입력
void MultiLayerPerceptron::SetInputSignal(double* input)
{
	int i;
	for (i = 0; i < pLayers[0].nNumNeurons; i++)
	{
		pLayers[0].pNeurons[i].x = input[i];
	}
}

void MultiLayerPerceptron::GetOutputSignal(double* output)
{
	int i;
	// 출력층의 뉴런의 수만큼.
	for (i = 0; i < pLayers[nNumLayers - 1].nNumNeurons; i++)
	{
		output[i] = pLayers[nNumLayers - 1].pNeurons[i].x; // 출력층의 실제 출력값을 output배열에 저장.
	}
}

void MultiLayerPerceptron::SaveWeights()
{
	int i, j, k;
	for (i = 1; i < nNumLayers; i++)
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
				pLayers[i].pNeurons[j].wsave[k] = pLayers[i].pNeurons[j].w[k]; // 현재 가중치를 이전 가중치로 저장.
}

void MultiLayerPerceptron::RestoreWeights()
{
	int i, j, k;
	for (i = 1; i < nNumLayers; i++)
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
				pLayers[i].pNeurons[j].w[k] = pLayers[i].pNeurons[j].wsave[k]; // 이전 가중치를 현재 가중치에 저장
}

/***************************************************************************/
/* calculate and feedforward outputs from the first layer to the last      */
void MultiLayerPerceptron::PropagateSignal()
{
	int i, j, k;
	/* 입력층을 제외한 각 층의 뉴런 수 만큼 for문 반복  */
	for (i = 1; i < nNumLayers; i++)
	{
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			/* i의 값에따라 현재 층에서 다음층으로의 입력 값(실제  출력값)을 해당 가중치와 곱하고 더한다.   */
			double sum = 0.0;
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
			{
				double out = pLayers[i - 1].pNeurons[k].x;
				double w = pLayers[i].pNeurons[j].w[k];
				sum += w * out;
			}
			/* 은닉층의 뉴런들과 출력층의 뉴런들에서의 실제 출력을 계산한다.(sigmoid사용) */
			pLayers[i].pNeurons[j].x = 1.0 / (1.0 + exp(-dGain * sum));
		}
	}
}

void MultiLayerPerceptron::ComputeOutputError(double* target)
{
	int  i;
	dMSE = 0.0;
	dMAE = 0.0;
	for (i = 0; i < pLayers[nNumLayers - 1].nNumNeurons; i++)
	{
		double x = pLayers[nNumLayers - 1].pNeurons[i].x; // 출력층의 실제 출력 값.
		double d = target[i] - x; // 기대 값 - 실제 출력 값.
		pLayers[nNumLayers - 1].pNeurons[i].e = dGain * x * (1.0 - x) * d; // 출력층의 오차기울기를 구해서 e에 넣는다.
		dMSE += (d * d); // 오차 제곱을 다 더한다.
		dMAE += fabs(d); // 오차의 절대값을 취한뒤 다 더한다.
	}
	/* 오차제곱 합의 평균 */
	dMSE /= (double)pLayers[nNumLayers - 1].nNumNeurons;
	/* 오차의 절대 값 합의 평균 */
	dMAE /= (double)pLayers[nNumLayers - 1].nNumNeurons;
}

/***************************************************************************/
/* backpropagate error from the output layer through to the first layer    */

void MultiLayerPerceptron::BackPropagateError()
{
	int i, j, k;
	/* 은닉층 -> 입력층 순으로 for문을 반복한다  */
	for (i = (nNumLayers - 2); i >= 0; i--)
	{
		/* 해당 층의 뉴런의 수 만큼 for문을 반복한다 */
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			double x = pLayers[i].pNeurons[j].x; // 현재 선택되어진 층의 뉴런의 출력 값을 x에 저장
			double E = 0.0;
			/* 이전 단계에서 자신에 의해 발생한 오차를 모두 더한것을 다시 오차로 사용하여(E), 해당 층의 뉴런의 오차 기울기를 구한다.  */
			for (k = 0; k < pLayers[i + 1].nNumNeurons; k++)
			{
				E += pLayers[i + 1].pNeurons[k].w[j] * pLayers[i + 1].pNeurons[k].e;
			}
			pLayers[i].pNeurons[j].e = dGain * x * (1.0 - x) * E;
		}
	}
}

/***************************************************************************/
/* update weights for all of the neurons from the first to the last layer  */

void MultiLayerPerceptron::AdjustWeights()
{
	int i, j, k;
	/* 입력층과 은닉층, 은닉층과 출력층 사이의 가중치를 학습시킨다.  */
	for (i = 1; i < nNumLayers; i++)
	{
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
			{
				double x = pLayers[i - 1].pNeurons[k].x;
				double e = pLayers[i].pNeurons[j].e;
				double dw = pLayers[i].pNeurons[j].dw[k];
				pLayers[i].pNeurons[j].w[k] += dEta * x * e + dAlpha * dw; // dEta = 매개변수, dAlpha = 임계치(?)
				pLayers[i].pNeurons[j].dw[k] = dEta * x * e;
			}
		}
	}
}

//트레이닝 시뮬.
void MultiLayerPerceptron::Simulate(double* input, double* output, double* target, bool training)
{

	if (!input)  return;
	if (!target) return;

	/* SetInputSignal로 input배열에 있는 숫자들을 입력층의 x에 넣고, PropagateSignal에서 실제 출력 값을 계산한다. */
	SetInputSignal(input);
	PropagateSignal();
	if (output) GetOutputSignal(output); // output에 출력층의 뉴런의 수만큼 실제 출력값을 output배열에 저장.

	/* output에 값이 있고, 학습중이 아니라면 입력 값 2개와 기대 값과 실제 출력 값을 print한다.  */
	if (output && !training) printf("test: %.2f %.2f %.2f = %.9f\n", input[0], input[1], target[0], output[0]);

	/* 출력 값에 대한 오류 계산 */
	ComputeOutputError(target);

	/* 학습중이라면 BackPropagateError를 이용하여 오차를 다시 구하고, AdjustWeights를 이용하여 가중치를 학습시킨다.  */
	if (training)
	{
		BackPropagateError();
		AdjustWeights();
	}
}

//테스트 시뮬. 12월7일
void MultiLayerPerceptron::Simulate(double* input, double* output)
{
	if (!input)  return;

	SetInputSignal(input);
	PropagateSignal();
	if (output) GetOutputSignal(output); 

	double temp;
	temp = output[0];
	int save = 0;
	for (int i = 1; i < 10; i++){
		if (temp < output[i]){
			temp = output[i];
			save = i;
		}
	}

	if (output)
	{
		CString msg;
		msg.Format(_T("값은 %d 입니다."), save);
		AfxMessageBox(msg);
	}	
}

bool read_number(FILE* fp, double* number)
{
	char szWord[256];
	int i = 0;
	int b;

	*number = 0.0;

	szWord[0] = '\0';
	while (((b = fgetc(fp)) != EOF) && (i<255))
	{
		if ((b == '.') ||
			(b == '0') ||
			(b == '1') ||
			(b == '2') ||
			(b == '3') ||
			(b == '4') ||
			(b == '5') ||
			(b == '6') ||
			(b == '7') ||
			(b == '8') ||
			(b == '9'))
		{
			szWord[i++] = (char)b;
		}
		else
			if (i>0) break;
	}
	szWord[i] = '\0';

	if (i == 0) return false;

	*number = atof(szWord); // 숫자 저장

	return true;
}

int MultiLayerPerceptron::Train(const char* fname) // 학습
{
	int count = 0;
	int nbi = 0;
	int nbt = 0;
	int save = 0;
	double* input = NULL;
	double* output = NULL;
	double* target = NULL;
	FILE*   fp = NULL;

	fp = fopen(fname, "r");
	if (!fp) return 0;

	input = new double[pLayers[0].nNumNeurons]; // 입력층의 갯 수 만큼 input 배열 생성
	output = new double[pLayers[nNumLayers - 1].nNumNeurons]; // 출력층의 갯수만큼 output 배열 생성
	target = new double[pLayers[nNumLayers - 1].nNumNeurons]; // 출력층의 갯수만큼 output 배열 생성

	if (!input) return 0;
	if (!output) return 0;
	if (!target) return 0;

	dAvgTestError = 0.0; // 12월6일 추가. 곽
	while (!feof(fp))
	{
		double dNumber;
		if (read_number(fp, &dNumber))
		{
			if (nbi < pLayers[0].nNumNeurons)
				input[nbi++] = dNumber;

			else if (nbt < pLayers[nNumLayers - 1].nNumNeurons)
			{
				for (int i = 0; i < 10; i++){
					target[i] = 0;
				}
				save = dNumber;
				target[save] = 1;
				nbt = 10;
			}

			/* nbi가 입력층의 뉴런 수와 같고, nbt가 출력층의 뉴런 수와 같다면 */
			if ((nbi == pLayers[0].nNumNeurons) &&
				(nbt == pLayers[nNumLayers - 1].nNumNeurons))
			{
				Simulate(input, output, target, true); // 앞의 저장된 배열로 simulate 수행
				dAvgTestError += dMAE; // 12월6일 추가. 곽
				nbi = 0;
				nbt = 0; // nbi,nbt초기화
				count++; // count +1
			}
		}
		else
		{
			break;
		}
	}

	if (fp) fclose(fp);

	if (input)  delete[] input;
	if (output) delete[] output;
	if (target) delete[] target;

	return count;
}

int MultiLayerPerceptron::Test(const char* fname) // 테스트
{
	int count = 0;
	int nbi = 0;
	//int nbt = 0;
	double* input = NULL;
	double* output = NULL;
	//double* target = NULL;
	FILE*   fp = NULL;

	fp = fopen(fname, "r");
	if (!fp) return 0;

	input = new double[pLayers[0].nNumNeurons]; // 입력층의 갯 수 만큼 input 배열 생성
	output = new double[pLayers[nNumLayers - 1].nNumNeurons]; // 출력층의 갯수만큼 output 배열 생성
	//target = new double[pLayers[nNumLayers - 1].nNumNeurons]; // 출력층의 갯수만큼 target 배열 생성

	if (!input) return 0;
	if (!output) return 0;
	//if (!target) return 0;

	dAvgTestError = 0.0;

	while (!feof(fp))
	{
		double dNumber;
		if (read_number(fp, &dNumber))
		{
			/* 설정해준 입력층의 갯수보다 작으면 읽은 숫자를 input배열에 저장. 커지면, target배열에 숫자 저장  */
			if (nbi < pLayers[0].nNumNeurons)
				input[nbi++] = dNumber;

			/*else if (nbt < pLayers[nNumLayers - 1].nNumNeurons)
			target[nbt++] = dNumber;*/

			/* nbi가 입력층의 뉴런 수와 같다면 */
			if ((nbi == pLayers[0].nNumNeurons))/* && (nbt == pLayers[nNumLayers - 1].nNumNeurons)*/
			{
				//Simulate(input, output, target, false); // 앞의 저장된 배열로 simulate 수행
				Simulate(input, output);
				//dAvgTestError += dMAE; // 테스트에서 나오는 평균 오차값은 ComputeOutputError시 나오는 오차의 절대값의 합의 평균이다.
				nbi = 0;
				//nbt = 0;
				count++;
			}
		}
		else
		{
			break;
		}
	}

	dAvgTestError /= count; // 훈련시킨 갯수만큼 오차를 다시 나누어준다. 즉 훈련을 마치고 난 다음의 오차의 평균.

	if (fp) fclose(fp);

	if (input)  delete[] input;
	if (output) delete[] output;
	//if (target) delete[] target;

	return count;
}

int MultiLayerPerceptron::Evaluate()
{
	int count = 0;
	return count;
}

void MultiLayerPerceptron::Run(const char* fname, const int& maxiter)
{
	int    countTrain = 0; // 훈련 횟수
	int    countLines = 0; // 읽은 라인 수
	bool   Stop = false;
	bool   firstIter = true;
	double dMinTestError = 0.0;

	/* InitializeRandoms은 랜덤의 시드를 바꾸어 주는 역할, RandomWeights는 가중치를 랜덤하게 지정하는 역할  */
	InitializeRandoms();
	RandomWeights();

	/* 훈련과 테스트를 사용자가 지정한 횟수만큼 반복한다.
	지정한 횟수가 되기전에 훈련시 나타난 평균 오차 값 < 1.2*최소 오차값 이라면
	반복을 멈추고 이전 가중치로 대체한다.  */
	do {
		countLines += Train(fname); // 학습
		countTrain++; // 훈련의 횟수

		if (firstIter) // 처음 반복 시
		{
			dMinTestError = dAvgTestError; // 최소 오차값에 훈련시 나온 평균 오차값을 넣음
			firstIter = false;
		}

		if (dAvgTestError < dMinTestError) // 훈련에서 나온 평균오차값이 최소 오차값보다 작다면
		{
			printf(" -> saving weights\n");
			dMinTestError = dAvgTestError; // 대체
			SaveWeights(); // 가중치 저장
		}
		else if (dAvgTestError > 1.2 * dMinTestError) // 1.2*최소 오차값이 훈련에서 나온 평균 오차값 보다 작다면
		{
			printf(" -> stopping training and restoring weights\n");
			Stop = true; // 반복 멈춤
			RestoreWeights(); // 이전 가중치를 현재 가중치로 대체.

			ofstream outFile("weight5.txt");

			for (int i = 1; i < nNumLayers; i++)
			{
				for (int j = 0; j < pLayers[i].nNumNeurons; j++)
				{
					for (int k = 0; k < pLayers[i - 1].nNumNeurons; k++)
					{
						outFile << pLayers[i].pNeurons[j].w[k] << "\t";
					}
				}
			}
			outFile.close();
		}
		else
		{
			printf(" -> ok\n");
		}

		printf("%d \tTestError: %f", countTrain, dAvgTestError);
	} while ((!Stop) && (countTrain<maxiter));

	printf("Training complete\n");

	if (!Stop){
		ofstream outFile("weight5.txt");

		for (int i = 1; i < nNumLayers; i++)
		{
			for (int j = 0; j < pLayers[i].nNumNeurons; j++)
			{
				for (int k = 0; k < pLayers[i - 1].nNumNeurons; k++)
				{
					outFile << pLayers[i].pNeurons[j].wsave[k] << "\t";
				}
			}
		}
		outFile.close();
	}
}

//테스트만 따로. 12월6일
void MultiLayerPerceptron::Actual_Test(const char* fname)
{
	RandomWeights();
	Test(fname); //테스트...
}