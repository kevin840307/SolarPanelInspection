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
* 0002h	File Size			4	整個點陣圖檔案的大小(單位：byte)
* 0006h	Reserved			4	保留欄位
* 000Ah	Bitmap Data Offset	4	點陣圖資料開始之前的偏移量(單位：byte)
********************************************************************************/
struct BITMAPFILEHEADER {
	char identifier[2];
	char fileSize[4];
	char reserved[4];
	char offset[4];
};


/********************************************************************************
* BitmapInfoHeader
* 000Eh	Bitmap Header Size	4	Bitmap Info Header 的長度
* 0012h	Width				4	點陣圖的寬度，以像素(pixel)為單位
* 0016h	Height				4	點陣圖的高度，以像素(pixel)為單位
* 001Ah	Planes				2	點陣圖的位元圖層數
* 001Ch	Bits Per Pixel		2	每個像素的位元數
* 									1：單色點陣圖(使用 2 色調色盤)
*									4：4 位元點陣圖(使用 16 色調色盤)
*									8：8 位元點陣圖(使用 256 色調色盤)
*									16：16 位元高彩點陣圖(不一定使用調色盤)
*									24：24 位元全彩點陣圖(不使用調色盤)
*									32：32 位元全彩點陣圖(不一定使用調色盤)
* 001Eh	Compression			4	壓縮方式
*									0：未壓縮
*									1：RLE 8-bit/pixel
*									2：RLE 4-bit/pixel
*									3：Bitfields
* 0022h	Bitmap Data Size	4	點陣圖資料的大小(單位：byte)
* 0026h	H-Resolution		4	水平解析度(單位：像素/公尺)
* 002Ah	V-Resolution		4	垂直解析度(單位：像素/公尺)
* 002Eh	Used Colors			4	點陣圖使用的調色盤顏色數
* 0032h	Important Colors	4	重要的顏色數
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
