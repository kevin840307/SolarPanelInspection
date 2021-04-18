#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H
#include "general.h"
#include "Image.h"
#include "Bitmap.h"
#include "Rect.h"
#include "draw.h"
#include <sstream>

enum ColerType
{
	BGR2GRAY_8BIT,
	BGR2HSV,
	HSV2BGR,
	BGR2YCbCr,
	YCbCr2BGR,
};

namespace MNDT {
	/*
		ChangeColor Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		type		= change type
	*/
	void ChangeColor(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, const ColerType type);

	/*
		ImagePadding8bit Parameter:
		src		= source of image
		pur		= purpose of image
		width		= Image width
		height		= Image height
		pad		= padding size
	*/
	void ImagePadding8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_INT32 pad);

	/*
		OriginalPadding8bit Parameter:
		src		= source of image
		pur		= purpose of image
		width		= Image width
		height		= Image height
		pad		= padding size
	*/
	void OriginalPadding8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_INT32 pad);

	/*
		Sobel8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		dx			= x gradient switch
		dy			= y gradient switch
	*/
	void Sobel8bit(C_UCHAR* src, int32_t* pur
		, C_UINT32 width, C_UINT32 height
		, C_BOOL dx, C_BOOL dy);

	/*
		SobelEdgeView8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
	*/
	void SobelEdgeView8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height);

	/*
		SobelScharr8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		dx			= x gradient switch
		dy			= y gradient switch
	*/
	void Scharr8bit(C_UCHAR* src, int32_t* pur
		, C_UINT32 width, C_UINT32 height
		, C_BOOL dx, C_BOOL dy);

	/*
		ScharrEdgeView8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
	*/
	void ScharrEdgeView8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height);

	/*
		Gamma8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		gamma		= calculation parmas
	*/
	void Gamma8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_DOUBLE gamma);

	/*
		Threshold8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		thresh		= threshold
	*/
	void Threshold8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 thresh);

	/*
		BlurGauss8bit Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		size		= gauss temp size
		sigma		= gauss temp sigma
	*/
	void BlurGauss8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 size, C_FLOAT sigma);

	void Screenshot8bit(const Image& srcImage, UCHAR* pur
		, const Rect& rect);

	void Screenshot24bit(const Image& srcImage, Image& purImage
		, const Rect& rect);

	void ContourFrame8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height);

	void ContourFrame8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 fileIndex);

	std::vector<Rect> ContourFrame8bit(C_UCHAR* src
		, C_UINT32 width, C_UINT32 height);

	void ThresholdingOTSU8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height);

	void Expansion8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 block);

	void Erosion8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 block);
}

namespace MNDT {
	namespace Inner {
		// tool start
		void Gaussian2DTemp(float** const temp, C_INT32 size, C_FLOAT sigma);

		void SetSobelKernels(int32_t* kernels
			, C_BOOL dx, C_BOOL dy);

		void SetScharrKernels(int32_t* kernels
			, C_BOOL dx, C_BOOL dy);
		// tool end

		// change color start
		void BGR2Gray8Bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height);
		// change color end

		UCHAR GetThresholdingScale(C_UCHAR* src
			, C_UINT32 width, C_UINT32 height);

		UCHAR GetMorphologyValue(const Image& srcImage
			, C_UINT32 blockRow, C_UINT32 blockCol, C_UINT32 blockSize
			, C_UCHAR threshold);
	}
}
namespace MNDT {
	template <class T>
	void Screenshot8bit(const Image& srcImage, T* pur
		, const Rect& rect)
	{
		concurrency::parallel_for(rect.Y(), static_cast<int>(rect.EndY() + 1), [&](const int& row)
		{
			C_UINT32 rowIndex = (row - rect.Y()) * rect.Width();
			for (UINT32 col = rect.X(); col <= rect.EndX(); col++)
			{
				*(pur + rowIndex + col - rect.X()) = srcImage.image[row][col];
			}
		});
	}

	template <class T>
	void L1Normalized8bit(T* histogram
		, C_UINT32 size)
	{
		double max = 0.0000000001;

		for (UINT32 index = 0; index < size; index++)
		{
			max = max < histogram[index] ? histogram[index] : max;
		}


		for (UINT32 index = 0; index < size; index++)
		{
			histogram[index] /= max;
		}
	}

	template <class T>
	void L1Normalized8bit(T* histogram, const T& base
		, C_UINT32 size)
	{
		for (UINT32 index = 0; index < size; index++)
		{
			histogram[index] /= base;
		}
	}
}
#endif