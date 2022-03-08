#include "Matcalf.h"

void ComplexMat3DTORealMat3D( Mat3DDoub & G , Mat3DDoub & ResultArray )
{

	/*========================================================
	//
	//       FFT에 의해서 생성된 Complex MATRIX대상이다.
	//
	//======================================================*/

	// [A]=[Ar]+[Ai]*i

	// [A]=[  Ar Ai ]
	//     [ -Ai Ar ]

	// [4][5][1024] -> [8][10][513]

	// Index
	int i,j,k;

	int cn=G.dim1();
	int cm=G.dim2();
	int ck=G.dim3();
	int rnn=cn*2;
	int rmm=cm*2;
	int rk=ck/2+1;

	ResultArray.AssignConstant(rnn,rmm,rk,0.);

	for ( k=2 ; k < rk ; k++) {
			for( i=0 ; i < cn ;i++) {
					 for ( j=0 ; j < cm ; j++) {
						 //실수부
						 ResultArray[i][j][k]=G[i][j][k*2-2];
						 ResultArray[i+cn][j+cm][k]=G[i][j][k*2-2];
						 //허수부
						 ResultArray[i+cn][j][k]=-1.*G[i][j][k*2-1];
						 ResultArray[i][j+cm][k]=G[i][j][k*2-1];
					 }
			}
	} 

	// 1번째와 (BS/2+1)번째에 대한 
	for ( i=0 ; i < cn ; i++) {
		for ( j=0 ; j < cm ; j++) {
				ResultArray[i][j][0]=G[i][j][0];
				ResultArray[i+cn][j+cm][0]=G[i][j][0];
				ResultArray[i][j][1]=G[i][j][1];
				ResultArray[i+cn][j+cm][1]=G[i][j][1];
		}
	}



				
}

/*
void ComplexArrayTORealArray(Doub Complex[MAX_V][MAX_V][MAX_BS]
							,Doub Real[MAX_V][MAX_V][MAX_BS]
							,int Size[3])
{

	int i,j,k;

	int cn=Size[0];
	int cm=Size[1];
	int ck=Size[2];
	int rnn=cn*2;
	int rmm=cm*2;
	int rk=ck/2+1;

	for ( k=2 ; k < rk ; k++) {
		for( i=0 ; i < cn ;i++) {
			for ( j=0 ; j < cm ; j++) {
				//실수부
				Real[i][j][k]=Complex[i][j][k*2-2];
				Real[i+cn][j+cm][k]=Complex[i][j][k*2-2];
				//허수부
				Real[i+cn][j][k]=-1.*Complex[i][j][k*2-1];
				Real[i][j+cm][k]=Complex[i][j][k*2-1];
			}
		}
	} 

	// 1번째와 (BS/2+1)번째에 대한 
	for ( i=0 ; i < cn ; i++) {
		for ( j=0 ; j < cm ; j++) {
			Real[i][j][0]=Complex[i][j][0];
			Real[i+cn][j+cm][0]=Complex[i][j][0];
			Real[i][j][1]=Complex[i][j][1];
			Real[i+cn][j+cm][1]=Complex[i][j][1];
		}
	}


}*/
void RealMat3DTOComplexMat3D( Mat3DDoub & G, Mat3DDoub & ResultArray )
{

	/*========================================================
	//
	//       FFT에 의해서 생성된 Complex MATRIX를 Real MATRIX 변환 후
	//
	//       다시 Complex MATRIX 변환
	//
	//======================================================*/

	//     [  Ar Ai ]
	//     [ -Ai Ar ]    =>  [Ar]+[Ai]*i

	// [nn][mm][oo] -> [n][m][(oo-1)*2]
	// [10][20][5]  -> [5][10][8]
	// 

	int i,j,k;

	int rn=G.dim1();  //2
	int rm=G.dim2();  //2
	int rk=G.dim3();  //5
	int cn=rn/2;      //1
	int cm=rm/2;      //1
	int ck=(rk-1)*2;  //8

	ResultArray.AssignConstant(cn,cm,ck,0.);

	for (k=2 ; k < (ck/2+1) ; k++){
		for( i=0 ; i< cn ;i++){
			for (j=0 ; j< cm ; j++){
				ResultArray[i][j][k*2-2]=G[i][j][k];
				ResultArray[i][j][k*2-1]=G[i][j+cm][k];
			}
		}
	} // for


	// 0번째와 (BS/2+1)번째에 대한
	for ( i=0 ; i < cn ; i++){
			for ( j=0 ; j < cm ; j++){
					ResultArray[i][j][0]=G[i][j][0];
					ResultArray[i][j][1]=G[i][j][1];
			}
	}

}


/*
void RealArrayTOComplexArray( Mat3DDoub & G, Doub Complex[MAX_V][MAX_V][MAX_BS] )
{

	int i,j,k;

	int rn=G.dim1();
	int rm=G.dim2();
	int rk=G.dim3();
	int cn=rn/2;
	int cm=rm/2;
	int ck=(rk-1)*2;

	for (k=1 ; k < (ck/2+1) ; k++){
		for( i=0 ; i< cn ;i++){
			for (j=0 ; j< cm ; j++){
				Complex[i][j][k*2]=G[i][j][k+1];
				Complex[i][j][k*2+1]=G[i][j+cm][k+1];
			}
		}
	} // for

	// 1번째와 (BS/2+1)번째에 대한
	for ( i=0 ; i < cn ; i++){
		for ( j=0 ; j < cm ; j++){
			Complex[i][j][0]=G[i][j][0];
			Complex[i][j][1]=G[i][j][1];
		}
	}
}
*/

// 일반적인 복소수 매트릭스 곱이 아닌 rlft에 의해서 계산된 복소수 계산
void Matrix3DComplexProduct( Mat3DDoub & LMAT
						   , Mat3DDoub & RMAT
						   , Mat3DDoub & ResultArray )
{

	int i, j, k;
	int Ln=LMAT.dim1();
	int Lm=LMAT.dim2();
	int Lk=LMAT.dim3();

	int Rn=RMAT.dim1();
	int Rm=RMAT.dim2();
	int Rk=RMAT.dim3();

	if ( !(Lk==Rk && Lm==Rn) ) {throw(" ");}


	ResultArray.AssignConstant(Ln,Rm,Rk,0.);

	/*========================================================
	//
	//       FFT에 의해서 생성된 Complex MATRIX대상이다.
	//
	//       Complex Value를 2 Real Array로 된 매트릭스에 대한
	//       PRODUCT에 대한 계산 과정이다.
	//       
	//       A.*B		( O )
	//       A.*conj(B) ( X )
	//
	//
	//       a=ar+ai*i;
	//       b=br+bi*i;
	//       c=a*b;
	//
	//       c=cr+ci*i;
	//       cr=ar*br-ai*bi;
	//       ci=ar*bi+ai*br;
	//
	/*======================================================*/

	// 복소수에 대한 처리부분이다.
	for ( k=1 ; k < Rk/2 ; k++ ){
		for ( i=0 ; i < Ln ; i++){
			for ( j=0 ; j < Rm ; j++){

				Doub real=0;	// 임시 저장 Real값
				Doub imag=0;	// 임시 저장 Imag값

				for ( int iv=0 ; iv < Rn ; iv++){

					real	+=	LMAT[i][iv][k*2]* RMAT[iv][j][k*2]
								-LMAT[i][iv][k*2+1]*RMAT[iv][j][k*2+1];

					imag	+=	LMAT[i][iv][k*2]* RMAT[iv][j][k*2+1]
								+LMAT[i][iv][k*2+1]*RMAT[iv][j][k*2];

				}
				ResultArray[i][j][k*2]=real;
				ResultArray[i][j][k*2+1]=imag;
			}
		}
	}

	// 0과 BS/2+1 실수에 대한 처리 부분이다.
	for ( k=0 ; k < 2 ; k++ ){
		for ( i=0 ; i < Ln ; i++ ){
			for ( j=0 ; j < Rm ; j++ ){

				Doub real=0;

				for ( int iv=0 ; iv < Rn ; iv++ )
				{
					real += LMAT[i][iv][k]*RMAT[iv][j][k];
				}

				ResultArray[i][j][k]=real;

			}
		}
	}

}

void Matrix3DComplexProduct( Mat3DDoub & LMAT, MatDoub & RMAT, MatDoub & ResultArray )
{
	int i,  k;
	int Ln=LMAT.dim1();
	int Lm=LMAT.dim2();
	int Lk=LMAT.dim3();

	int Rn=RMAT.nrows();
	int Rk=RMAT.ncols();

	if ( !(Lk==Rk && Lm==Rn) ) {throw(" ");}


	ResultArray.Assign(Ln,Rk,0.);

	/*========================================================
	//
	//       FFT에 의해서 생성된 Complex MATRIX대상이다.
	//
	//       Complex Value를 2 Real Array로 된 매트릭스에 대한
	//       PRODUCT에 대한 계산 과정이다.
	//       
	//       A.*B		( O )
	//       A.*conj(B) ( X )
	//
	//
	//       a=ar+ai*i;
	//       b=br+bi*i;
	//       c=a*b;
	//
	//       c=cr+ci*i;
	//       cr=ar*br-ai*bi;
	//       ci=ar*bi+ai*br;
	//
	/*======================================================*/

	// 복소수에 대한 처리부분이다.
	for ( k=1 ; k < Rk/2 ; k++ ){
		for ( i=0 ; i < Ln ; i++){


				Doub real=0;	// 임시 저장 Real값
				Doub imag=0;	// 임시 저장 Imag값

				for ( int iv=0 ; iv < Rn ; iv++){

					real	+=	LMAT[i][iv][k*2]* RMAT[iv][k*2]
								-LMAT[i][iv][k*2+1]*RMAT[iv][k*2+1];

					imag	+=	LMAT[i][iv][k*2]* RMAT[iv][k*2+1]
								+LMAT[i][iv][k*2+1]*RMAT[iv][k*2];

				}
				ResultArray[i][k*2]=real;
				ResultArray[i][k*2+1]=imag;
		}
	}

	// 0과 BS/2+1 실수에 대한 처리 부분이다.
	for ( k=0 ; k < 2 ; k++ ){
		for ( i=0 ; i < Ln ; i++ ){

				Doub real=0;

				for ( int iv=0 ; iv < Rn ; iv++ )
				{
					real += LMAT[i][iv][k]*RMAT[iv][k];
				}

				ResultArray[i][k]=real;

			}
	}
}

/*
void Matrix3DComplexProduct2( Doub LMAT[MAX_V][MAX_V][MAX_BS]
							,Doub RMAT[MAX_V][MAX_V][MAX_BS]
							,Doub Complex[MAX_V][MAX_V][MAX_BS]
							,int Size[2][3])
{

							
	int i, j, k;
	int Ln=Size[0][0];
	int Lm=Size[0][1];
	int Lk=Size[0][2];

	int Rn=Size[1][0];
	int Rm=Size[1][1];
	int Rk=Size[1][2];

	if ( !(Lk==Rk && Lm==Rn) ) {throw(" ");}



	/ *========================================================
	//
	//       FFT에 의해서 생성된 Complex MATRIX대상이다.
	//
	//       Complex Value를 2 Real Array로 된 매트릭스에 대한
	//       PRODUCT에 대한 계산 과정이다.
	//       
	//       A.*B		( O )
	//       A.*conj(B) ( X )
	//
	//
	//       a=ar+ai*i;
	//       b=br+bi*i;
	//       c=a*b;
	//
	//       c=cr+ci*i;
	//       cr=ar*br-ai*bi;
	//       ci=ar*bi+ai*br;
	//
	/ *======================================================* /

//	2~BS
	for ( k=1 ; k < Rk/2 ; k++ ){
		for ( i=0 ; i < Ln ; i++){
			for ( j=0 ; j < Rm ; j++){

				Doub real=0;	// 임시 저장 Real값
				Doub imag=0;	// 임시 저장 Imag값

				for ( int iv=0 ; iv < Rn ; iv++){
					
					real	+=	LMAT[i][iv][k]* RMAT[iv][j][k]-LMAT[i][iv][k+1]*RMAT[iv][j][k+1];
					imag	+=	LMAT[i][iv][k]* RMAT[iv][j][k+1]+LMAT[i][iv][k+1]*RMAT[iv][j][k];

				}

			
				Complex[i][j][k*2]=real;
				Complex[i][j][k*2+1]=imag;

			}
		}
	}
		

	for ( k=0 ; k < 2 ; k++ ){
		for ( i=0 ; i < Ln ; i++ ){
			for ( j=0 ; j < Rm ; j++ ){

				Doub real=0;
				
				for ( int iv=0 ; iv < Rn ; iv++ ){
					real += LMAT[i][iv][k]*RMAT[iv][j][k];
				}
				
				Complex[i][j][k]=real;

			}
		}
	}
}*/

void Matrix3DRealProduct( Mat3DDoub & LMAT
						, Mat3DDoub & RMAT
						, Mat3DDoub & ResultArray )
{

	/*========================================================
	//
	//       순전한 Real MATRIX대상이다.
	//
	/*======================================================*/

	int Ln=LMAT.dim1();
	int Lm=LMAT.dim2();
	int Lk=LMAT.dim3();

	int Rn=RMAT.dim1();
	int Rm=RMAT.dim2();
	int Rk=RMAT.dim3();

	if ( !(Lk==Rk && Lm==Rn) ) {throw(" ");}

	ResultArray.AssignConstant(Ln,Rm,Rk,0.);

	for ( int k=0 ; k < Rk ; k++ ){
		for ( int i=0 ; i < Ln ; i++ ){
			for ( int j=0 ; j < Rm ; j++ ){

				Doub real=0;  // 임시 저장 Real값
				
				for ( int iv=0 ; iv < Rn ; iv++ ){
					real += LMAT[i][iv][k]*RMAT[iv][j][k];
				}
				
				ResultArray[i][j][k]=real;

			}
		}
	}
}

void ComplexMatrixToAmplitudeNAngle( Mat3DDoub_I & H
								   , Mat3DDoub &Amplitude
								   , Mat3DDoub &Angle )
{
	int i,j,k;

	// 차원 
	int nn=H.dim1();
	int mm=H.dim2();
	int kk=H.dim3();
	int hk=kk/2+1;

	// 변수 초기화
	Amplitude.AssignConstant(nn,mm,hk,0.);
	Angle.AssignConstant(nn,mm,hk,0.);

	double num=0,ang=0;

	for( k=1 ; k < hk-1 ; k++ ){
		for ( i=0 ; i < nn ; i++ ){
			for ( j=0; j < mm ; j++ )
			{

				num=SQR<double>( H[i][j][k*2])+SQR<double>(H[i][j][k*2+1]);
				ang=tan( -1.*H[i][j][k*2+1] / H[i][j][k*2] );

				Amplitude[i][j][k]	=	pow(num,0.5); 
				Angle[i][j][k]		=   ang;
			}
		}
	}

	// 다시 한번 살펴보자~
	for ( i=0 ; i < nn ; i++ ){
		for ( j=0; j < mm ; j++ )
		{
			Amplitude[i][j][0]=H[i][j][0];
			Amplitude[i][j][hk-1]=H[i][j][1];
		}
	}

}



