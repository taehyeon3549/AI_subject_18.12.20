/*********************************************************************
* File  : mlp.h
*********************************************************************/


#ifndef _MLP_H_
#define _MLP_H_


struct Neuron {
	double  x;     /*  �Է� �� */
	double  e;     /*  ������  */
	double* w;     /*  ����ġ �� */
	double* dw;    /*  ����ġ ���� �� */
	double* wsave; /*  ����ġ ����. ���� ����ġ */
};

struct Layer {
	int     nNumNeurons; // �� ���̾��� ������
	Neuron* pNeurons; // �� ���̾ �������ִ� ��������
};

class MultiLayerPerceptron {

	int    nNumLayers; // ���̾� ��
	Layer* pLayers; // �� ���̾��� ����

	double dMSE;
	double dMAE;

	void RandomWeights();

	void SetInputSignal(double* input);
	void GetOutputSignal(double* output);

	void SaveWeights();
	void RestoreWeights();

	void PropagateSignal();
	void ComputeOutputError(double* target);
	void BackPropagateError();
	void AdjustWeights();

	void Simulate(double* input, double* output, double* target, bool training);
	void Simulate(double* input, double* output);

public:

	double dEta;
	double dAlpha;
	double dGain;
	double dAvgTestError;

	MultiLayerPerceptron(int nl, int npl[]);
	~MultiLayerPerceptron();

	int Train(const char* fnames);
	int Test(const char* fname);
	int Evaluate();

	void Run(const char* fname, const int& maxiter);
	void Actual_Test(const char* fname);

};

#endif
