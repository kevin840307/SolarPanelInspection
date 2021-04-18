#pragma once
#ifndef GENERAL_H
#define GENERAL_H
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include <ppl.h>
//#define NDEBUG
#include <assert.h>

typedef unsigned char UCHAR;
typedef const unsigned char C_UCHAR;
typedef const char C_CHAR;
typedef unsigned __int32 UINT32;
typedef const unsigned __int32 C_UINT32;
typedef __int32 INT32;
typedef const __int32 C_INT32;
typedef const __int32 C_INT32;
typedef const double C_DOUBLE;
typedef const float C_FLOAT;
typedef const bool C_BOOL;

namespace AdaBoostType {
	typedef std::vector<double> DATA;
	typedef const std::vector<double> C_DATA;
	typedef std::vector<std::vector<double>> DATAS;
	typedef const std::vector<std::vector<double>> C_DATAS;
	typedef __int32 LABEL;
	typedef const __int32 C_LABEL;
	typedef std::vector<__int32> LABELS;
	typedef const std::vector<__int32> C_LABELS;

}

namespace MNDT {
	enum ImageType
	{
		BGR_24BIT = 3,
		GRAY_8BIT = 1
	};

	enum RotateType
	{
		HORIZONTAL,
		VERTICAL
	};

	enum ResizeType
	{
		NEAREST,
		LINEAR
	};

	enum Normalized
	{
		L1,
		L2
	};

	template <typename T>
	inline void Write(T& data)
	{
		std::fstream fwLog;

		fwLog.open(MNDT::LOG_FILE, std::ios::app);
		fwLog << data;
		fwLog.close();
	}

	inline float FixValue(C_FLOAT& value)
	{
		return abs(value) < 0.00000001f ? 0 : value;
	}

	inline double FixValue(C_DOUBLE& value)
	{
		return abs(value) < 0.00000001 ? 0 : value;
	}

	extern char* LOG_FILE;
	extern double PI;
}
#endif // !GENERAL_H
