#pragma once
#ifndef __SVD_H__
#define __SVD_H__

#include "typedef.h"


///////////////////////////////////////////////////////////////////////
struct SVD {
	int m,n;
	MatDoub u,v,pinv;
	VecDoub w;
	Doub eps, tsh;

	SVD(); // Contructor 초기화는 SVDInitialization을 해야한다. 
		   // Class에서 사용하기 위한 것

	SVD(MatDoub_I &a);
    
	void SVDInitialization( MatDoub_I &a );

	void solve(VecDoub_I &b, VecDoub_O &x, Doub thresh);
	void solve(MatDoub_I &b, MatDoub_O &x, Doub thresh);

	int rank(Doub thresh);
	int nullity(Doub thresh);
	MatDoub range(Doub thresh);
	MatDoub nullspace(Doub thresh);
	void PeudoInverseMatrix(); // Result in pinv

	Doub inv_condition();

	void decompose();
	void reorder();
	Doub pythag(const Doub a, const Doub b);
};

///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////

class CPeudoInverse3D {
private :

	int n,m,k;
	SVD tSVD;
	Mat3DDoub PINV3D;

public :

	CPeudoInverse3D(const Mat3DDoub_I &a);
	Mat3DDoub & GetPinverseMatrix3D();
	~CPeudoInverse3D();

};



#endif