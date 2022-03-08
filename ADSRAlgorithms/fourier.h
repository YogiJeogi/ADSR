#pragma once
#ifndef __FOURIER_H__
#define __FOURIER_H__

#include "typedef.h"

extern void four1(Doub *data, const int n, const int isign);
extern void four1(VecDoub_IO &data, const int isign);
extern void four1(VecComplex_IO &data, const int isign);
extern void realft(VecDoub_IO &data, const int isign);
extern void rlftin3D(Doub * data, int size, const int isign);//Hdlee

#endif /* __FOURIER_H__ */