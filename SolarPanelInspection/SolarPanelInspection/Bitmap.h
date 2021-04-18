#pragma once
#ifndef BITMAP_H
#define BITMAP_H
#include "general.h"
#include "Library.h"
#include <iostream>
#include <fstream>

// from:http://crazycat1130.pixnet.net/blog/post/1345538-%E9%BB%9E%E9%99%A3%E5%9C%96%EF%BC%88bitmap%EF%BC%89%E6%AA%94%E6%A1%88%E6%A0%BC%E5%BC%8F

/********************************************************************************
* BitmapFileHeader:
* 0000h	Identifier (ID)		2	'BM'
* 0002h	File Size			4	����I�}���ɮת��j�p(���Gbyte)
* 0006h	Reserved			4	�O�d���
* 000Ah	Bitmap Data Offset	4	�I�}�ϸ�ƶ}�l���e�������q(���Gbyte)
********************************************************************************/
struct BITMAPFILEHEADER {
	char identifier[2];
	char fileSize[4];
	char reserved[4];
	char offset[4];
};


/********************************************************************************
* BitmapInfoHeader
* 000Eh	Bitmap Header Size	4	Bitmap Info Header ������
* 0012h	Width				4	�I�}�Ϫ��e�סA�H����(pixel)�����
* 0016h	Height				4	�I�}�Ϫ����סA�H����(pixel)�����
* 001Ah	Planes				2	�I�}�Ϫ��줸�ϼh��
* 001Ch	Bits Per Pixel		2	�C�ӹ������줸��
* 									1�G����I�}��(�ϥ� 2 ��զ�L)
*									4�G4 �줸�I�}��(�ϥ� 16 ��զ�L)
*									8�G8 �줸�I�}��(�ϥ� 256 ��զ�L)
*									16�G16 �줸���m�I�}��(���@�w�ϥνզ�L)
*									24�G24 �줸���m�I�}��(���ϥνզ�L)
*									32�G32 �줸���m�I�}��(���@�w�ϥνզ�L)
* 001Eh	Compression			4	���Y�覡
*									0�G�����Y
*									1�GRLE 8-bit/pixel
*									2�GRLE 4-bit/pixel
*									3�GBitfields
* 0022h	Bitmap Data Size	4	�I�}�ϸ�ƪ��j�p(���Gbyte)
* 0026h	H-Resolution		4	�����ѪR��(���G����/����)
* 002Ah	V-Resolution		4	�����ѪR��(���G����/����)
* 002Eh	Used Colors			4	�I�}�ϨϥΪ��զ�L�C���
* 0032h	Important Colors	4	���n���C���
********************************************************************************/
struct BITMAPINFOHEADER {
	char headerSize[4];
	char width[4];
	char height[4];
	char planes[2];
	char pixOfBit[2];
	char compression[4];
	char dmpSize[4];
	char HResolution[4];
	char VResolution[4];
	char usedColors[4];
	char importantColors[4];
};

//struct Pixel {
//	UCHAR B, G, R;
//	Pixel(C_UCHAR& b, C_UCHAR g, C_UCHAR r) {
//		B = b;
//		G = g;
//		R = r;
//	}
//
//	Pixel(C_UCHAR& gray) {
//		Pixel(gray, gray, gray);
//	}
//
//	Pixel() {
//		Pixel(0, 0, 0);
//	}
//
//};

class Bitmap
{
public:
	inline __int32 CharsToInt(C_CHAR* chars, C_UINT32 size) const;
	inline void IntToChars(C_INT32 num, char* chars, C_UINT32 size);

	Bitmap(const std::string path);
	Bitmap(C_UCHAR* data
		, C_UINT32 width, C_UINT32 height
		, const MNDT::ImageType type);
	~Bitmap();

	void PrintFileHeader() const;
	void PrintInfoHeader() const;

	void Save(C_CHAR* path) const;

	UINT32 OffsetWidth() const;
	UINT32 OffsetWidth(C_UINT32 width) const;
	UINT32 Width() const;
	UINT32 Height() const;
	UINT32 ImageType() const;

	UCHAR* data = nullptr;
private:

	void FileInit();

	BITMAPFILEHEADER _bitmapFileHeader;
	BITMAPINFOHEADER _bitmapInfoHeader;
	char* _palettet = nullptr;
	std::ifstream _file;
	std::string _path;
};

#endif // !BITMAP_H
