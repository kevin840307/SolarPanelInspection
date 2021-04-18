// ImageKmean.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <ppl.h>

typedef unsigned char UCHAR;
typedef const unsigned char C_UCHAR;
typedef const char C_CHAR;
typedef __int32 INT32;
typedef const __int32 C_INT32;
typedef unsigned __int32 UINT32;
typedef const unsigned __int32 C_UINT32;
typedef const __int32 C_INT32;
typedef const double C_DOUBLE;
typedef const float C_FLOAT;
typedef const bool C_BOOL;

using namespace std;

void RandData(INT32** data, C_UINT32 D
	, C_UINT32 minN, C_UINT32 maxN
	, C_UINT32 minValue)
{
	srand(time(NULL));

	for (UINT32 n = minN; n < maxN; n++)
	{
		for (UINT32 d = 0; d < D; d++)
		{
			data[n][d] = rand() % 10 + minValue;
		}
	}
}

void RandData(INT32** data, C_UINT32 N, C_UINT32 D)
{
	srand(time(NULL));

	for (UINT32 n = 0; n < N; n++)
	{
		for (UINT32 d = 0; d < D; d++)
		{
			data[n][d] = rand() % 100;
		}
	}
}

void Print(INT32** data, C_UINT32 N, C_UINT32 D)
{
	for (UINT32 d = 0; d < D; d++)
	{
		for (UINT32 n = 0; n < N; n++)
		{
			cout << data[n][d] << ", ";
		}
		cout << endl;
	}
}

UINT32 Update(INT32** centerDatas, C_INT32** centerSum, C_UINT32* centerCount
	, C_UINT32 centerN, C_UINT32 D)
{
	UINT32 update = 0;
	for (UINT32 centerIndex = 0; centerIndex < centerN; centerIndex++)
	{
		C_UINT32 count = centerCount[centerIndex];
		if (count == 0)
		{
			continue;
		}
		for (UINT32 d = 0; d < D; d++)
		{
			centerDatas[centerIndex][d] = centerSum[centerIndex][d] / count;
		}
		update++;
	}

	return update;
}

void Add(C_INT32* trainData, INT32* centerSum
	, C_UINT32 D)
{
	for (UINT32 index = 0; index < D; index++)
	{
		centerSum[index] += trainData[index];
	}
}

double Distance(C_INT32* trainData, C_INT32* centerData
	, C_UINT32 D)
{
	double sum = 0;
	for (UINT32 index = 0; index < D; index++)
	{
		sum += pow(trainData[index] - centerData[index], 2);
	}
	return sqrt(sum);
}

void Train(C_INT32** trainDatas, C_UINT32 trainN
	, INT32** centerDatas, C_UINT32 centerN
	, C_UINT32 D)
{
	C_UINT32 trainTimes = 100;
	for (UINT32 time = 0; time < trainTimes; time++)
	{
		UINT32* centerCount = new UINT32[centerN]{ 0 };
		INT32** centerSum = new INT32*[centerN];
		for (UINT32 index = 0; index < centerN; index++)
		{
			centerSum[index] = new INT32[D]{ 0 };
		}

		for (UINT32 trainIndex = 0; trainIndex < trainN; trainIndex++)
		{
			double minValue = INT_MAX;
			INT32 minIndex = 0;
			for (UINT32 centerIndex = 0; centerIndex < centerN; centerIndex++)
			{
				C_DOUBLE nowValue = Distance(trainDatas[trainIndex], centerDatas[centerIndex], D);
				if (minValue > nowValue)
				{
					minValue = nowValue;
					minIndex = centerIndex;
				}
			}
			Add(trainDatas[trainIndex], centerSum[minIndex], D);
			centerCount[minIndex]++;
		}

		C_UINT32 update = Update(centerDatas, const_cast<C_INT32**>(centerSum), centerCount
			, centerN, D);

		for (UINT32 index = 0; index < centerN; index++)
		{
			delete[] centerSum[index];
			centerSum[index] = nullptr;
		}
		delete[] centerSum;
		centerSum = nullptr;

		delete[] centerCount;
		centerCount = nullptr;

		//if (update == 0)
		//{
		//	return;
		//}
	}

}

#include "KMeam.h"
int main()
{
	KMeam<INT32> kMeam(3, 100, 2, 100);
	kMeam.PrintCenterDatas();
	kMeam.PrintTrainDatas();
	kMeam.Train(100);
	kMeam.PrintCenterDatas();
	C_UINT32 D = 2;
	C_UINT32 trainN = 100;
	C_UINT32 centerN = 3;

	INT32** trainDatas = new INT32*[trainN];
	for (UINT32 index = 0; index < trainN; index++)
	{
		trainDatas[index] = new INT32[D];
	}
	RandData(trainDatas, D, 0, 33, 45);
	RandData(trainDatas, D, 33, 66, 30);
	RandData(trainDatas, D, 66, 100, 50);

	INT32** centerDatas = new INT32*[centerN];
	for (UINT32 index = 0; index < centerN; index++)
	{
		centerDatas[index] = new INT32[D];
	}
	RandData(centerDatas, centerN, D);


	Train(const_cast<C_INT32**>(trainDatas), trainN, centerDatas, centerN, D);

	cout << "Point:" << endl;
	Print(trainDatas, trainN, D);
	cout << "Center:" << endl;
	Print(centerDatas, centerN, D);


	for (UINT32 index = 0; index < trainN; index++)
	{
		delete[] trainDatas[index];
		trainDatas[index] = nullptr;
	}
	delete[] trainDatas;
	trainDatas = nullptr;

	for (UINT32 index = 0; index < centerN; index++)
	{
		delete[] centerDatas[index];
		centerDatas[index] = nullptr;
	}
	delete[] centerDatas;
	centerDatas = nullptr;

	system("pause");
	return 0;
}

