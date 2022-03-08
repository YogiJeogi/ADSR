#define PI 3.141592653589793238
#include "spectrum.h"

Doub square(int j,int n) {return 1.;}
Doub bartlett(int j,int n) {return 1.-abs(2.*j/(n-1.)-1.);}
Doub welch(int j,int n) {return 1.-SQR(2.*j/(n-1.)-1.);}

Doub blackman( int j,int n )
	{ double tmp=double(j)/double(n); return 0.42-0.5*cos(2.*PI*(j)/(n-1))+0.08*cos(4.*PI*(j)/(n-1));}

Doub hann( int j,int n )
	{	Doub twopi = 8.*atan(1.);	return 0.5*(1.-cos(j*twopi/(n-1.)));	}

MatDoub Spectreg2D::spectrum()
{
	MatDoub spec(n,m+1);
	if (nsum == 0) throw("no data yet");
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<=m;j++) spec[i][j] = specsum[i][j]/nsum;
	}
	return spec;
}

VecDoub Spectreg2D::frequencies()
{
	VecDoub freq(m+1);
	for (int i=0;i<=m;i++) freq[i] = i*0.5/m;
	return freq;
}

SpectOlap2D::SpectOlap2D( int en,int em,int eolap, int eFs ,int ePoint) 
	: n(en)	, m(em)	, m2(2*m) , nsum(0)	
	, olapshift(eolap) , Fs(eFs) , specsum(en,em+1,0.), wksp(m2)
	, LSavedRespData(0) ,LShiftedData(0) , Point(ePoint)
{
	Last=false;
	signals.Assign(n,Point,0.);
	vecforPSD.Assign(n,m2,0.);
}

void SpectOlap2D::adddataseg( const MatDoub &data, Doub (*window)(int,int) )
{
	if (olapshift==m2 )
	{
		adddata(data,window);
		return;
	}

	int i,j,k;
	int IsAvgEnough;
	int NumberAvg;

	if ( data.ncols() > 0 && n==data.nrows())
	{
		// 입력데이타와 저장된 데이타의 합이 지정된 타켓포인트보다 큰가를 판단
		// -> 작다면
		if ( (LSavedRespData+data.ncols()) < Point )
		{
			for (i=0 ; i< n ; i++)
				for (j=0 ; j < data.ncols() ; j++ )
					signals[i][j+LSavedRespData]=data[i][j];
				
				// Save Index Increment
				LSavedRespData += data.ncols();
		}
		// -> 같거나 크다면
		else
		{
			for (i=0 ; i< n ; i++)
				for (j=0 ; j < (Point-LSavedRespData) ; j++ )
					signals[i][j+LSavedRespData]=data[i][j];
				
				// Save Index Increment
				LSavedRespData = Point;
				Last=true;
		}
	}

	IsAvgEnough=(LSavedRespData-LShiftedData)-m2;
	
	// 마지막 턴에서 샘플이 m2(BS)보다 작을 경우 처리 1
	if ( Last && IsAvgEnough < 0 )
	{
		for (i=0 ; i< n ; i++)
			for (j=0 ; j < (Point-LShiftedData) ; j++ )
				vecforPSD[i][j] = signals[i][j+LShiftedData];

			adddata(vecforPSD,window,(Point-LShiftedData));
	}

	if( IsAvgEnough > -1 ) {

		// 몇번을 에버리징할 수 있는가에 대한 계산
		NumberAvg = double(IsAvgEnough)/double(olapshift) > IsAvgEnough/olapshift 
					? IsAvgEnough/olapshift +1 : IsAvgEnough/olapshift ; 

		//enum { HANN , BARTLETT, WELCH, SQARE }; preferST.h
		for ( i=0 ; i < NumberAvg ; i++)
		{
				
			for ( j=0 ; j < n ; j++)
				for ( k=0 ; k < m2 ; k++)
					vecforPSD[j][k] = signals[j][k+LShiftedData];

			adddata(vecforPSD,window);

			// Shift만큼 올라갈 계산
			LShiftedData += olapshift;

		} 
	}
}

MatDoub SpectOlap2D::spectrum()
{
	MatDoub spec(n,m+1);
	if (nsum == 0) throw("no data yet");
	for (int i=0;i<n;i++)
	{
		for (int j=0;j<=m;j++) spec[i][j] = specsum[i][j]/nsum;
	}
	return spec;
}

VecDoub SpectOlap2D::frequencies()
{
	VecDoub freq(m+1);
	for (int i=0;i<=m;i++) freq[i] = i*0.5/m*Fs;
	return freq;
}

void SpectOlap2D::adddata( const MatDoub &data, Doub (*window)(int, int) ,int lp)
{
	for( int i=0 ; i < data.nrows() ; i++ )
	{
		Doub w,fac,sumw = 0.;
		if (data.nrows() != n ) throw("wrong size Dimention");
		if (data.ncols() != m2) throw("wrong size data segment");
		for (int j=0;j<m2;j++) {
			w = window(j,m2);
			wksp[j] = w*data[i][j];
			if ( lp == 0 ) sumw += SQR(w);
			else if( lp !=0 && j < lp )	
				sumw += SQR(w);
		}
		fac = 2./(sumw*Fs);
		realft(wksp,1);
		specsum[i][0] += 0.5*fac*SQR(wksp[0]);
		for (int j=1;j<m;j++) specsum[i][j] += fac*(SQR(wksp[2*j])+SQR(wksp[2*j+1]));
		specsum[i][m] += 0.5*fac*SQR(wksp[1]);
	}
	nsum++;
}

