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
//���ڿ�  �ڸ��� �ڵ�
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

// �Է°��� ����϶�, Low�� High������ ���ǰ���  ����.
int RandomEqualINT(int Low, int High)
{
	return rand() % (High - Low + 1) + Low;
}

// �Է°��� �Ǽ��϶�, Low�� High������ ���ǰ���  ����.
double RandomEqualREAL(double Low, double High)
{
	return ((double)rand() / RAND_MAX) * (High - Low) + Low;
}

// ���� �ۼ�Ʈ��. 
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

	/* nl = ���̾��� ����, pLayers = �� ���̾��� ���� */
	nNumLayers = nl;
	pLayers = new Layer[nl];

	/* �ʱ�ȭ ����. i�� 0 = �Է���, 1 = ������, 2 = ����� �� ��Ÿ����.  */
	for (i = 0; i < nl; i++)
	{

		/* ���ο��� �������� �� ������ �������� �����Ѵ�. */
		pLayers[i].nNumNeurons = npl[i];
		pLayers[i].pNeurons = new Neuron[npl[i]];

		/* �Է����� x���� 1�� �ʱ�ȭ. �������� 0���� �ʱ�ȭ. j�� �� ������ ���� ������ ��Ÿ����.  */
		for (j = 0; j < npl[i]; j++)
		{
			pLayers[i].pNeurons[j].x = 1.0;
			pLayers[i].pNeurons[j].e = 0.0;
			if (i>0)
			{
				pLayers[i].pNeurons[j].w = new double[npl[i - 1]]; // ��������, ��������� ����ġ�� �����ϹǷ� �迭�� ������ �ش�.
				pLayers[i].pNeurons[j].dw = new double[npl[i - 1]];
				pLayers[i].pNeurons[j].wsave = new double[npl[i - 1]];
			}
			else
			{
				pLayers[i].pNeurons[j].w = NULL; // �Է������� ����ġ�� �����Ƿ� null�� ��Ÿ����.
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

//�ʱ� ����ġ�� �����ϰ�

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
				if (tok[count] != "") // ����Ǿ��ִ� ����ġ�� �ִٸ�
				{
					pLayers[i].pNeurons[j].w[k] = stod(tok[count]);
				}
				else // ������ -0.5 ~ 0.5�� �ʱ�ȭ
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

//�Է����� �� �Է�
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
	// ������� ������ ����ŭ.
	for (i = 0; i < pLayers[nNumLayers - 1].nNumNeurons; i++)
	{
		output[i] = pLayers[nNumLayers - 1].pNeurons[i].x; // ������� ���� ��°��� output�迭�� ����.
	}
}

void MultiLayerPerceptron::SaveWeights()
{
	int i, j, k;
	for (i = 1; i < nNumLayers; i++)
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
				pLayers[i].pNeurons[j].wsave[k] = pLayers[i].pNeurons[j].w[k]; // ���� ����ġ�� ���� ����ġ�� ����.
}

void MultiLayerPerceptron::RestoreWeights()
{
	int i, j, k;
	for (i = 1; i < nNumLayers; i++)
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
				pLayers[i].pNeurons[j].w[k] = pLayers[i].pNeurons[j].wsave[k]; // ���� ����ġ�� ���� ����ġ�� ����
}

/***************************************************************************/
/* calculate and feedforward outputs from the first layer to the last      */
void MultiLayerPerceptron::PropagateSignal()
{
	int i, j, k;
	/* �Է����� ������ �� ���� ���� �� ��ŭ for�� �ݺ�  */
	for (i = 1; i < nNumLayers; i++)
	{
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			/* i�� �������� ���� ������ ������������ �Է� ��(����  ��°�)�� �ش� ����ġ�� ���ϰ� ���Ѵ�.   */
			double sum = 0.0;
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
			{
				double out = pLayers[i - 1].pNeurons[k].x;
				double w = pLayers[i].pNeurons[j].w[k];
				sum += w * out;
			}
			/* �������� ������� ������� �����鿡���� ���� ����� ����Ѵ�.(sigmoid���) */
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
		double x = pLayers[nNumLayers - 1].pNeurons[i].x; // ������� ���� ��� ��.
		double d = target[i] - x; // ��� �� - ���� ��� ��.
		pLayers[nNumLayers - 1].pNeurons[i].e = dGain * x * (1.0 - x) * d; // ������� �������⸦ ���ؼ� e�� �ִ´�.
		dMSE += (d * d); // ���� ������ �� ���Ѵ�.
		dMAE += fabs(d); // ������ ���밪�� ���ѵ� �� ���Ѵ�.
	}
	/* �������� ���� ��� */
	dMSE /= (double)pLayers[nNumLayers - 1].nNumNeurons;
	/* ������ ���� �� ���� ��� */
	dMAE /= (double)pLayers[nNumLayers - 1].nNumNeurons;
}

/***************************************************************************/
/* backpropagate error from the output layer through to the first layer    */

void MultiLayerPerceptron::BackPropagateError()
{
	int i, j, k;
	/* ������ -> �Է��� ������ for���� �ݺ��Ѵ�  */
	for (i = (nNumLayers - 2); i >= 0; i--)
	{
		/* �ش� ���� ������ �� ��ŭ for���� �ݺ��Ѵ� */
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			double x = pLayers[i].pNeurons[j].x; // ���� ���õǾ��� ���� ������ ��� ���� x�� ����
			double E = 0.0;
			/* ���� �ܰ迡�� �ڽſ� ���� �߻��� ������ ��� ���Ѱ��� �ٽ� ������ ����Ͽ�(E), �ش� ���� ������ ���� ���⸦ ���Ѵ�.  */
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
	/* �Է����� ������, �������� ����� ������ ����ġ�� �н���Ų��.  */
	for (i = 1; i < nNumLayers; i++)
	{
		for (j = 0; j < pLayers[i].nNumNeurons; j++)
		{
			for (k = 0; k < pLayers[i - 1].nNumNeurons; k++)
			{
				double x = pLayers[i - 1].pNeurons[k].x;
				double e = pLayers[i].pNeurons[j].e;
				double dw = pLayers[i].pNeurons[j].dw[k];
				pLayers[i].pNeurons[j].w[k] += dEta * x * e + dAlpha * dw; // dEta = �Ű�����, dAlpha = �Ӱ�ġ(?)
				pLayers[i].pNeurons[j].dw[k] = dEta * x * e;
			}
		}
	}
}

//Ʈ���̴� �ù�.
void MultiLayerPerceptron::Simulate(double* input, double* output, double* target, bool training)
{

	if (!input)  return;
	if (!target) return;

	/* SetInputSignal�� input�迭�� �ִ� ���ڵ��� �Է����� x�� �ְ�, PropagateSignal���� ���� ��� ���� ����Ѵ�. */
	SetInputSignal(input);
	PropagateSignal();
	if (output) GetOutputSignal(output); // output�� ������� ������ ����ŭ ���� ��°��� output�迭�� ����.

	/* output�� ���� �ְ�, �н����� �ƴ϶�� �Է� �� 2���� ��� ���� ���� ��� ���� print�Ѵ�.  */
	if (output && !training) printf("test: %.2f %.2f %.2f = %.9f\n", input[0], input[1], target[0], output[0]);

	/* ��� ���� ���� ���� ��� */
	ComputeOutputError(target);

	/* �н����̶�� BackPropagateError�� �̿��Ͽ� ������ �ٽ� ���ϰ�, AdjustWeights�� �̿��Ͽ� ����ġ�� �н���Ų��.  */
	if (training)
	{
		BackPropagateError();
		AdjustWeights();
	}
}

//�׽�Ʈ �ù�. 12��7��
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
		msg.Format(_T("���� %d �Դϴ�."), save);
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

	*number = atof(szWord); // ���� ����

	return true;
}

int MultiLayerPerceptron::Train(const char* fname) // �н�
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

	input = new double[pLayers[0].nNumNeurons]; // �Է����� �� �� ��ŭ input �迭 ����
	output = new double[pLayers[nNumLayers - 1].nNumNeurons]; // ������� ������ŭ output �迭 ����
	target = new double[pLayers[nNumLayers - 1].nNumNeurons]; // ������� ������ŭ output �迭 ����

	if (!input) return 0;
	if (!output) return 0;
	if (!target) return 0;

	dAvgTestError = 0.0; // 12��6�� �߰�. ��
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

			/* nbi�� �Է����� ���� ���� ����, nbt�� ������� ���� ���� ���ٸ� */
			if ((nbi == pLayers[0].nNumNeurons) &&
				(nbt == pLayers[nNumLayers - 1].nNumNeurons))
			{
				Simulate(input, output, target, true); // ���� ����� �迭�� simulate ����
				dAvgTestError += dMAE; // 12��6�� �߰�. ��
				nbi = 0;
				nbt = 0; // nbi,nbt�ʱ�ȭ
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

int MultiLayerPerceptron::Test(const char* fname) // �׽�Ʈ
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

	input = new double[pLayers[0].nNumNeurons]; // �Է����� �� �� ��ŭ input �迭 ����
	output = new double[pLayers[nNumLayers - 1].nNumNeurons]; // ������� ������ŭ output �迭 ����
	//target = new double[pLayers[nNumLayers - 1].nNumNeurons]; // ������� ������ŭ target �迭 ����

	if (!input) return 0;
	if (!output) return 0;
	//if (!target) return 0;

	dAvgTestError = 0.0;

	while (!feof(fp))
	{
		double dNumber;
		if (read_number(fp, &dNumber))
		{
			/* �������� �Է����� �������� ������ ���� ���ڸ� input�迭�� ����. Ŀ����, target�迭�� ���� ����  */
			if (nbi < pLayers[0].nNumNeurons)
				input[nbi++] = dNumber;

			/*else if (nbt < pLayers[nNumLayers - 1].nNumNeurons)
			target[nbt++] = dNumber;*/

			/* nbi�� �Է����� ���� ���� ���ٸ� */
			if ((nbi == pLayers[0].nNumNeurons))/* && (nbt == pLayers[nNumLayers - 1].nNumNeurons)*/
			{
				//Simulate(input, output, target, false); // ���� ����� �迭�� simulate ����
				Simulate(input, output);
				//dAvgTestError += dMAE; // �׽�Ʈ���� ������ ��� �������� ComputeOutputError�� ������ ������ ���밪�� ���� ����̴�.
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

	dAvgTestError /= count; // �Ʒý�Ų ������ŭ ������ �ٽ� �������ش�. �� �Ʒ��� ��ġ�� �� ������ ������ ���.

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
	int    countTrain = 0; // �Ʒ� Ƚ��
	int    countLines = 0; // ���� ���� ��
	bool   Stop = false;
	bool   firstIter = true;
	double dMinTestError = 0.0;

	/* InitializeRandoms�� ������ �õ带 �ٲپ� �ִ� ����, RandomWeights�� ����ġ�� �����ϰ� �����ϴ� ����  */
	InitializeRandoms();
	RandomWeights();

	/* �Ʒð� �׽�Ʈ�� ����ڰ� ������ Ƚ����ŭ �ݺ��Ѵ�.
	������ Ƚ���� �Ǳ����� �Ʒý� ��Ÿ�� ��� ���� �� < 1.2*�ּ� ������ �̶��
	�ݺ��� ���߰� ���� ����ġ�� ��ü�Ѵ�.  */
	do {
		countLines += Train(fname); // �н�
		countTrain++; // �Ʒ��� Ƚ��

		if (firstIter) // ó�� �ݺ� ��
		{
			dMinTestError = dAvgTestError; // �ּ� �������� �Ʒý� ���� ��� �������� ����
			firstIter = false;
		}

		if (dAvgTestError < dMinTestError) // �Ʒÿ��� ���� ��տ������� �ּ� ���������� �۴ٸ�
		{
			printf(" -> saving weights\n");
			dMinTestError = dAvgTestError; // ��ü
			SaveWeights(); // ����ġ ����
		}
		else if (dAvgTestError > 1.2 * dMinTestError) // 1.2*�ּ� �������� �Ʒÿ��� ���� ��� ������ ���� �۴ٸ�
		{
			printf(" -> stopping training and restoring weights\n");
			Stop = true; // �ݺ� ����
			RestoreWeights(); // ���� ����ġ�� ���� ����ġ�� ��ü.

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

//�׽�Ʈ�� ����. 12��6��
void MultiLayerPerceptron::Actual_Test(const char* fname)
{
	RandomWeights();
	Test(fname); //�׽�Ʈ...
}