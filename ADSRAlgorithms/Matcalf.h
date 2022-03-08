#include "typedef.h"

void ComplexMat3DTORealMat3D( Mat3DDoub & G , Mat3DDoub & ResultArray );
//void ComplexArrayTORealArray( Doub Complex[MAX_V][MAX_V][MAX_BS],Doub Real[MAX_V][MAX_V][MAX_BS],int Size[3]);
void RealMat3DTOComplexMat3D( Mat3DDoub & G,  Mat3DDoub & ResultArray );
//void RealArrayTOComplexArray( Mat3DDoub & G,  Doub Complex[MAX_V][MAX_V][MAX_BS]);
void Matrix3DComplexProduct( Mat3DDoub & LMAT, Mat3DDoub & RMAT, Mat3DDoub & ResultArray );
void Matrix3DComplexProduct( Mat3DDoub & LMAT, MatDoub & RMAT, MatDoub & ResultArray );
//void Matrix3DComplexProduct2( Doub LMAT[MAX_V][MAX_V][MAX_BS],Doub RMAT[MAX_V][MAX_V][MAX_BS],Doub ResultArray[MAX_V][MAX_V][MAX_BS],int Size[2][3]);
void Matrix3DRealProduct( Mat3DDoub & LMAT, Mat3DDoub & RMAT, Mat3DDoub & ResultArray );
void ComplexMatrixToAmplitudeNAngle( Mat3DDoub_I & H, Mat3DDoub &Amplitude, Mat3DDoub &Angle);
