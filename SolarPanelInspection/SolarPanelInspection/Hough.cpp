#include "stdafx.h"
#include "Hough.h"

namespace MNDT {
	//void HoughLines(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height
	//	, C_FLOAT rho, C_FLOAT theta, C_FLOAT threshold)
	//{
	//	C_FLOAT fixRho = 1.0f / rho;
	//	C_UINT32 thetaSize = static_cast<UINT32>(PI / theta);
	//	float* fixSin = new float[thetaSize];
	//	float* fixCos = new float[thetaSize];
	//
	//	for (UINT32 index = 0; index < thetaSize; index++)
	//	{
	//		fixSin[index] = FixValue(sin(theta * index)) * fixRho;
	//		fixCos[index] = FixValue(cos(theta * index)) * fixRho;
	//	}
	//
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	C_DOUBLE originalR = std::max((width + height) * sin(PI / 4.0), static_cast<double>(std::max(width, height)));
	//	//C_DOUBLE originalR = 14 * sin(PI / 4.0);
	//	C_UINT32 maxR = static_cast<UINT32>(originalR * fixRho) + 1 + 2;
	//	C_UINT32 xAxisOffset = maxR * (thetaSize + 2);
	//	UINT32* count = new UINT32[xAxisOffset * 2]{ 0 };
	//
	//	for (UINT32 row = 0; row < height; row++)
	//	{
	//		for (UINT32 col = 0; col < width; col++)
	//		{
	//			if (srcImage.image[row][col] > 0)
	//			{
	//				for (UINT32 index = 0; index < thetaSize; index++)
	//				{
	//					int32_t r = static_cast<int32_t>(fixSin[index] * row + fixCos[index] * col);
	//
	//					r = r < 0 ? abs(r) + xAxisOffset : r;
	//					count[maxR * (index + 1) + r + 1]++;
	//				}
	//			}
	//		}
	//	}
	//
	//
	//	//for (UINT32 index = 0; index < thetaSize; index++)
	//	//{
	//	//	int32_t r = static_cast<int32_t>(fixSin[index] * 7 + fixCos[index] * 7);
	//	//	UINT32 offset = r < 0 ? xAxisOffset : 0;
	//	//	count[offset + abs(r) + maxR * index]++;
	//
	//	//	r = static_cast<int32_t>(fixSin[index] * 3 + fixCos[index] * 3);
	//	//	offset = r < 0 ? xAxisOffset : 0;
	//	//	count[offset + abs(r) + maxR * index]++;
	//
	//	//	r = static_cast<int32_t>(fixSin[index] * 5 + fixCos[index] * 5);
	//	//	offset = r < 0 ? xAxisOffset : 0;
	//	//	count[offset + abs(r) + maxR * index]++;
	//
	//	//	r = static_cast<int32_t>(fixSin[index] * 1 + fixCos[index] * 1);
	//	//	offset = r < 0 ? xAxisOffset : 0;
	//	//	count[offset + abs(r) + maxR * index]++;
	//
	//	//}
	//
	//	//SetNormalizedHistogram8bit(count, maxR * thetaSize * 2, 255);
	//
	//	//HoughDrawPolar(pur
	//	//	, width, height
	//	//	, count
	//	//	, theta, maxR);
	//
	//	UINT32* findCount = new UINT32[xAxisOffset * 2]{ 0 };
	//	UINT32 findIndex = 0;
	//
	//	for (UINT32 index = 0; index < thetaSize; index++)
	//	{
	//		C_UINT32 offset = maxR * (index + 1);
	//		UINT32* nowCountPointer = nullptr;
	//
	//		for (UINT32 countIndex = offset + 1; countIndex < offset + maxR - 1; countIndex++)
	//		{
	//			// +
	//			nowCountPointer = count + countIndex;
	//			if (*nowCountPointer > threshold
	//				|| *nowCountPointer > *(nowCountPointer - 1)
	//				|| *nowCountPointer > *(nowCountPointer + 1)
	//				|| *nowCountPointer > *(nowCountPointer - maxR)
	//				|| *nowCountPointer > *(nowCountPointer + maxR))
	//			{
	//				findCount[findIndex++] = countIndex;
	//			}
	//
	//			// -
	//			nowCountPointer = count + countIndex + xAxisOffset;
	//
	//			if (*nowCountPointer > threshold
	//				|| *nowCountPointer > *(nowCountPointer - 1)
	//				|| *nowCountPointer > *(nowCountPointer + 1)
	//				|| *nowCountPointer > *(nowCountPointer - maxR)
	//				|| *nowCountPointer > *(nowCountPointer + maxR))
	//			{
	//				findCount[findIndex++] = countIndex + xAxisOffset;
	//			}
	//		}
	//	}
	//
	//	std::sort(findCount, findCount + findIndex
	//		, [&count](const UINT32& index1, const UINT32& index2) { return *(count + index1) > *(count + index2); });
	//
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);
	//
	//	for (UINT32 index = 0; index < findIndex; index++)
	//	{
	//		UINT32 countIndex = findCount[index] - 1;
	//		
	//		if (countIndex < xAxisOffset)
	//		{
	//			UINT32 thetaIndex = countIndex / maxR - 1;
	//			int32_t r = countIndex % maxR;
	//			float dangle = theta * thetaIndex;
	//			float drho =  r;
	//
	//			double a = cos(dangle), b = sin(dangle);
	//			double x0 = a*drho , y0 = b*drho ;
	//			int32_t x1 = x0 - 1*b;
	//			int32_t y1 = y0 + 1* a ;
	//			int32_t x2 = x0 + 1* b;
	//			int32_t y2 = y0 - 1* a ;
	//
	//			if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0
	//				|| x1 >= width || y1 >= height || x2 >= width || y2 >= height)
	//			{
	//				continue;
	//			}
	//			DrawLine8bit(purImage, Point(x1, y1), Point(x2, y2));
	//		}
	//		else
	//		{
	//			countIndex -= xAxisOffset;
	//			UINT32 thetaIndex = countIndex / maxR - 1;
	//			int32_t r = countIndex % maxR;
	//			float dangle = theta * thetaIndex;
	//			float drho = -r;
	//
	//			double a = cos(dangle), b = sin(dangle);
	//			double x0 = a*drho, y0 = b*drho;
	//			int32_t x1 = x0 - b;
	//			int32_t y1 = y0 + a;
	//			int32_t x2 = x0 + b;
	//			int32_t y2 = y0 - a;
	//
	//			if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0
	//				|| x1 >= width || y1 >= height || x2 >= width || y2 >= height)
	//			{
	//				continue;
	//			}
	//			DrawLine8bit(purImage, Point(x1, y1), Point(x2, y2));
	//		}
	//
	//		//int32_t r = static_cast<int32_t>(rho(fixSin[index] * row + fixCos[index] * col));
	//
	//		//r = r < 0 ? abs(r) + xAxisOffset : r;
	//		//count[maxR * (index + 1) + r + 1]++;
	//	}
	//
	//	//for (UINT32 row = 0; row < height; row++)
	//	//{
	//	//	for (UINT32 col = 0; col < width; col++)
	//	//	{
	//	//		if (srcImage.image[row][col] > 0)
	//	//		{
	//	//			for (UINT32 index = 0; index < thetaSize; index++)
	//	//			{
	//	//				int32_t r = static_cast<int32_t>(fixSin[index] * row + fixCos[index] * col);
	//
	//	//				r = r < 0 ? abs(r) + xAxisOffset : r;
	//	//				if (count[maxR * (index + 1) + r + 1] > 0)
	//	//				{
	//	//					purImage.image[row][col] = 150;
	//	//					break;
	//	//				}
	//	//			}
	//	//		}
	//	//	}
	//	//}
	//
	//	delete[] findCount;
	//	findCount = nullptr;
	//
	//	delete[] fixSin;
	//	fixSin = nullptr;
	//
	//	delete[] fixCos;
	//	fixCos = nullptr;
	//
	//	delete[] count;
	//	count = nullptr;
	//}
	void HoughLines(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_FLOAT rho, C_FLOAT theta, C_UINT32 threshold)
	{
		// 1. init params
		C_FLOAT fixRho = 1.0f / rho;
		C_UINT32 thetaSize = static_cast<UINT32>(PI / theta);
		float* fixSin = new float[thetaSize];
		float* fixCos = new float[thetaSize];

		for (UINT32 index = 0; index < thetaSize; index++)
		{
			fixSin[index] = FixValue(sin(theta * index)) * fixRho;
			fixCos[index] = FixValue(cos(theta * index)) * fixRho;
		}

		// 2. hough total
		Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
		C_DOUBLE originalR = std::max((width + height) * sin(PI / 4.0), static_cast<double>(std::max(width, height)));
		C_UINT32 maxR = static_cast<UINT32>(originalR * fixRho) + 1 + 2;
		C_UINT32 xAxisOffset = maxR * (thetaSize + 2);
		UINT32* count = new UINT32[xAxisOffset * 2]{ 0 };

		concurrency::parallel_for(0, static_cast<int>(height), [&](const int& row)
		{
			for (UINT32 col = 0; col < width; col++)
			{
				if (srcImage.image[row][col] > 0)
				{
					for (UINT32 index = 0; index < thetaSize; index++)
					{
						int32_t r = static_cast<int32_t>(fixSin[index] * row + fixCos[index] * col);

						r = r < 0 ? abs(r) + xAxisOffset : r;
						count[maxR * (index + 1) + r + 1]++;
					}
				}
			}
		});

		//// draw hough
		//Inner::DrawHoughPolar(pur
		//	, width, height
		//	, count
		//	, theta, maxR);

		// 3. update neighbours
		Inner::HoughLineNeighboursUpdate(count
			, thetaSize, maxR
			, threshold);


		// 4. draw line
		Image purImage(pur, width, height, ImageType::GRAY_8BIT);
		concurrency::parallel_for(0, static_cast<int>(height), [&](const int& row)
		{
			for (UINT32 col = 0; col < width; col++)
			{
				//if (srcImage.image[row][col] > 0)
				//{
					for (UINT32 index = 0; index < thetaSize; index++)
					{
						int32_t r = static_cast<int32_t>(fixSin[index] * row + fixCos[index] * col);

						r = r < 0 ? abs(r) + xAxisOffset : r;
						if (count[maxR * (index + 1) + r + 1] > 0)
						{
							purImage.image[row][col] = 255;
						}
					}
				//}
			}
		});

		delete[] fixSin;
		fixSin = nullptr;

		delete[] fixCos;
		fixCos = nullptr;

		delete[] count;
		count = nullptr;
	}

	void Inner::HoughLineNeighboursUpdate(UINT32* count
		, C_UINT32 thetaSize, C_UINT32 maxR
		, C_UINT32 threshold)
	{
		C_UINT32 xAxisOffset = maxR * (thetaSize + 2);
		UINT32* originalcount = new UINT32[xAxisOffset * 2];
		memcpy(originalcount, count, sizeof(UINT32) * xAxisOffset * 2);

		for (UINT32 index = 0; index < thetaSize; index++)
		{
			C_UINT32 offset = maxR * (index + 1);
			concurrency::parallel_for(static_cast<int>(offset + 1), static_cast<int>(offset + maxR - 1), [&](const int& countIndex)
			{
				// x axis -> + and -
				for (UINT32 axis = 0; axis < 2; axis++)
				{
					C_UINT32 offset = countIndex + xAxisOffset * axis;
					C_UINT32* nowCountPointer = originalcount + offset;
					if (*nowCountPointer < threshold
						|| *nowCountPointer < *(nowCountPointer - 1)
						|| *nowCountPointer < *(nowCountPointer + 1)
						|| *nowCountPointer < *(nowCountPointer - maxR)
						|| *nowCountPointer < *(nowCountPointer + maxR))
					{
						*(count + offset) = 0;
					}
				}
			});
		}

		delete[] originalcount;
		originalcount = nullptr;
	}

}