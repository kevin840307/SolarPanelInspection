#include"stdafx.h"
#include "HOG.h"

HOG::~HOG()
{

}

void HOG::Gradient(C_UCHAR* src
	, C_UINT32 width, C_UINT32 height
	, double* amplitudes, double* angles)
{
	// 1. padding
	C_DOUBLE angle = 180.0 / MNDT::PI;
	C_UINT32 padWidth = width + 2;
	C_UINT32 padHeight = height + 2;
	UCHAR* padData = new UCHAR[padWidth * padHeight];

	MNDT::ImagePadding8bit(src, padData, width, height, 1);

	Image padImage(padData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);

	// 2. calculate hog of gradient
	concurrency::parallel_for(1, static_cast<int>(padHeight - 1), [&](const int& row)
	{
		C_UINT32 rowIndex = width * (row - 1);
		for (UINT32 col = 1; col < padWidth - 1; col++)
		{
			C_UINT32 nowIndex = rowIndex + (col - 1);

			C_FLOAT Gx = static_cast<float>(padImage.image[row][col + 1])
				- static_cast<float>(padImage.image[row][col - 1]);

			C_FLOAT Gy = static_cast<float>(padImage.image[row + 1][col])
				- static_cast<float>(padImage.image[row - 1][col]);

			*(angles + nowIndex) = abs(atan2(Gy, Gx) * angle);
			*(amplitudes + nowIndex) = sqrt(Gx * Gx + Gy * Gy);
		}

	});
	delete[] padData;
	padData = nullptr;
}

void HOG::GradienView(C_UCHAR* src, UCHAR* pur
	, C_UINT32 width, C_UINT32 height)
{
	C_UINT32 size = width * height;
	double* amplitudes = new double[size];
	double* angles = new double[size];

	Gradient(src, width, height
		, amplitudes, angles);

	delete[] angles;
	angles = nullptr;

	concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
	{
		*(pur + index) = static_cast<UCHAR>(*(amplitudes + index));
	});

	delete[] amplitudes;
	amplitudes = nullptr;
}
