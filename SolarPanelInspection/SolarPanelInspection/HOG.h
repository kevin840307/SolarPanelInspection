#pragma once
#ifndef HOG_H
#define HOG_H
#include "general.h"
#include "Library.h"

class HOG
{
public:
	~HOG();

	HOG(C_UINT32 cellX = 8, C_UINT32 cellY = 8, C_UINT32 blockX = 2, C_UINT32 blockY = 2) :
		_cellX(cellX), _cellY(cellY), _blockX(blockX), _blockY(blockY) {};

	void GradienView(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height);
private:

	void Gradient(C_UCHAR* src
		, C_UINT32 width, C_UINT32 height
		, double* amplitudes, double* angles);

	C_UINT32 _bin = 9;
	C_UINT32 _cellX = 8;
	C_UINT32 _cellY = 8;
	C_UINT32 _blockX = 2;
	C_UINT32 _blockY = 2;
};


#endif