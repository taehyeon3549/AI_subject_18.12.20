/*********************************************************************
* File  : mlp.h
*********************************************************************/


#ifndef _MLP_H_
#define _MLP_H_


struct Neuron {
	double  x;     /*  입력 값 */
	double  e;     /*  오차율  */
	double* w;     /*  가중치 값 */
	double* dw;    /*  가중치 보정 값 */
	double* wsave; /*  가중치 저장. 이전 가중치 */
};

struct Layer {
	int     nNumNeurons; // 각 레이어의 뉴런수
	Neuron* pNeurons; // 각 레이어가 가지고있는 뉴런집합
};

class MultiLayerPerceptron {

	int    nNumLayers; // 레이어 수
	Layer* pLayers; // 각 레이어의 집합

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
