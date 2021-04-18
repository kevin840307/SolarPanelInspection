#include "stdafx.h"
#include "Bitmap.h"

inline __int32 Bitmap::CharsToInt(C_CHAR* chars, C_UINT32 size) const
{
	__int32 num = 0;
	memcpy(&num, chars, size);

	return num;
}

inline void Bitmap::IntToChars(C_INT32 num, char* chars, C_UINT32 size)
{
	memcpy(chars, &num, size);
}

Bitmap::Bitmap(const std::string path)
{
	_file.open(path, std::ios::in | std::ios::binary);
	_path = path;

	FileInit();
}

Bitmap::Bitmap(C_UCHAR* data
	, C_UINT32 width, C_UINT32 height
	, const MNDT::ImageType type)
{
	this->data = new UCHAR[width * height * type];
	memcpy(this->data, data, sizeof(UCHAR) * width * height * type);

	C_UINT32 relWidth = type * width + OffsetWidth(width);
	C_UINT32 offset = type == MNDT::ImageType::GRAY_8BIT ? 1078 : 54;

	IntToChars(19778, _bitmapFileHeader.identifier, 2);
	IntToChars(relWidth * height + offset, _bitmapFileHeader.fileSize, 4);
	IntToChars(0, _bitmapFileHeader.reserved, 4);
	IntToChars(offset, _bitmapFileHeader.offset, 4);

	IntToChars(40, _bitmapInfoHeader.headerSize, 4);
	IntToChars(width, _bitmapInfoHeader.width, 4);
	IntToChars(height, _bitmapInfoHeader.height, 4);
	IntToChars(1, _bitmapInfoHeader.planes, 2);
	IntToChars(type * 8, _bitmapInfoHeader.pixOfBit, 2);
	IntToChars(0, _bitmapInfoHeader.compression, 4);
	IntToChars(relWidth * height, _bitmapInfoHeader.dmpSize, 4);
	IntToChars(4724, _bitmapInfoHeader.HResolution, 4);
	IntToChars(4724, _bitmapInfoHeader.VResolution, 4);
	IntToChars(0, _bitmapInfoHeader.usedColors, 4);
	IntToChars(0, _bitmapInfoHeader.importantColors, 4);

	if (MNDT::ImageType::GRAY_8BIT == type)
	{
		_palettet = new char[1024];
		for (UINT32 index = 0; index < 256; index++)
		{
			char* nowPix = _palettet + index * 4;
			nowPix[0] = index;
			nowPix[1] = index;
			nowPix[2] = index;
			nowPix[3] = 0;
		}
	}
}

Bitmap::~Bitmap()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}

	if (_palettet != nullptr)
	{
		delete[] _palettet;
		_palettet = nullptr;
	}
}


void Bitmap::FileInit()
{
	if (_file.is_open())
	{
		_file.read((char *)&_bitmapFileHeader, sizeof(BITMAPFILEHEADER));
		_file.read((char *)&_bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

		C_UINT32 offset = CharsToInt(_bitmapFileHeader.offset, 4) - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		if (offset > 0)
		{
			_palettet = new char[offset];
			_file.read((char *)(_palettet), sizeof(char) * offset);
		}

		C_UINT32 channel = ImageType();
		C_UINT32 offsetWidth = OffsetWidth();
		C_UINT32 width = Width() * channel;
		C_UINT32 height = Height();
		data = new UCHAR[width * height];
		for (int index = height - 1; index >= 0; index--)
		{
			_file.read((char *)(data + index * width), sizeof(char) * width);
			_file.seekg(offsetWidth, std::ios::cur);
		}
	}
	_file.close();
}

void Bitmap::PrintFileHeader() const
{
	std::cout << "Identifier(ID)：" << CharsToInt(_bitmapFileHeader.identifier, 2) << std::endl;
	std::cout << "File Size：" << CharsToInt(_bitmapFileHeader.fileSize, 4) << std::endl;
	std::cout << "Reserved：" << CharsToInt(_bitmapFileHeader.reserved, 4) << std::endl;
	std::cout << "Bitmap Data Offset：" << CharsToInt(_bitmapFileHeader.offset, 4) << std::endl;
}

void Bitmap::PrintInfoHeader() const
{
	std::cout << "Bitmap Header Size：" << CharsToInt(_bitmapInfoHeader.headerSize, 4) << std::endl;
	std::cout << "Width：" << CharsToInt(_bitmapInfoHeader.width, 4) << std::endl;
	std::cout << "Height：" << CharsToInt(_bitmapInfoHeader.height, 4) << std::endl;
	std::cout << "Planes：" << CharsToInt(_bitmapInfoHeader.planes, 2) << std::endl;
	std::cout << "Bits Per Pixel：" << CharsToInt(_bitmapInfoHeader.pixOfBit, 2) << std::endl;
	std::cout << "Compression：" << CharsToInt(_bitmapInfoHeader.compression, 4) << std::endl;
	std::cout << "Bitmap Data Size：" << CharsToInt(_bitmapInfoHeader.dmpSize, 4) << std::endl;
	std::cout << "H-Resolution：" << CharsToInt(_bitmapInfoHeader.HResolution, 4) << std::endl;
	std::cout << "V-Resolution：" << CharsToInt(_bitmapInfoHeader.VResolution, 4) << std::endl;
	std::cout << "Used Colors：" << CharsToInt(_bitmapInfoHeader.usedColors, 4) << std::endl;
	std::cout << "Important Colors：" << CharsToInt(_bitmapInfoHeader.importantColors, 4) << std::endl;
}

void Bitmap::Save(C_CHAR* path) const
{
	std::ofstream ofstream;
	ofstream.open(path, std::ios::out | std::ios::binary);

	ofstream.write((char *)&_bitmapFileHeader, sizeof(BITMAPFILEHEADER));

	ofstream.write((char *)&_bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	C_UINT32 offset = CharsToInt(_bitmapFileHeader.offset, 4) - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
	ofstream.write((char *)(_palettet), sizeof(char) * offset);

	C_UINT32 channel = ImageType();
	C_UINT32 offsetWidth = OffsetWidth();
	C_UINT32 width = Width() * channel;
	C_UINT32 height = Height();

	char* offsetChar = offsetWidth != 0 ? new char[offsetWidth] {0} : nullptr;

	for (int index = height - 1; index >= 0; index--)
	{
		ofstream.write((char *)(data + index * width), sizeof(char) * width);

		if (offsetChar != nullptr)
		{
			ofstream.write(offsetChar, sizeof(char) * offsetWidth);
		}
	}
	if (offsetChar != nullptr)
	{
		delete[] offsetChar;
		offsetChar = nullptr;
	}

	ofstream.close();
}

UINT32 Bitmap::Width() const
{
	return CharsToInt(_bitmapInfoHeader.width, 4);
}

UINT32 Bitmap::OffsetWidth(C_UINT32 width) const
{
	return ((4 - (width * ImageType()) % 4) % 4);
}

UINT32 Bitmap::OffsetWidth() const
{
	return OffsetWidth(CharsToInt(_bitmapInfoHeader.width, 4));
}

UINT32 Bitmap::Height() const
{
	return CharsToInt(_bitmapInfoHeader.height, 4);
}

UINT32 Bitmap::ImageType() const
{
	return (CharsToInt(_bitmapInfoHeader.pixOfBit, 2) >> 3);
}