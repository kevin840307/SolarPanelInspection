#pragma once
#ifndef KMEAN_H
#include "general.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

template <class T>
class KMean
{
public:
	// K is group
	KMean(const std::string& path);
	KMean(C_UINT32 dimension) : _dimension(dimension) {};
	KMean(C_UINT32 K, C_UINT32 dimension, C_UINT32 randMax, C_BOOL rand) : _dimension(dimension) { Init(K, randMax, rand); };
	KMean(C_UINT32 K, C_UINT32 N, C_UINT32 dimension, C_UINT32 randMax, C_BOOL rand) : _dimension(dimension) { Init(K, N, randMax, rand); };

	~KMean();

	UINT32 GetTrainSize() const;
	UINT32 GetCenterSize() const;
	UINT32 GetDimension() const;
	void PrintTrainDatas() const;
	void PrintCenterDatas() const;

	void SetTrainDatas(std::vector<T*>& data);
	void SetCenterDatas(std::vector<T*>& data);
	UINT32 Predict(const T* data) const;
	void Train(C_UINT32 trainTimes);
	void Save(std::string path) ;
private:
	void Init(C_UINT32 K, C_UINT32 randMax, C_BOOL rand);
	void Init(C_UINT32 K, C_UINT32 N, C_UINT32 randMax, C_BOOL rand);
	void RandData(std::vector<T*>& data, C_UINT32 randMax);
	void RandDataD(std::vector<T*>& data, C_UINT32 randMax);
	void Print(const std::vector<T*>& data) const;
	void DeleteResource();

	UINT32 Update(const T** centerSum, C_UINT32* centerCount);
	void Add(const T* trainData, T* centerSum);
	double Distance(const T* trainData, const T* centerData) const;

	std::vector<T*> _trainDatas;
	std::vector<T*> _centerDatas;
	UINT32 _dimension = 0;
	bool _initCenter = false;
	bool _initTrain = false;
};

template <class T>
void KMean<T>::DeleteResource()
{
	if (_initCenter)
	{
		C_UINT32 centerSize = _centerDatas.size();
		for (UINT32 index = 0; index < centerSize; index++)
		{
			delete[] _centerDatas[index];
			_centerDatas[index] = nullptr;
		}
	}

	if (_initTrain)
	{
		C_UINT32 trainSize = _trainDatas.size();
		for (UINT32 index = 0; index < trainSize; index++)
		{
			delete[] _trainDatas[index];
			_trainDatas[index] = nullptr;
		}
	}
}

template <class T>
KMean<T>::~KMean()
{
	DeleteResource();
}

template <class T>
KMean<T>::KMean(const std::string& path)
{
	std::ifstream read(path, std::ios::in);
	if (read.is_open())
	{
		UINT32 size = 0;
		read >> size;
		read >> _dimension;
		_centerDatas.resize(size);
		for (UINT32 n = 0; n < size; n++)
		{
			_centerDatas[n] = new T[_dimension];
			for (UINT32 d = 0; d < _dimension; d++)
			{
				read >> _centerDatas[n][d];
			}
		}
		_initCenter = true;
	}
	read.close();
}

template <class T>
void KMean<T>::Print(const std::vector<T*>& data) const
{
	C_UINT32 size = data.size();
	for (UINT32 d = 0; d < _dimension; d++)
	{
		for (UINT32 n = 0; n < size; n++)
		{
			cout << data[n][d] << ", ";
		}
		cout << endl;
	}
}

template <class T>
void KMean<T>::PrintTrainDatas() const
{
	Print(_trainDatas);
}

template <class T>
void KMean<T>::PrintCenterDatas() const
{
	Print(_centerDatas);
}

template <class T>
void KMean<T>::RandData(std::vector<T*>& data, C_UINT32 randMax)
{
	srand(time(NULL));

	C_UINT32 size = data.size();
	for (UINT32 n = 0; n < size; n++)
	{
		for (UINT32 d = 0; d < _dimension; d++)
		{
			data[n][d] = (rand() % randMax);
		}
	}
}

template <class T>
void KMean<T>::RandDataD(std::vector<T*>& data, C_UINT32 randMax)
{
	srand(time(NULL));

	C_UINT32 size = data.size();
	for (UINT32 n = 0; n < size; n++)
	{
		for (UINT32 d = 0; d < _dimension; d++)
		{
			data[n][d] = (rand() % randMax) / static_cast<double>(randMax);
		}
	}
}

template <class T>
void KMean<T>::Init(C_UINT32 K, C_UINT32 randMax, C_BOOL rand)
{
	_centerDatas.resize(K);
	for (UINT32 index = 0; index < K; index++)
	{
		_centerDatas[index] = new T[_dimension];
	}
	if (rand)
	{
		RandDataD(_centerDatas, randMax);
	}
	else
	{
		RandData(_centerDatas, randMax);
	}
	_initCenter = true;
}

template <class T>
void KMean<T>::Init(C_UINT32 K, C_UINT32 N, C_UINT32 randMax, C_BOOL rand)
{
	Init(K, randMax);
	_trainDatas.resize(N);
	for (UINT32 index = 0; index < N; index++)
	{
		_trainDatas[index] = new T[_dimension];
	}
	if (rand)
	{
		RandDataD(_trainDatas, randMax);
	}
	else
	{
		RandData(_trainDatas, randMax);
	}
	_initTrain = true;
}

template <class T>
UINT32 KMean<T>::GetTrainSize() const
{
	return _trainDatas.size();
}

template <class T>
UINT32 KMean<T>::GetCenterSize() const
{
	return _centerDatas.size();
}

template <class T>
UINT32 KMean<T>::GetDimension() const
{
	return _dimension;
}

template <class T>
void KMean<T>::Save(std::string path)
{
	std::ofstream ofstream;
	ofstream.open(path, std::ios::out);
	C_UINT32 size = _centerDatas.size();

	ofstream << size << " " << _dimension << std::endl;
	for (UINT32 n = 0; n < size; n++)
	{
		for (UINT32 d = 0; d < _dimension; d++)
		{
			ofstream << _centerDatas[n][d] << " ";
		}
		ofstream << std::endl;
	}
	ofstream.close();
}

template <class T>
UINT32 KMean<T>::Update(const T** centerSum, C_UINT32* centerCount)
{
	UINT32 update = 0;
	C_UINT32 centerN = _centerDatas.size();
	for (UINT32 centerIndex = 0; centerIndex < centerN; centerIndex++)
	{
		C_UINT32 count = centerCount[centerIndex];
		if (count == 0)
		{
			continue;
		}
		for (UINT32 d = 0; d < _dimension; d++)
		{
			_centerDatas[centerIndex][d] = centerSum[centerIndex][d] / count;
		}
		update++;
	}
	return update;
}

template <class T>
void KMean<T>::Add(const T* trainData, T* centerSum)
{
	for (UINT32 index = 0; index < _dimension; index++)
	{
		centerSum[index] += trainData[index];
	}
}

template <class T>
double KMean<T>::Distance(const T* trainData, const T* centerData) const
{
	double sum = 0;
	for (UINT32 index = 0; index < _dimension; index++)
	{
		sum += pow(trainData[index] - centerData[index], 2);
	}
	return sqrt(sum);
}

template <class T>
void KMean<T>::SetTrainDatas(std::vector<T*>& data)
{
	_trainDatas = data;
}

template <class T>
void KMean<T>::SetCenterDatas(std::vector<T*>& data)
{
	_centerDatas = data;
}

template <class T>
UINT32 KMean<T>::Predict(const T* data) const
{
	C_UINT32 centerN = _centerDatas.size();
	double minValue = DBL_MAX;
	INT32 minIndex = 0;
	for (UINT32 centerIndex = 0; centerIndex < centerN; centerIndex++)
	{
		C_DOUBLE nowValue = Distance(data, _centerDatas[centerIndex]);
		if (minValue > nowValue)
		{
			minValue = nowValue;
			minIndex = centerIndex;
		}
	}
	return minIndex;
}

template <class T>
void KMean<T>::Train(C_UINT32 trainTimes)
{
	C_UINT32 trainN = _trainDatas.size();
	C_UINT32 centerN = _centerDatas.size();
	for (UINT32 time = 0; time < trainTimes; time++)
	{
		UINT32* centerCount = new UINT32[centerN]{ 0 };
		T** centerSum = new T*[centerN];
		for (UINT32 index = 0; index < centerN; index++)
		{
			centerSum[index] = new T[_dimension]{ 0 };
		}

		concurrency::parallel_for(0, static_cast<int>(trainN), [&](C_INT32& trainIndex)
		{
			UINT32 minIndex = Predict(_trainDatas[trainIndex]);
			Add(_trainDatas[trainIndex], centerSum[minIndex]);
			centerCount[minIndex]++;
		});

		C_UINT32 update = Update(const_cast<const T**>(centerSum), centerCount);

		for (UINT32 index = 0; index < centerN; index++)
		{
			delete[] centerSum[index];
			centerSum[index] = nullptr;
		}
		delete[] centerSum;
		centerSum = nullptr;

		delete[] centerCount;
		centerCount = nullptr;

		if ((update / static_cast<double>(centerN)) <= 0.0)
		{
			return;
		}
	}

}

#endif // !KMEAN_H
