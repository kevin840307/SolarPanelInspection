#include "stdafx.h"
#include "Library.h"

namespace MNDT {
	void ChangeColor(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, const ColerType type)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);

		switch (type)
		{
		case ColerType::BGR2GRAY_8BIT:
			Inner::BGR2Gray8Bit(src, pur, width, height);
			break;
		}
	}

	// 灰階
	// R 0.299 ≈ 19595
	// G 0.587 ≈ 38469
	// B 0.114 ≈ 7472 (進位 反推回去比7471接近)
	// 為了快速運算(整數運算+位移) 先將數值左位移16次冪 65536
	void Inner::BGR2Gray8Bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height)
	{
		C_UINT32 size = width * height;
		concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
		{
			C_UCHAR* nowSrc = src + index * 3;
			C_UINT32 B = *nowSrc;
			C_UINT32 G = *(nowSrc + 1);
			C_UINT32 R = *(nowSrc + 2);
			C_UCHAR pix = static_cast<UCHAR>((R * 19595 + G * 38469 + B * 7472) >> 16);
			*(pur + index) = pix;
		});
	}

	void ImagePadding8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_INT32 pad)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);
		assert(pad >= 0);

		C_UINT32 copySize = width * sizeof(UCHAR);
		C_UINT32 purWidth = (width + (pad << 1));
		pur += (pad * purWidth) + pad;

		concurrency::parallel_for(0, static_cast<int>(height), [&](const int& index)
		{
			memcpy(pur + purWidth * index, src + width * index, copySize);
		});
	}

	void OriginalPadding8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_INT32 pad)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);
		assert(pad >= 0);

		C_UINT32 copySize = width * sizeof(UCHAR);
		C_UINT32 purWidth = (width + (pad << 1));
		pur += (pad * purWidth) + pad;

		concurrency::parallel_for(0, static_cast<int>(height), [&](const int& index)
		{
			memcpy(pur + purWidth * index, src + width * index, copySize);
		});
	}

	void Sobel8bit(C_UCHAR* src, int32_t* data
		, C_UINT32 width, C_UINT32 height
		, C_BOOL dx, C_BOOL dy)
	{
		// 1. padding
		C_UINT32 padWidth = width + 2;
		C_UINT32 padHeight = height + 2;
		UCHAR* padSrc = new UCHAR[padWidth * padHeight];

		ImagePadding8bit(src, padSrc, width, height, 1);


		// 2. set kernel
		Image srcImage(padSrc, padWidth, padHeight, ImageType::GRAY_8BIT);
		int32_t kernels[9];

		Inner::SetSobelKernels(kernels
			, dx, dy);


		// 3. calculate convolution
		for (UINT32 row = 1; row < padHeight - 1; row++)
		{
			for (UINT32 col = 1; col < padWidth - 1; col++)
			{
				int32_t sum = 0;
				UINT32 kernelsIndex = 0;

				for (int32_t blockRow = -1; blockRow <= 1; blockRow++)
				{
					for (int32_t blockCol = -1; blockCol <= 1; blockCol++)
					{
						UCHAR pix = srcImage.image[row + blockRow][col + blockCol];

						sum += (static_cast<int32_t>(pix) * kernels[kernelsIndex]);
						kernelsIndex++;
					}
				}

				*data = sum;
				data++;
			}
		}

		delete[] padSrc;
		padSrc = nullptr;
	}

	void SobelEdgeView8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height)
	{
		// 1. get sobel
		C_UINT32 size = width * height;
		int32_t* Gx = new int32_t[size];
		int32_t* Gy = new int32_t[size];
		C_UCHAR* srcEnd = src + size;

		Sobel8bit(src, Gx
			, width, height
			, true, false);

		Sobel8bit(src, Gy
			, width, height
			, false, true);


		// 2. calculate abs and get max
		int32_t max = 0;
		int32_t* data = new int32_t[size];
		int32_t* dataPointer = data;
		int32_t* GxPointer = Gx;
		int32_t* GyPointer = Gy;


		while (src < srcEnd)
		{
			*dataPointer = abs(*GxPointer) + abs(*GyPointer);
			max = max < *dataPointer ? *dataPointer : max;

			dataPointer++;
			src++;
			GxPointer++;
			GyPointer++;
		}

		delete[] Gx;
		Gx = nullptr;

		delete[] Gy;
		Gy = nullptr;



		// 3. normalization
		concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
		{
			*(pur + index) = *(data + index) * 255 / max;
		});

		delete[] data;
		data = nullptr;
	}

	void Inner::SetSobelKernels(int32_t* kernels
		, C_BOOL dx, C_BOOL dy)
	{
		if (!dx && dy)
		{
			//int32_t arrays[9] = { -3, -10, -3, 0, 0, 0, 3, 10, 3 };
			int32_t arrays[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
		else if (dx && !dy)
		{
			//int32_t arrays[9] = { -3, 0, 3, -10, 0, 10, -3, 0, 3 };
			int32_t arrays[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
		else if (dx && dy)
		{
			int32_t arrays[9] = { -2, -2, 0, -2, 0, 2, 0, 2, 2 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
		else
		{
			int32_t arrays[9] = { 0, -2, -2, 2, 0, -2, 2, 2, 0 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
	}

	void Scharr8bit(C_UCHAR* src, int32_t* data
		, C_UINT32 width, C_UINT32 height
		, C_BOOL dx, C_BOOL dy)
	{
		// 1. padding
		C_UINT32 padWidth = width + 2;
		C_UINT32 padHeight = height + 2;
		UCHAR* padSrc = new UCHAR[padWidth * padHeight];

		ImagePadding8bit(src, padSrc, width, height, 1);


		// 2. set kernel
		Image srcImage(padSrc, padWidth, padHeight, ImageType::GRAY_8BIT);
		int32_t kernels[9];

		Inner::SetScharrKernels(kernels
			, dx, dy);


		// 3. calculate convolution
		for (UINT32 row = 1; row < padHeight - 1; row++)
		{
			for (UINT32 col = 1; col < padWidth - 1; col++)
			{
				int32_t sum = 0;
				UINT32 kernelsIndex = 0;

				for (int32_t blockRow = -1; blockRow <= 1; blockRow++)
				{
					for (int32_t blockCol = -1; blockCol <= 1; blockCol++)
					{
						UCHAR pix = srcImage.image[row + blockRow][col + blockCol];

						sum += (static_cast<int32_t>(pix) * kernels[kernelsIndex]);
						kernelsIndex++;
					}
				}

				*data = sum;
				data++;
			}
		}

		delete[] padSrc;
		padSrc = nullptr;
	}

	void ScharrEdgeView8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height)
	{
		// 1. get sobel
		C_UINT32 size = width * height;
		int32_t* Gx = new int32_t[size];
		int32_t* Gy = new int32_t[size];
		C_UCHAR* srcEnd = src + size;

		Scharr8bit(src, Gx
			, width, height
			, true, false);

		Scharr8bit(src, Gy
			, width, height
			, false, true);


		// 2. calculate abs and get max
		int32_t max = 0;
		int32_t* data = new int32_t[size];
		int32_t* dataPointer = data;
		int32_t* GxPointer = Gx;
		int32_t* GyPointer = Gy;


		while (src < srcEnd)
		{
			*dataPointer = abs(*GxPointer) + abs(*GyPointer);
			max = max < *dataPointer ? *dataPointer : max;

			dataPointer++;
			src++;
			GxPointer++;
			GyPointer++;
		}

		delete[] Gx;
		Gx = nullptr;

		delete[] Gy;
		Gy = nullptr;



		// 3. normalization
		concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
		{
			*(pur + index) = *(data + index) * 255 / max;
		});

		delete[] data;
		data = nullptr;
	}

	void Inner::SetScharrKernels(int32_t* kernels
		, C_BOOL dx, C_BOOL dy)
	{
		if (!dx && dy)
		{
			int32_t arrays[9] = { -3, -10, -3, 0, 0, 0, 3, 10, 3 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
		else if (dx && !dy)
		{
			int32_t arrays[9] = { -3, 0, 3, -10, 0, 10, -3, 0, 3 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
		else if (dx && dy)
		{
			int32_t arrays[9] = { -2, -2, 0, -2, 0, 2, 0, 2, 2 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
		else
		{
			int32_t arrays[9] = { 0, -2, -2, 2, 0, -2, 2, 2, 0 };
			memcpy(kernels, arrays, sizeof(int32_t) * 9);
		}
	}

	void Gamma8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_DOUBLE gamma)
	{
		// 1. get gamma table
		UCHAR gammaLUT[256];
		concurrency::parallel_for(0, static_cast<int>(256), [&](const int& index)
		{
			double pix = (index + 0.5) / 256.0; //歸一化
			pix = pow(pix, gamma);
			gammaLUT[index] = static_cast<UCHAR>((pix * 256 - 0.5)); //反歸一化
		});
		

		// 2. set gamma pixel
		C_UINT32 size = width * height;
		concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
		{
			*(pur + index) = gammaLUT[*(src + index)];
		});
	}

	void Threshold8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 thresh)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);

		C_UINT32 size = width * height;
		concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
		{
			*(pur + index) = *(src + index) < thresh ? 0 : 255;
		});

	}

	void BlurGauss8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 size, C_FLOAT sigma)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);

		if (!(size & 1))
		{
			const_cast<UINT32&>(size) = size + 1;
		}

		C_UINT32 blockSize = size * size;
		C_INT32 pad = (size >> 1);
		C_UINT32 padWidth = width + (pad << 1);
		C_UINT32 padHeight = height + (pad << 1);
		UCHAR* padSrc = new UCHAR[padWidth * padHeight];

		ImagePadding8bit(src, padSrc, width, height, pad);

		Image srcImage(padSrc, padWidth, padHeight, ImageType::GRAY_8BIT);
		Image purImage(pur, width, height, ImageType::GRAY_8BIT);

		float** temp = new float*[size];

		for (UINT32 index = 0; index < size; index++)
		{
			temp[index] = new float[size];
		}

		Inner::Gaussian2DTemp(temp, size, sigma);

		concurrency::parallel_for(pad, static_cast<int>(padHeight - pad), [&](const int& row)
		{
			for (UINT32 col = pad; col < padWidth - pad; col++)
			{
				float sum = 0;

				for (int32_t blockRow = -pad; blockRow <= pad; blockRow++)
				{
					for (int32_t blockCol = -pad; blockCol <= pad; blockCol++)
					{
						sum += (srcImage.image[row + blockRow][col + blockCol] * temp[pad + blockRow][pad + blockCol]);
					}
				}
				purImage.image[row - pad][col - pad] = static_cast<UCHAR>(sum);
			}
		});



		for (UINT32 index = 0; index < size; index++)
		{
			delete[] temp[index];
			temp[index] = nullptr;
		}
		delete[] temp;
		temp = nullptr;

		delete[] padSrc;
		padSrc = nullptr;
	}

	void Inner::Gaussian2DTemp(float** const temp, C_INT32 size, C_FLOAT sigma)
	{
		float sum = 0;
		C_INT32 center = size >> 1;
		C_FLOAT expBase = -(2.0f * sigma * sigma);
		C_FLOAT scaleBase = (2.0f * sigma * sigma) *  static_cast<float>(PI);

		for (int32_t row = 0; row < size; row++)
		{
			C_INT32 y = center - row;
			for (int32_t col = 0; col < size; col++)
			{
				C_INT32 x = col - center;

				float gaussNum = 0;

				gaussNum = exp((x * x + y * y) / expBase);
				gaussNum = (gaussNum / scaleBase);
				temp[row][col] = gaussNum;
				sum += gaussNum;
			}
		}

		for (int32_t row = 0; row < size; row++)
		{
			for (int32_t col = 0; col < size; col++)
			{
				temp[row][col] /= sum;
			}
		}
	}

	void Screenshot8bit(const Image& srcImage, UCHAR* pur
		, const Rect& rect)
	{
		Image purImage(pur, rect.Width(), rect.Height(), MNDT::ImageType::GRAY_8BIT);
		concurrency::parallel_for(rect.Y(), static_cast<int>(rect.EndY() + 1), [&](const int& row)
		{
			for (UINT32 col = rect.X(); col <= rect.EndX(); col++)
			{
				purImage.image[row - rect.Y()][col - rect.X()] = srcImage.image[row][col];
			}
		});
	}

	void Screenshot24bit(const Image& srcImage, Image& purImage
		, const Rect& rect)
	{
		concurrency::parallel_for(rect.Y(), static_cast<int>(rect.EndY() + 1), [&](const int& row)
		{
			for (UINT32 col = rect.X(); col <= rect.EndX(); col++)
			{
				purImage.SetPixel(row - rect.Y(), col - rect.X(), srcImage.GetPixel(row, col));
			}
		});
	}

	void ContourFrame8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 fileIndex)
	{
		C_UINT32 padWidth = width + 2;
		C_UINT32 padHeight = height + 2;
		UCHAR* padData = new UCHAR[padWidth * padHeight];
		MNDT::ImagePadding8bit(src, padData
			, width, height, 1);

		Image padImage(padData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);
		std::vector<UINT32> labelLinks(1, 0);
		UINT32* edges = new UINT32[padWidth * padHeight]{ 0 };
		UINT32 previousIndex = 1;
		UINT32 nowIndex = padWidth + 1;
		UINT32 label = 1;
		for (UINT32 row = 1; row < padHeight - 1; row++)
		{
			for (UINT32 col = 1; col < padWidth - 1; col++)
			{
				if (padImage.image[row][col] > 250)
				{
					std::vector<UINT32> labels;
					if (edges[nowIndex - 1] > 0)
					{
						labels.push_back(edges[nowIndex - 1]);
					}
					if (edges[previousIndex - 1] > 0)
					{
						labels.push_back(edges[previousIndex - 1]);
					}
					if (edges[previousIndex] > 0)
					{
						labels.push_back(edges[previousIndex]);
					}
					if (edges[previousIndex + 1] > 0)
					{
						labels.push_back(edges[previousIndex + 1]);
					}

					UINT32 minLabel = INT_MAX;
					for (UINT32 index = 0; index < labels.size(); index++)
					{
						minLabel = minLabel > labels[index] ? labels[index] : minLabel;
					}

					if (minLabel == INT_MAX)
					{
						edges[nowIndex] = label;
						labelLinks.push_back(label);
						label++;
					}
					else
					{
						edges[nowIndex] = labelLinks[minLabel];

						for (UINT32 index = 0; index < labels.size(); index++)
						{
							if (labelLinks[minLabel] < labelLinks[labels[index]])
							{
								labelLinks[labels[index]] = labelLinks[minLabel];
							}
							else if (labelLinks[minLabel] > labelLinks[labels[index]])
							{
								labelLinks[minLabel] = labelLinks[labels[index]];
							}
						}
					}
				}

				previousIndex++;
				nowIndex++;
			}
			previousIndex += 2;
			nowIndex += 2;
		}

		delete[] padData;
		padData = nullptr;

		for (UINT32 index = 1; index < labelLinks.size(); index++)
		{
			UINT32 nowLabel = labelLinks[index];
			UINT32 nowIndex = index;
			while (nowLabel != nowIndex)
			{
				nowLabel = labelLinks[labelLinks[nowIndex]];
				nowIndex = labelLinks[nowLabel];
			}
			labelLinks[index] = nowLabel;
		}


		Rect* rects = new Rect[labelLinks.size()];
		nowIndex = padWidth + 1;
		for (__int32 row = 1; row < padHeight - 1; row++)
		{
			for (__int32 col = 1; col < padWidth - 1; col++)
			{
				Rect& rect = rects[labelLinks[edges[nowIndex]]];
				if (rect.X() == 0 || rect.X() > col - 1)
				{
					rect.X(col - 1);
				}
				if (rect.EndX() == -1 || rect.EndX() < col - 1)
				{
					rect.Width(col - 1 - rect.X());
				}
				if (rect.Y() == 0 || rect.Y() > row - 1)
				{
					rect.Y(row - 1);
				}
				if (rect.EndY() == -1 || rect.EndY() < row - 1)
				{
					rect.Height(row - 1 - rect.Y());
				}

				nowIndex++;
			}
			nowIndex += 2;
		}

		delete[] edges;
		edges = nullptr;

		Image purImage(pur, width, height, MNDT::ImageType::GRAY_8BIT);
		Image srcImage(const_cast<UCHAR*>(src), width, height, MNDT::ImageType::GRAY_8BIT);
		for (UINT32 index = 1; index < labelLinks.size(); index++)
		{
			const Rect& rect = rects[index];
			if (rect.Width() > 3000 && rect.Height() > 2000
				&& rect.Width() < width - 300)
			{
				UCHAR* rectData2 = new UCHAR[rect.Width() * rect.Height()];
				MNDT::Screenshot8bit(srcImage, rectData2, rect);
				Bitmap bit(rectData2, rect.Width(), rect.Height(), MNDT::ImageType::GRAY_8BIT);
				std::string str = "";
				std::stringstream stream;
				stream << fileIndex << "_" << index;
				stream >> str;
				str = "D:\\Data\\" + str + ".bmp";
				bit.Save(str.c_str());

				delete[] rectData2;
				rectData2 = nullptr;

				MNDT::DrawRect8bit(purImage, rect, 255);
			}
		}

		delete[] rects;
		rects = nullptr;

	}

	void ContourFrame8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height)
	{
		C_UINT32 padWidth = width + 2;
		C_UINT32 padHeight = height + 2;
		UCHAR* padData = new UCHAR[padWidth * padHeight];
		MNDT::ImagePadding8bit(src, padData
			, width, height, 1);

		Image padImage(padData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);
		std::vector<UINT32> labelLinks(1, 0);
		UINT32* edges = new UINT32[padWidth * padHeight]{ 0 };
		UINT32 previousIndex = 1;
		UINT32 nowIndex = padWidth + 1;
		UINT32 label = 1;
		for (UINT32 row = 1; row < padHeight - 1; row++)
		{
			for (UINT32 col = 1; col < padWidth - 1; col++)
			{
				if (padImage.image[row][col] > 250)
				{
					std::vector<UINT32> labels;
					if (edges[nowIndex - 1] > 0)
					{
						labels.push_back(edges[nowIndex - 1]);
					}
					if (edges[previousIndex - 1] > 0)
					{
						labels.push_back(edges[previousIndex - 1]);
					}
					if (edges[previousIndex] > 0)
					{
						labels.push_back(edges[previousIndex]);
					}
					if (edges[previousIndex + 1] > 0)
					{
						labels.push_back(edges[previousIndex + 1]);
					}

					UINT32 minLabel = INT_MAX;
					for (UINT32 index = 0; index < labels.size(); index++)
					{
						minLabel = minLabel > labels[index] ? labels[index] : minLabel;
					}

					if (minLabel == INT_MAX)
					{
						edges[nowIndex] = label;
						labelLinks.push_back(label);
						label++;
					}
					else
					{
						edges[nowIndex] = labelLinks[minLabel];

						for (UINT32 index = 0; index < labels.size(); index++)
						{
							if (labelLinks[minLabel] < labelLinks[labels[index]])
							{
								labelLinks[labels[index]] = labelLinks[minLabel];
							}
							else if (labelLinks[minLabel] > labelLinks[labels[index]])
							{
								labelLinks[minLabel] = labelLinks[labels[index]];
							}
						}
					}
				}

				previousIndex++;
				nowIndex++;
			}
			previousIndex += 2;
			nowIndex += 2;
		}

		delete[] padData;
		padData = nullptr;

		for (UINT32 index = 1; index < labelLinks.size(); index++)
		{
			UINT32 nowLabel = labelLinks[index];
			UINT32 nowIndex = index;
			while (nowLabel != nowIndex)
			{
				nowLabel = labelLinks[labelLinks[nowIndex]];
				nowIndex = labelLinks[nowLabel];
			}
			labelLinks[index] = nowLabel;
		}


		Rect* rects = new Rect[labelLinks.size()];
		nowIndex = padWidth + 1;
		for (__int32 row = 1; row < padHeight - 1; row++)
		{
			for (__int32 col = 1; col < padWidth - 1; col++)
			{
				Rect& rect = rects[labelLinks[edges[nowIndex]]];
				if (rect.X() == 0 || rect.X() > col - 1)
				{
					rect.X(col - 1);
				}
				if (rect.EndX() == -1 || rect.EndX() < col - 1)
				{
					rect.Width(col - 1 - rect.X());
				}
				if (rect.Y() == 0 || rect.Y() > row - 1)
				{
					rect.Y(row - 1);
				}
				if (rect.EndY() == -1 || rect.EndY() < row - 1)
				{
					rect.Height(row - 1 - rect.Y());
				}

				nowIndex++;
			}
			nowIndex += 2;
		}

		delete[] edges;
		edges = nullptr;

		Image purImage(pur, width, height, MNDT::ImageType::GRAY_8BIT);
		Image srcImage(const_cast<UCHAR*>(src), width, height, MNDT::ImageType::GRAY_8BIT);
		for (UINT32 index = 1; index < labelLinks.size(); index++)
		{
			const Rect& rect = rects[index];
			if (rect.Width() > 0 && rect.Height() > 0)
			{
				MNDT::DrawRect8bit(purImage, rect, 255);
			}
		}

		delete[] rects;
		rects = nullptr;

	}

	std::vector<Rect> ContourFrame8bit(C_UCHAR* src
		, C_UINT32 width, C_UINT32 height)
	{
		C_UINT32 padWidth = width + 2;
		C_UINT32 padHeight = height + 2;
		UCHAR* padData = new UCHAR[padWidth * padHeight];
		MNDT::ImagePadding8bit(src, padData
			, width, height, 1);

		Image padImage(padData, padWidth, padHeight, MNDT::ImageType::GRAY_8BIT);
		std::vector<UINT32> labelLinks(1, 0);
		UINT32* edges = new UINT32[padWidth * padHeight]{ 0 };
		UINT32 previousIndex = 1;
		UINT32 nowIndex = padWidth + 1;
		UINT32 label = 1;
		for (UINT32 row = 1; row < padHeight - 1; row++)
		{
			for (UINT32 col = 1; col < padWidth - 1; col++)
			{
				if (padImage.image[row][col] > 250)
				{
					std::vector<UINT32> labels;
					if (edges[nowIndex - 1] > 0)
					{
						labels.push_back(edges[nowIndex - 1]);
					}
					if (edges[previousIndex - 1] > 0)
					{
						labels.push_back(edges[previousIndex - 1]);
					}
					if (edges[previousIndex] > 0)
					{
						labels.push_back(edges[previousIndex]);
					}
					if (edges[previousIndex + 1] > 0)
					{
						labels.push_back(edges[previousIndex + 1]);
					}

					UINT32 minLabel = INT_MAX;
					for (UINT32 index = 0; index < labels.size(); index++)
					{
						minLabel = minLabel > labels[index] ? labels[index] : minLabel;
					}

					if (minLabel == INT_MAX)
					{
						edges[nowIndex] = label;
						labelLinks.push_back(label);
						label++;
					}
					else
					{
						edges[nowIndex] = labelLinks[minLabel];

						for (UINT32 index = 0; index < labels.size(); index++)
						{
							if (labelLinks[minLabel] < labelLinks[labels[index]])
							{
								labelLinks[labels[index]] = labelLinks[minLabel];
							}
							else if (labelLinks[minLabel] > labelLinks[labels[index]])
							{
								labelLinks[minLabel] = labelLinks[labels[index]];
							}
						}
					}
				}

				previousIndex++;
				nowIndex++;
			}
			previousIndex += 2;
			nowIndex += 2;
		}

		delete[] padData;
		padData = nullptr;

		for (UINT32 index = 1; index < labelLinks.size(); index++)
		{
			UINT32 nowLabel = labelLinks[index];
			UINT32 nowIndex = index;
			while (nowLabel != nowIndex)
			{
				nowLabel = labelLinks[labelLinks[nowIndex]];
				nowIndex = labelLinks[nowLabel];
			}
			labelLinks[index] = nowLabel;
		}


		std::vector<Rect> rects(labelLinks.size());
		nowIndex = padWidth + 1;
		for (__int32 row = 1; row < padHeight - 1; row++)
		{
			for (__int32 col = 1; col < padWidth - 1; col++)
			{
				Rect& rect = rects[labelLinks[edges[nowIndex]]];
				if (rect.X() == 0 || rect.X() > col - 1)
				{
					rect.X(col - 1);
				}
				if (rect.EndX() == -1 || rect.EndX() < col - 1)
				{
					rect.Width(col - 1 - rect.X());
				}
				if (rect.Y() == 0 || rect.Y() > row - 1)
				{
					rect.Y(row - 1);
				}
				if (rect.EndY() == -1 || rect.EndY() < row - 1)
				{
					rect.Height(row - 1 - rect.Y());
				}

				nowIndex++;
			}
			nowIndex += 2;
		}

		delete[] edges;
		edges = nullptr;

		return rects;
	}

	void ThresholdingOTSU8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height)
	{
		C_UCHAR scale = Inner::GetThresholdingScale(src, width, height);
		C_UINT32 size = width * height;
		concurrency::parallel_for(0, static_cast<int>(size), [&](const int& index)
		{
			*(pur + index) = *(src + index) < scale ? 255 : 0;
		});
	}

	UCHAR Inner::GetThresholdingScale(C_UCHAR* src
		, C_UINT32 width, C_UINT32 height)
	{
		C_UINT32 size = (width * height);
		double histogram[256] = { 0.0 };

		for (int index = 0; index < size; index++)
		{
			histogram[*src]++;
			src++;
		}

		for (UINT32 index = 0; index < 256; index++)
		{
			histogram[index] /= size;
		}

		UCHAR indexMax = 0;
		double maxScale = -1;

		for (UINT32 index = 0; index < 256; index++)
		{
			double w0 = 0;
			double s0 = 0;
			double u0 = 0;
			double w1 = 0;
			double s1 = 0;
			double u1 = 0;
			double argU = 0;
			double scale = -1;

			for (UINT32 pix = 0; pix < 256; pix++)
			{
				if (pix <= index)	//背景部分  
				{
					w0 += histogram[pix];
					s0 += pix * histogram[pix];
				}
				else				//前景部分  
				{
					w1 += histogram[pix];
					s1 += pix * histogram[pix];
				}
			}

			u0 = s0 / w0;
			u1 = s1 / w1;
			argU = s0 + s1;
			scale = w0 * (u0 - argU) * (u0 - argU) + w1 * (u1 - argU) * (u1 - argU);

			if (index > 10 && scale > maxScale)
			{
				maxScale = scale;
				indexMax = index;
			}
		}

		return indexMax;
	}

	void Expansion8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 block)
	{
		Image srcImage(const_cast<UCHAR*>(src), width, height, MNDT::ImageType::GRAY_8BIT);
		Image purImage(pur, width, height, MNDT::ImageType::GRAY_8BIT);
		C_UINT32 offset = block >> 1;
		concurrency::parallel_for(static_cast<int>(offset), static_cast<int>(height - offset), [&](const int& row)
		{
			for (UINT32 col = offset; col < width - offset; col++)
			{
				C_UCHAR pix = Inner::GetMorphologyValue(srcImage
					, row - offset, col - offset, block
					, 255);
				purImage.image[row][col] = pix;
			}
		});
	}

	void Erosion8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 block)
	{
		Image srcImage(const_cast<UCHAR*>(src), width, height, MNDT::ImageType::GRAY_8BIT);
		Image purImage(pur, width, height, MNDT::ImageType::GRAY_8BIT);
		C_UINT32 offset = block >> 1;
		concurrency::parallel_for(static_cast<int>(offset), static_cast<int>(height - offset), [&](const int& row)
		{
			for (UINT32 col = offset; col < width - offset; col++)
			{
				C_UCHAR pix = Inner::GetMorphologyValue(srcImage
					, row - offset, col - offset, block
					, 0);
				purImage.image[row][col] = pix;
			}
		});
	}

	UCHAR Inner::GetMorphologyValue(const Image& srcImage
		, C_UINT32 blockRow, C_UINT32 blockCol, C_UINT32 blockSize
		, C_UCHAR threshold)
	{
		for (UINT32 row = blockRow; row < blockRow + blockSize; row++)
		{
			for (UINT32 col = blockCol; col < blockCol + blockSize; col++)
			{
				if (srcImage.image[row][col] == threshold)
				{
					return threshold;
				}
			}
		}
		return 255 - threshold;
	}
}