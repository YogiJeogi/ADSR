#pragma once
#ifndef __SPECTRUM_H__
#define __SPECTRUM_H__

#include "typedef.h"
#include "fourier.h"

extern Doub square(int j,int n);
extern Doub bartlett(int j,int n);
extern Doub welch(int j,int n);
extern Doub hann(int j,int n);
extern Doub blackman(int j,int n);


//*************************************************************************//
// HANDONG
//*************************************************************************//

struct Spectreg2D {
	int n,m,m2,nsum; // n 채널 / m2 데이타 / m+1 spectral line, m은 BS의 반이다.
	MatDoub specsum;
	VecDoub wksp;

	Spectreg2D(int en,int em) 
		: n(en)	, m(em)	, m2(2*m) , nsum(0)	, specsum(en,em+1,0.), wksp(m2) 
	{ if (m & (m-1)) throw("m must be power of 2"); }

	template<class D>
	void adddataseg(MatDoub_I &data, D &window) {
		for( int i=0 ; i< data.nrows() ; i++ )
		{
			Doub w,fac,sumw = 0.;
			if (data.nrows() != n ) throw("wrong size Dimention");
			if (data.ncols() != m2) throw("wrong size data segment");
			for (int j=0;j<m2;j++) {
				w = window(j,m2);
				wksp[j] = w*data[i][j];
				sumw += SQR(w);
			}
			fac = 2./(sumw*m2);
			realft(wksp,1);
			specsum[i][0] += 0.5*fac*SQR(wksp[0]);
			for (int j=1;j<m;j++) specsum[i][j] += fac*(SQR(wksp[2*j])+SQR(wksp[2*j+1]));
			specsum[i][m] += 0.5*fac*SQR(wksp[1]);
		}
		nsum++;
	}

	MatDoub spectrum();
	VecDoub frequencies();
};
struct SpectOlap2D
{
	int n,m,m2,nsum,Fs; // n 채널 / m2 데이타 / m+1 spectral line, m은 BS의 반이다.
	int olapshift,Point;
	int LSavedRespData,LShiftedData;
	bool Last;
	MatDoub signals;// 전제 시그널
	MatDoub vecforPSD; // 부분 데이타 Length BS;
	MatDoub specsum;
	VecDoub wksp;
	SpectOlap2D( int en,int em,int eolap, int eFs ,int ePoint);

	void adddataseg(const MatDoub &data, Doub (*window)(int,int));
	void adddata(const MatDoub &data, Doub (*window)(int, int),int lp=0);

	MatDoub spectrum();
	VecDoub frequencies();
};

#endif /* __SPECTRUM_H__ */