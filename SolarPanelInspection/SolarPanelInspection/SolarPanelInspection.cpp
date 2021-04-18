// SolarPanelInspection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Library.h"
#include <iostream>
#include <string>
#include "HOG.h"
#include "Hough.h"
#include "KMean.h"

std::string _dataPath = "D:\\Downloads\\secret_plan.txt";
std::string _screenshotPath = "D:\\Downloads\\Data\\";
std::string _screenshotDataPath = "D:\\Downloads\\secret_plan2.txt";
std::string _stainDataPath = "D:\\Downloads\\SData\\";

inline void Copy(C_UCHAR* src, UCHAR* pur, C_UINT32 size)
{
	memcpy(pur, src, sizeof(UCHAR) * size);
}

inline Pixel GetColor()
{
	return Pixel(rand() % 255, rand() % 255, rand() % 255);
}

void MarkLine(Image& purImage)
{
	C_UINT32 width = purImage.Width();
	C_UINT32 height = purImage.Height();
	UINT32 pad = 10;
	C_UINT32 splitHSize = height / 6;
	for (UINT32 index = 0; index < 5; index++)
	{
		UINT32 nowHeight = (index + 1) * splitHSize;
		MNDT::DrawLine24bit(purImage
			, Point(0, nowHeight + pad), Point(width - 1, nowHeight + pad), Pixel(255, 0, 0));
		MNDT::DrawLine24bit(purImage
			, Point(0, nowHeight - pad), Point(width - 1, nowHeight - pad), Pixel(255, 0, 0));
	}

	C_UINT32 splitWSize = width / 10;
	for (UINT32 index = 0; index < 9; index++)
	{
		UINT32 nowWedith = (index + 1) * splitWSize;
		MNDT::DrawLine24bit(purImage
			, Point(nowWedith + pad, 0), Point(nowWedith + pad, height - 1), Pixel(255, 0, 0));
		MNDT::DrawLine24bit(purImage
			, Point(nowWedith - pad, 0), Point(nowWedith - pad, height - 1), Pixel(255, 0, 0));
	}
}

void Save(const Image& srcImage, const Rect& rect, std::string path)
{
	UCHAR* rectData = new UCHAR[rect.Width() * rect.Height() * 3];
	Image purImage(rectData, rect.Width(), rect.Height(), MNDT::ImageType::BGR_24BIT);
	MNDT::Screenshot24bit(srcImage, purImage, rect);

	//MarkLine(purImage, rect.Width(), rect.Height());

	Bitmap bit(rectData, rect.Width(), rect.Height(), MNDT::ImageType::BGR_24BIT);
	bit.Save(path.c_str());

	//std::ofstream ofstream;
	//ofstream.open(_screenshotDataPath, std::ios::out | std::ios::app);
	//ofstream << path << std::endl;
	//ofstream.close();

	delete[] rectData;
	rectData = nullptr;

}

std::string GetScreenshotPath(C_UINT32 index)
{
	std::string path = "";
	std::stringstream stream;
	stream << index;
	stream >> path;
	path = _screenshotPath + path + ".bmp";
	return path;
}

std::string GetStainPath(C_UINT32 index)
{
	std::string path = "";
	std::stringstream stream;
	stream << index;
	stream >> path;
	path = _stainDataPath + path + ".bmp";
	return path;
}

Rect GetRealRect(const std::vector<Rect>& rects)
{
	for (UINT32 index = 0; index < rects.size(); index++)
	{
		const Rect& rect = rects[index];
		if (rect.Width() > 3000 && rect.Height() > 2000
			&& rect.Width() < 3800)
		{
			return rect;
		}
	}
}

void Expansion8bit(UCHAR* scr8bitData, C_UINT32 width, C_UINT32 height, C_UINT32 time)
{
	C_UINT32 size = width * height;
	UCHAR* pur8bitData = new UCHAR[size];
	for (UINT32 index = 0; index < time; index++)
	{
		MNDT::Expansion8bit(scr8bitData, pur8bitData
			, width, height, 3);
		Copy(pur8bitData, scr8bitData, size);
	}
	delete[] pur8bitData;
	pur8bitData = nullptr;
}

void Erosion8bit(UCHAR* scr8bitData, C_UINT32 width, C_UINT32 height, C_UINT32 time)
{
	C_UINT32 size = width * height;
	UCHAR* pur8bitData = new UCHAR[size];
	for (UINT32 index = 0; index < time; index++)
	{
		MNDT::Erosion8bit(scr8bitData, pur8bitData
			, width, height, 3);
		Copy(pur8bitData, scr8bitData, size);
	}
	delete[] pur8bitData;
	pur8bitData = nullptr;
}

void Closure8bit(UCHAR* pur8bitData, C_UINT32 width, C_UINT32 height, C_UINT32 time)
{
	Expansion8bit(pur8bitData, width, height, time);
	Erosion8bit(pur8bitData, width, height, time);
}

void Disconnect8bit(UCHAR* scr8bitData, C_UINT32 width, C_UINT32 height, C_UINT32 time)
{
	Erosion8bit(scr8bitData, width, height, time);
	Expansion8bit(scr8bitData, width, height, time);
}

void OR(C_UCHAR* src8bitData, UCHAR* pur8bitData, C_UINT32 size)
{
	C_UCHAR* srcEnd = src8bitData + size;

	while (src8bitData != srcEnd)
	{
		*pur8bitData = (*pur8bitData | *src8bitData);
		pur8bitData++;
		src8bitData++;
	}
}

void AND(C_UCHAR* src8bitData, UCHAR* pur8bitData, C_UINT32 size)
{
	C_UCHAR* srcEnd = src8bitData + size;

	while (src8bitData != srcEnd)
	{
		*pur8bitData = (*pur8bitData & *src8bitData);
		pur8bitData++;
		src8bitData++;
	}
}

void Mask(C_UCHAR* mask, UCHAR* pur, C_UINT32 size)
{
	for (UINT32 index = 0; index < size; index++)
	{
		if (*(mask + index) == 255
			&& *(pur + index) != 255)
		{
			*(pur + index) = 255;
		}
		else
		{
			*(pur + index) = 0;
		}
	}
}

void Mask(const Image& src, Image& pur, const Pixel pix = Pixel(255, 255, 255))
{
	C_UINT32 wedth = pur.Width();
	C_UINT32 height = pur.Height();
	for (UINT32 row = 0; row < height; row++)
	{
		for (UINT32 col = 0; col < wedth; col++)
		{
			if ((src.image[row][col] == 255))
			{
				pur.SetPixel(row, col, pix);
			}
		}
	}
}

UINT32 Diff(const Pixel& pix1, const Pixel& pix2)
{
	return abs(static_cast<__int32>(pix1.B) - static_cast<__int32>(pix2.B));
}

void SetHoughLines(UCHAR* scr8bitData
	, C_UINT32 width, C_UINT32 height
	, C_UINT32 minSize = 1300, C_BOOL expamsion = true)
{
	C_UINT32 size = width * height;
	UCHAR* pur8bitData = new UCHAR[size]{ 0 };

	// 1.
	MNDT::HoughLines(scr8bitData, pur8bitData
		, width, height
		, 0.335f, (float)(MNDT::PI / 10), minSize);
	Copy(pur8bitData, scr8bitData, size);

	// 2.
	if (expamsion)
	{
		Closure8bit(scr8bitData, width, height, 3);
		Expansion8bit(scr8bitData, width, height, 3);
	}

	delete[] pur8bitData;
	pur8bitData = nullptr;
}

void SetOriginalEdge(const Bitmap& srcBitmap, UCHAR* pur8bitData, C_UINT32 times)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();
	C_UINT32 size = width * height;
	UCHAR* scr8bitData = new UCHAR[size]{ 0 };

	// 1.
	MNDT::ChangeColor(srcBitmap.data, pur8bitData
		, width, height
		, ColerType::BGR2GRAY_8BIT);
	Copy(pur8bitData, scr8bitData, size);

	// 2.
	for (UINT32 index = 0; index < times; index++)
	{
		MNDT::BlurGauss8bit(scr8bitData, pur8bitData
			, width, height
			, 3, 1);
		Copy(pur8bitData, scr8bitData, size);
	}

	// 3.
	MNDT::SobelEdgeView8bit(scr8bitData, pur8bitData
		, width, height);
	Copy(pur8bitData, scr8bitData, size);

	// 4.
	MNDT::Threshold8bit(scr8bitData, pur8bitData
		, width, height, 10);
	Copy(pur8bitData, scr8bitData, size);

	// 5.
	Closure8bit(pur8bitData, width, height, times);

	delete[] scr8bitData;
	scr8bitData = nullptr;
}

void SetHOGEdge(const Bitmap& srcBitmap, UCHAR* pur8bitData, C_UINT32 times)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();
	C_UINT32 size = width * height;
	UCHAR* scr8bitData = new UCHAR[size]{ 0 };

	// 1.
	MNDT::ChangeColor(srcBitmap.data, pur8bitData
		, width, height
		, ColerType::BGR2GRAY_8BIT);
	Copy(pur8bitData, scr8bitData, size);

	// 2.

	MNDT::BlurGauss8bit(scr8bitData, pur8bitData
		, width, height
		, 3, 1);
	Copy(pur8bitData, scr8bitData, size);


	// 3.
	HOG hog;
	hog.GradienView(scr8bitData, pur8bitData
		, width, height);
	Copy(pur8bitData, scr8bitData, size);

	// 4.
	MNDT::Threshold8bit(scr8bitData, pur8bitData
		, width, height, 10);

	// 5.
	Closure8bit(pur8bitData, width, height, times);

	delete[] scr8bitData;
	scr8bitData = nullptr;
}



// mark start
void FindCenter(UCHAR* pur8bitData, C_UINT32 width, C_UINT32 height)
{
	C_UINT32 pad = 47;
	C_UINT32 padWidth = width + (pad << 1);
	C_UINT32 padHeight = height + (pad << 1);
	UCHAR* padData = new UCHAR[padWidth *padHeight];
	MNDT::ImagePadding8bit(pur8bitData, padData, width, height, pad);

	Image padImage(padData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);
	Image nowImage(pur8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	memset(pur8bitData, 0, sizeof(UCHAR) * width * height);
	for (UINT32 row = pad; row < padHeight - pad; row++)
	{
		for (UINT32 col = pad; col < padWidth - pad; col++)
		{
			if ((padImage.image[row][col] == 255)
				&& (padImage.image[row - pad][col] == 255)
				&& (padImage.image[row + pad][col] == 255))
			{
				nowImage.image[row - pad][col - pad] = 255;
			}
		}
	}

	delete[] padData;
	padData = nullptr;
}

void FindDoublePoint(UCHAR* pur8bitData, C_UINT32 width, C_UINT32 height)
{

	C_UINT32 pad = 30;
	C_UINT32 padWidth = width + (pad << 1);
	C_UINT32 padHeight = height + (pad << 1);
	UCHAR* padData = new UCHAR[padWidth *padHeight]{ 255 };

	MNDT::ImagePadding8bit(pur8bitData, padData, width, height, pad);

	Image padImage(padData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);
	Image nowImage(pur8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	memset(pur8bitData, 0, sizeof(UCHAR) * width * height);
	for (UINT32 row = pad; row < padHeight - pad; row++)
	{
		for (UINT32 col = pad; col < padWidth - pad; col++)
		{
			if ((padImage.image[row][col] == 255)
				&& (padImage.image[row][col - pad] == 255)
				&& (padImage.image[row][col + pad] == 255)
				&& (padImage.image[row - pad][col] == 255)
				&& (padImage.image[row + pad][col] == 255))
			{
				nowImage.image[row - pad][col - pad] = 255;
			}
		}
	}
	delete[] padData;
	padData = nullptr;
}

void CalcMaskCenter(const Image& originalImage, Image& pur
	, C_UINT32 minRow, C_UINT32 minCol, C_UINT32 pad
	, const Pixel pix = Pixel(255, 255, 255))
{
	const Pixel nowPix = originalImage.GetPixel(minRow, minCol);
	C_UINT32 initRow = minRow < pad ? 0 : minRow - pad;
	C_UINT32 initCol = minCol < pad ? 0 : minCol - pad;
	C_UINT32 maxRow = (minRow + pad + 10) > pur.Height() ? pur.Height() : (minRow + pad + 10);
	C_UINT32 maxCol = (minCol + pad + 10) > pur.Width() ? pur.Width() : (minCol + pad + 10);
	for (UINT32 row = initRow; row < maxRow; row++)
	{
		for (UINT32 col = initCol; col < maxCol; col++)
		{
			if (Diff(nowPix, originalImage.GetPixel(row, col)) < 2)
			{
				pur.SetPixel(row, col, pix);
			}
		}
	}
}

void FixCenterMask(const Image& originalImage, UCHAR* pur8bitData
	, C_UINT32 width, C_UINT32 height
	, const Pixel pix = Pixel(255, 255, 255))
{
	C_UINT32 pad = 28;
	C_UINT32 padWidth = width + (pad << 1);
	C_UINT32 padHeight = height + (pad << 1);
	UCHAR* srcPadData = new UCHAR[padWidth * padHeight]{ 0 };
	MNDT::ImagePadding8bit(pur8bitData, srcPadData, width, height, pad);

	Image srcPadImage(srcPadData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);
	Image purImage(pur8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	memset(pur8bitData, 0, sizeof(UCHAR) * width * height);
	for (UINT32 row = pad; row < padHeight - pad; row++)
	{
		for (UINT32 col = pad; col < padWidth - pad; col++)
		{
			if ((srcPadImage.image[row][col] == 255))
			{
				CalcMaskCenter(originalImage, purImage
					, row - pad, col - pad, pad, pix);
				col += 300;
			}
		}
	}
	Closure8bit(pur8bitData, width, height, 1);
	Expansion8bit(pur8bitData, width, height, 2);

	delete[] srcPadData;
	srcPadData = nullptr;
}

void FindPointProceess(const Bitmap& srcBitmap, UCHAR* pur8bitData)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();

	// 1.
	SetHOGEdge(srcBitmap, pur8bitData, 3);

	// 2.
	SetHoughLines(pur8bitData, width, height);

	// 3.
	FindDoublePoint(pur8bitData, width, height);

	// 4.
	FindCenter(pur8bitData, width, height);

	// 5.
	Disconnect8bit(pur8bitData, width, height, 3);

	// 6.
	Image originalImage(srcBitmap.data, width, height, MNDT::ImageType::BGR_24BIT);
	FixCenterMask(originalImage, pur8bitData
		, width, height
		, Pixel(255, 255, 255));
}

void FindLineProceess(const Bitmap& srcBitmap, UCHAR* pur8bitData)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();

	// 1.
	SetHOGEdge(srcBitmap, pur8bitData, 3);
	Expansion8bit(pur8bitData, width, height, 3);

	// 2.
	SetHoughLines(pur8bitData, width, height);
}

void FindLineEdge(const Bitmap& srcBitmap, UCHAR* pur8bitData)
{
	// 1.
	SetOriginalEdge(srcBitmap, pur8bitData, 3);
}

void StainProcess(const Bitmap& srcBitmap, const std::string& path, C_UINT32 type)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();
	C_UINT32 size = width * height;

	// 1.
	UCHAR* point8bitData = new UCHAR[size]{ 0 };
	FindPointProceess(srcBitmap, point8bitData);

	// 2.
	UCHAR* line8bitData = new UCHAR[size]{ 0 };
	FindLineProceess(srcBitmap, line8bitData);

	// 3.
	UCHAR* edge8bitData = new UCHAR[size]{ 0 };
	FindLineEdge(srcBitmap, edge8bitData);

	// 4.
	UCHAR* pur8bitData = new UCHAR[size]{ 0 };
	Copy(line8bitData, pur8bitData, size);
	OR(point8bitData, pur8bitData, size);

	// 5.
	Mask(edge8bitData, pur8bitData, size);

	// 6.
	Image originalImage(srcBitmap.data, width, height, MNDT::ImageType::BGR_24BIT);

	if (type & 1)
	{
		Image lineImage(line8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
		Mask(lineImage, originalImage, Pixel(255, 50, 255));
	}

	if (type & 2)
	{
		Image pointImage(point8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
		Mask(pointImage, originalImage, Pixel(0, 255, 255));
	}

	if (type & 4)
	{
		Image purImage(pur8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
		Mask(purImage, originalImage, Pixel(0, 0, 255));
	}
	// 7.
	//std::vector<Rect> rects = MNDT::ContourFrame8bit(pur8bitData
	//	, width, height);
	//memset(pur8bitData, 0, size);
	//for (UINT32 index = 1; index < rects.size(); index++)
	//{
	//	const Rect& rect = rects[index];
	//	if (rect.Width() * rect.Height() > 100)
	//	{
	//		MNDT::DrawRect8bit(purImage, rect, 255);
	//	}
	//}

	srcBitmap.Save(path.c_str());

	delete[] point8bitData;
	point8bitData = nullptr;

	delete[] line8bitData;
	line8bitData = nullptr;

	delete[] edge8bitData;
	edge8bitData = nullptr;

	delete[] pur8bitData;
	pur8bitData = nullptr;
}

void StainProcess(const Bitmap& srcBitmap, C_UINT32 fileIndex, C_UINT32 type)
{
	StainProcess(srcBitmap, GetStainPath(fileIndex), type);
}

void LoadScreenshotData()
{
	std::ifstream file;
	std::string imgPath;

	file.open(_screenshotDataPath, std::ios::in);

	UINT32 index = 0;
	while (std::getline(file, imgPath))
	{
		const Bitmap bitmap(imgPath);
		StainProcess(bitmap, index, 7);
		index++;
	}
}
// mark end


// screenshot start
void ScreenshotProcess(const Bitmap& srcBitmap, const std::string& path)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();
	C_UINT32 size = width * height;
	UCHAR* scr8bitData = new UCHAR[size]{ 0 };
	UCHAR* pur8bitData = new UCHAR[size]{ 0 };

	// 1.
	MNDT::ChangeColor(srcBitmap.data, pur8bitData
		, width, height
		, ColerType::BGR2GRAY_8BIT);
	Copy(pur8bitData, scr8bitData, size);

	// 2.
	MNDT::Gamma8bit(scr8bitData, pur8bitData
		, width, height, 1.5);
	Copy(pur8bitData, scr8bitData, size);

	// 3.
	MNDT::ThresholdingOTSU8bit(scr8bitData, pur8bitData
		, width, height);
	Copy(pur8bitData, scr8bitData, size);

	// 4.
	//C_UINT32 time = 1;
	//for (UINT32 index = 0; index < time; index++)
	//{
	//	MNDT::Expansion8bit(scr8bitData, pur8bitData
	//		, width, height, 3);
	//	Copy(pur8bitData, scr8bitData, size);
	//}

	//for (UINT32 index = 0; index < time; index++)
	//{
	//	MNDT::Erosion8bit(scr8bitData, pur8bitData
	//		, width, height, 3);
	//	Copy(pur8bitData, scr8bitData, size);
	//}


	//5.
	MNDT::SobelEdgeView8bit(scr8bitData, pur8bitData
		, width, height);
	Copy(pur8bitData, scr8bitData, size);

	//6.
	MNDT::Threshold8bit(scr8bitData, pur8bitData
		, width, height, 20);
	Copy(pur8bitData, scr8bitData, size);
	memset(pur8bitData, 0, sizeof(UCHAR) * size);

	//7.
	//MNDT::ContourFrame8bit(scr8bitData, pur8bitData
	//	, width, height
	//	, fileIndex);
	const std::vector<Rect> rects = MNDT::ContourFrame8bit(scr8bitData
		, width, height);
	const Rect& rect = GetRealRect(rects);
	const Image srcImage(srcBitmap.data
		, width, height
		, MNDT::ImageType::BGR_24BIT);
	Save(srcImage, rect, path);
	//8.

	//Bitmap purBitmap(pur8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	//purBitmap.Save("D://Data//pur.bmp");

	delete[] pur8bitData;
	pur8bitData = nullptr;

	delete[] scr8bitData;
	scr8bitData = nullptr;
}

void ScreenshotProcess(const Bitmap& srcBitmap, C_UINT32 fileIndex)
{
	ScreenshotProcess(srcBitmap, GetScreenshotPath(fileIndex));
}

void LoadData()
{
	std::ifstream file;
	std::string imgPath;

	file.open(_dataPath, std::ios::in);

	UINT32 index = 0;
	while (std::getline(file, imgPath))
	{
		const Bitmap bitmap(imgPath);
		ScreenshotProcess(bitmap, index);
		index++;
	}
}
// screenshot end


// k-mean train start
void SetFeature(const Image& purImage, double* pur
	, C_UINT32 row, C_UINT32 col, C_UINT32 block)
{
	for (UINT32 index = col; index < col + block; index++)
	{
		*pur = purImage.image[row][index];
		pur++;
	}

	for (UINT32 index = col; index < col + block; index++)
	{
		*pur = purImage.image[row + block - 1][index];
		pur++;
	}

	for (UINT32 index = row + 1; index < row + block - 1; index++)
	{
		*pur = purImage.image[index][col];
		pur++;
	}

	for (UINT32 index = row + 1; index < row + block - 1; index++)
	{
		*pur = purImage.image[index][col + block - 1];
		pur++;
	}

	C_UINT32 pad = (block >> 1);
	*pur = purImage.image[row + pad][col + pad];
}

std::vector<double*> GetKMeanTrainData(UCHAR* gray8bitData, UCHAR* line8bitData
	, C_UINT32 width, C_UINT32 height, C_UINT32 block)
{

	C_UINT32 pad = (block >> 1);
	C_UINT32 size = (width - (pad << 1)) * (height - (pad << 1));
	C_UINT32 fetureSize = block * block;
	C_UINT32 histogramSize = fetureSize * 2;
	std::vector<double*> trainDatas(size);
	Image grayImage(gray8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	Image lineImage(line8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	concurrency::parallel_for(static_cast<int>(pad), static_cast<int>(height - pad), [&](C_INT32& row)
	{
		C_UINT32 rowIndex = (row - pad) * (width - (pad << 1));
		for (UINT32 col = pad; col < width - pad; col++)
		{
			Rect rect(col - pad, row - pad, block, block);
			double* pur = new double[histogramSize] {0};
			double* purPointer = pur;

			MNDT::Screenshot8bit(grayImage, purPointer, rect);
			MNDT::L1Normalized8bit(purPointer, fetureSize);
			purPointer += fetureSize;

			MNDT::Screenshot8bit(lineImage, purPointer, rect);
			MNDT::L1Normalized8bit(purPointer, fetureSize);

			trainDatas[rowIndex + col - pad] = pur;
		}
	});

	return trainDatas;
}


void KMeanTrainProcess(const Bitmap& srcBitmap
	, C_UINT32 K, C_UINT32 block, C_UINT32 times
	, const std::string& savePath)
{
	C_UINT32 width = srcBitmap.Width();
	C_UINT32 height = srcBitmap.Height();
	C_UINT32 size = width * height;

	// 1.
	UCHAR* gray8bitData = new UCHAR[size]{ 0 };
	MNDT::ChangeColor(srcBitmap.data, gray8bitData
		, width, height
		, ColerType::BGR2GRAY_8BIT);

	// 2.
	UCHAR* line8bitData = new UCHAR[size]{ 0 };
	SetHOGEdge(srcBitmap, line8bitData, 3);
	SetHoughLines(line8bitData, width, height, 270, false);

	// 3.
	std::vector<double*> trainDatas = GetKMeanTrainData(gray8bitData, line8bitData
		, width, height, block);

	delete[] line8bitData;
	line8bitData = nullptr;

	delete[] gray8bitData;
	gray8bitData = nullptr;

	// 3.
	KMean<double> kmean(K, block * block * 2, 100, true);
	kmean.SetTrainDatas(trainDatas);
	kmean.Train(times);
	kmean.Save(savePath);

	for (UINT32 index = 0; index < trainDatas.size(); index++)
	{
		delete[] trainDatas[index];
		trainDatas[index] = nullptr;
	}
}

void LoadKMeanTrainData(const std::string& loadPath, const std::string& savePath
	, C_UINT32 K, C_UINT32 block, C_UINT32 times)
{
	Bitmap bitmap(loadPath);
	KMeanTrainProcess(bitmap, K, block, times, savePath);
}
// k-mean train end



// k-mean predict start
void MeanPredictProcess(const KMean<double>& kmean, Bitmap& bitmap, const std::string& savePath, C_UINT32 showFeature)
{
	C_UINT32 width = bitmap.Width();
	C_UINT32 height = bitmap.Height();
	C_UINT32 size = width * height;
	C_UINT32 block = static_cast<UINT32>(sqrt(kmean.GetDimension() / 2));
	C_UINT32 pad = (block >> 1);
	C_UINT32 padWidth = width + (pad << 1);
	C_UINT32 padHeight = height + (pad << 1);
	UCHAR* gray8bitData = new UCHAR[size]{ 0 };

	// 1.
	MNDT::ChangeColor(bitmap.data, gray8bitData
		, width, height
		, ColerType::BGR2GRAY_8BIT);

	// 2.
	UCHAR* line8bitData = new UCHAR[size]{ 0 };
	SetHOGEdge(bitmap, line8bitData, 3);
	SetHoughLines(line8bitData, width, height, 700, false);

	// 3.
	C_UINT32 K = kmean.GetCenterSize();
	Pixel* pixels = new Pixel[kmean.GetCenterSize()];
	srand(time(NULL));
	for (UINT32 index = 0; index < K; index++)
	{
		pixels[index] = GetColor();
	}

	C_UINT32 fetureSize = block * block;
	C_UINT32 histogramSize = kmean.GetDimension();
	Image srcImage(bitmap.data, width, height, MNDT::BGR_24BIT);
	Image grayImage(gray8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	Image lineImage(line8bitData, width, height, MNDT::ImageType::GRAY_8BIT);
	concurrency::parallel_for(static_cast<int>(pad), static_cast<int>(height - pad), [&](C_INT32& row)
	{
		C_UINT32 rowIndex = (row - pad) * width;
		for (UINT32 col = pad; col < width - pad; col++)
		{
			Rect rect(col - pad, row - pad, block, block);
			double* pur = new double[histogramSize] {0};
			double* purPointer = pur;

			MNDT::Screenshot8bit(grayImage, purPointer, rect);
			MNDT::L1Normalized8bit(purPointer, fetureSize);
			purPointer += fetureSize;

			MNDT::Screenshot8bit(lineImage, purPointer, rect);
			MNDT::L1Normalized8bit(purPointer, fetureSize);

			C_UINT32 predict = kmean.Predict(pur);
			C_UINT32 showValue = 1 << predict;
			if ((showValue & showFeature) == showValue)
			{
				srcImage.SetPixel(row, col, pixels[predict]);
			}

			delete[] pur;
			pur = nullptr;
		}
	});


	delete[] pixels;
	pixels = nullptr;

	delete[] gray8bitData;
	gray8bitData = nullptr;

	delete[] line8bitData;
	line8bitData = nullptr;

	bitmap.Save(savePath.c_str());
}

void MeanPredict(const std::string& centerPath, const std::string& imgPath, const std::string& savePath, C_UINT32 showFeature)
{
	KMean<double> kmean(centerPath);
	Bitmap bitmap(imgPath);
	MeanPredictProcess(kmean, bitmap, savePath, showFeature);
}
// k-mean predict end


int main(int argc, char *argv[])
{
	/*MeanPredict("D:\\VisualStudio2013Project\\Projects\\SolarPanellnaspectionWindows\\SolarPanellnaspectionWindows\\bin\\Debug\\KMean\\kmean.txt"
		, "D:\\VisualStudio2013Project\\Projects\\SolarPanellnaspectionWindows\\SolarPanellnaspectionWindows\\bin\\Debug\\KMean\\1.bmp"
		, "D:\\VisualStudio2013Project\\Projects\\SolarPanellnaspectionWindows\\SolarPanellnaspectionWindows\\bin\\Debug\\KMean\\2.bmp");*/
	//LoadKMeanTrainData("D:\\VisualStudio2013Project\\Projects\\SolarPanellnaspectionWindows\\SolarPanellnaspectionWindows\\bin\\Debug\\KMean\\3.bmp", "D:\\VisualStudio2013Project\\Projects\\SolarPanellnaspectionWindows\\SolarPanellnaspectionWindows\\bin\\Debug\\KMean\\kmean.txt"
	//	, 100, 3, 100);
	
	if (argc > 1)
	{
		std::string commend(argv[1]);
		if (commend == "Screenshot" && argc > 3)
		{
			const Bitmap bitmap(argv[2]);
			ScreenshotProcess(bitmap, argv[3]);

			std::cout << "1";
			return 1;
		}
		else if (commend == "Detection" && argc > 4)
		{
			const Bitmap bitmap(argv[2]);
			StainProcess(bitmap, argv[3], std::stoi(argv[4]));

			std::cout << 1;
			return 1;
		}
		else if (commend == "ScreenshotAll"  && argc > 3)
		{
			_dataPath = argv[2];
			_screenshotPath = argv[3];
			LoadData();

			std::cout << "1";
			return 1;
		}
		else if (commend == "DetectionAll"  && argc > 3)
		{
			_screenshotDataPath = argv[2];
			_stainDataPath = argv[3];
			LoadScreenshotData();

			std::cout << "1";
			return 1;
		}
		else if (commend == "MeanTrain"  && argc > 3)
		{
			LoadKMeanTrainData(argv[2], argv[3]
				, std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6]));

			std::cout << "1";
			return 1;
		}
		else if (commend == "MeanPredict"  && argc > 3)
		{
			MeanPredict(argv[2], argv[3], argv[4], std::stoi(argv[5]));

			std::cout << "1";
			return 1;
		}
	}
	//system("pause");
	//std::cout << "0";
	return 0;
}

