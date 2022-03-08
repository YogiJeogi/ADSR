#include "SI_class.h"

//#define __DEBUG__1

SYSTEMIDENTIFICATION::SYSTEMIDENTIFICATION(SYSTEMSETTING  &SS) 
{
	SIInitialization(SS);
}

SYSTEMIDENTIFICATION::SYSTEMIDENTIFICATION()
{
	m_tnsum_psd=0;
	m_tnsum_avg=0;
	m_tnsum_fft=0;

	// 비동기 데이타 측정으로 인한 측정 포인트
	// 현상태의 측정 포인트가 되겠다.
	m_nLSavedRespData=0;	
	m_nLShiftedData=0;			// 오버랩에 의한 이동 포인트

	// 시스템 상태를 나타내는 플래그를 선언하였다.
	m_bIsFFTReady				=		false;
	m_bIsPSDReady				=		false;
	m_bIsRespDataFull			=		false;
	m_bIsHReady					=		false;
	m_bIsWhiteNoiseReady		=		false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void SYSTEMIDENTIFICATION::SIInitialization( const SYSTEMSETTING & SS )
{
    
	nNExc						=	SS.FRONTEND.m_nInput_Channels;
	nNPriResp					=	SS.FRONTEND.m_nResponse_Channels;
	nSamplingFreq				=	SS.FRONTEND.m_nSamplingFreq;
	nLBlockSize					=	SS.SPECTRAL.m_nBlocksize;
	nStartFrqIndex				=	SS.SPECTRAL.m_nStartFreqIndex;		// 주파수를 뜻하는 포인트다.
	nStopFrqIndex				=	SS.SPECTRAL.m_nStopFreqIndex;
	nShiftPoints				=	SS.SPECTRAL.m_nPshifted;
	nWindow						=	SS.SPECTRAL.m_nWindow;
	nCycAvg						=	SS.SPECTRAL.m_nCyclicAveg;
	nSignalsPoints				=	SS.SI.m_nSignalsPoints;
	nWhiteNoiseAdded			=	SS.SI.m_nWhiteNoiseAdded;
	nWhiteNoiseBorderFreq		=	SS.SI.m_nWhiteNoiseBorderFreq;
	nWHiteNoiseBorderFreqIndex  =	SS.SI.m_nWhiteNoiseBorderFreqIndex;
	dbWhiteNoiseAlpha			=	SS.SI.m_dbWhiteNoiseAlpha;
	dbWhiteNoiseSTD				=	SS.SI.m_dbWhiteNoiseSTD;

	//
	m_tnsum_psd=0;
	m_tnsum_avg=0;
	m_tnsum_fft=0;

	m_nLSavedRespData=0;
	m_nLShiftedData=0;

	// 플래그를 초기화 or 재 초기화가 되겠다.
	m_bIsFFTReady				=		false;
	m_bIsPSDReady				=		false;
	m_bIsRespDataFull			=		false;
	m_bIsHReady					=		false;
	m_bIsWhiteNoiseReady		=		false;

	Xavg.Assign(nNExc	    ,nLBlockSize,0.);
	Yavg.Assign(nNPriResp   ,nLBlockSize,0.);

	m_WHN_driver.Assign(nNExc   ,nSignalsPoints,0.);
	m_Response.Assign(nNPriResp ,nSignalsPoints,0.);

	m_Xfft.Assign(nNExc	   ,nLBlockSize,0.);
	m_Yfft.Assign(nNPriResp,nLBlockSize,0.);

	m_Gxx.AssignConstant(nNExc	  ,nNExc	,nLBlockSize,0.);
    m_Gyy.AssignConstant(nNPriResp,nNPriResp,nLBlockSize,0.);
	m_Gxy.AssignConstant(nNExc	  ,nNPriResp,nLBlockSize,0.);
	m_Gyx.AssignConstant(nNPriResp,nNExc	,nLBlockSize,0.);

	// To do More
	m_oCOH.AssignConstant(nNPriResp,nNExc,nLBlockSize/2+1,0.);

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
void SYSTEMIDENTIFICATION::InsertResponseSignals( MatDoub & Response , bool IsFirst )
{

	int i,j,k;
	int Number_Of_Frame_of_Cyclic_Averaging_Blocks;
	int Index_of_Frame_of_Cyclic_Averaging;
	int IsCyclicAveragingEnough;
	
	if ( IsFirst==true)
	{
		
		Xavg.Assign(nNExc	 ,nLBlockSize,0.);
		Yavg.Assign(nNPriResp,nLBlockSize,0.);
		m_Xfft.Assign(nNExc	   ,nLBlockSize,0.);
		m_Yfft.Assign(nNPriResp,nLBlockSize,0.);
		m_Gxx.AssignConstant(nNExc	  ,nNExc	,nLBlockSize,0.);
		m_Gyy.AssignConstant(nNPriResp,nNPriResp,nLBlockSize,0.);
		m_Gxy.AssignConstant(nNExc	  ,nNPriResp,nLBlockSize,0.);
		m_Gyx.AssignConstant(nNPriResp,nNExc	,nLBlockSize,0.);


		m_tnsum_avg			= 0;
		m_tnsum_fft			= 0;
		m_nLShiftedData		= 0;
		m_nLSavedRespData	= 0;

		// 응답을 다시 받는 경우에 대비한
		// 다음 행할 것에 대한 플래그 초기화
		m_bIsFFTReady		   =	false;
		m_bIsPSDReady		   =	false;
		m_bIsHReady			   =	false;
		m_bIsINVHReady         =    false;
		m_bIsRespDataFull	   =	false;

	}
/*========================================================
//
//        RESPONSE DATA 저장
//
/*======================================================*/
	// Respose 데이타를 m_Respose에 추가 Push_back
	// 예외 처리문 필요
	if ( Response.ncols() > 0 && nNPriResp==Response.nrows())
	{
		// 입력데이타와 저장된 데이타의 합이 지정된 타켓포인트보다 큰가를 판단
		// -> 작다면
		if ( (m_nLSavedRespData+Response.ncols()) < nSignalsPoints ){
			for (i=0 ; i< nNPriResp ; i++)
				for (j=0 ; j < Response.ncols() ; j++ ){
					m_Response[i][j+m_nLSavedRespData]=Response[i][j];
				}
			// Save Index Increment
			m_nLSavedRespData += Response.ncols();
		}
		// -> 같거나 크다면
		else
		{
			for (i=0 ; i< nNPriResp ; i++)
				for (j=0 ; j < (nSignalsPoints-m_nLSavedRespData) ; j++ )
				{
					m_Response[i][j+m_nLSavedRespData]=Response[i][j];
				}
			// Save Index Increment
			m_nLSavedRespData = nSignalsPoints;
			// Set Up Flag
			m_bIsRespDataFull=true;
		}
	}
	else{ throw(" Response Data NOT match or No DATA"); }

	// Overlap Shift해서 Nc*BS보다 마니 남아 있다면 Avg을 해라.
	IsCyclicAveragingEnough=(m_nLSavedRespData-m_nLShiftedData)-nLBlockSize*nCycAvg;

/*========================================================
//
//        Cyclic Averaging과 FFT
//
/*======================================================*/

	
	if( IsCyclicAveragingEnough > -1 ) {

		// 몇번을 에버리징할 수 있는가에 대한 계산
		Number_Of_Frame_of_Cyclic_Averaging_Blocks = (IsCyclicAveragingEnough)/nShiftPoints+1; 

		//enum { HANN , BARTLETT, WELCH, SQARE }; preferST.h
		for ( Index_of_Frame_of_Cyclic_Averaging=0 ; Index_of_Frame_of_Cyclic_Averaging < Number_Of_Frame_of_Cyclic_Averaging_Blocks ; Index_of_Frame_of_Cyclic_Averaging++)
		{
			
		    // 임시 저장 초기화 Xavg,Yavg================================================
			Xavg.Assign(nNExc,nLBlockSize,0.);
			Yavg.Assign(nNPriResp,nLBlockSize,0.);

			for ( i=0 ; i < nCycAvg ; i++ )
			{ // Cyclic Avg
				//== X avg==
				for ( j=0 ; j < nNExc ; j++){
					for ( k=0 ; k < nLBlockSize ; k++)
						switch(nWindow)
						{
							case HANN:		// 0
								Xavg[j][k] += hann(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_WHN_driver[j][k+(nLBlockSize*i)+m_nLShiftedData];
								break;
							case BARTLETT:
								Xavg[j][k] += bartlett(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_WHN_driver[j][k+(nLBlockSize*i)+m_nLShiftedData];
								break;
							case WELCH:
								Xavg[j][k] += welch(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_WHN_driver[j][k+(nLBlockSize*i)+m_nLShiftedData];
								break;
							case SQARE: //3
								Xavg[j][k] += (square(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_WHN_driver[j][k+(nLBlockSize*i)+m_nLShiftedData]);
								break;								
						}						
				} // for Xavg
				//== Y avg==
				for ( j=0 ; j < nNPriResp ; j++){
					for ( k=0 ; k < nLBlockSize ; k++)
						switch(nWindow)
						{
							case HANN:
								Yavg[j][k] += hann(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_Response[j][k+(nLBlockSize*i)+m_nLShiftedData];
								break;
							case BARTLETT:
								Yavg[j][k] += bartlett(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_Response[j][k+(nLBlockSize*i)+m_nLShiftedData];
								break;
							case WELCH:
								Yavg[j][k] += welch(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_Response[j][k+(nLBlockSize*i)+m_nLShiftedData];
								break;
							case SQARE:
								Yavg[j][k] += (square(k+(nLBlockSize*i),nCycAvg*nLBlockSize)
									*m_Response[j][k+(nLBlockSize*i)+m_nLShiftedData]);
								break;									
						}
						
				} // == Y avg==
			} // for ( i=0 ; i < m_SST.SPECTRAL.m_nCyclic_Aveg ; i++ ){ 
			
			// Shift만큼 올라갈 계산
			m_nLShiftedData += nShiftPoints;
			
			// 앞서 계산과정은 덧샘뿐이였다. 평균을 구하면                     //
			//== X avg==													   //	
// 			for ( j=0 ; j < nNExc ; j++){            // 
// 					for ( k=0 ; k < nLBlockSize ; k++)     
// 						Xavg[j][k] /= double(nCycAvg);
// 			}
// 			//== Y avg==
// 			for ( j=0 ; j < nNPriResp ; j++){
// 					for ( k=0 ; k < nLBlockSize ; k++)
// 						Yavg[j][k] /= double(nCycAvg);
// 			}

			// 정적 메모리
			/*========================================================
			//       FFT X,Y
			/*======================================================*/
			// X
			for ( j=0 ; j < nNExc ; j++) 
					rlftin3D(Xavg[j],nLBlockSize,1);
			// Y 
			for ( j=0 ; j < nNPriResp ; j++)
			        rlftin3D(Yavg[j],nLBlockSize,1);
			 
			/*======================================================*/	

   
			for ( j=0 ; j < nNExc ; j++)
				for ( k=0 ; k < nLBlockSize ; k++)
					/*m_Xfft[j][k] = m_Xfft[j][k]+Xavg[j][k];*/
					m_Xfft[j][k] =Xavg[j][k]*2/(nLBlockSize);
			for ( j=0 ; j < nNPriResp ; j++)
				for ( k=0 ; k < nLBlockSize ; k++)
					/*m_Yfft[j][k] = m_Yfft[j][k]+Yavg[j][k];*/
					m_Yfft[j][k] =Yavg[j][k]*2/(nLBlockSize);

			m_tnsum_fft++;

			m_bIsFFTReady=true;  // Flag 선언
			calculatePSD();
		} // for ( tavg=0 ; tavg < tnavg ; tavg++){

// 		m_bIsFFTReady=true;  // Flag 선언
// 		calculatePSD();

		if(m_bIsRespDataFull)
		{
			//CalculatePSD();
			CalculateH();
		}
    }  //if( tnavg >0 ) {
/*========================================================
//
//       END Cyclic Averaging과 FFT
//
/*======================================================*/
}
///////////////////////////////////////////////////////////////////////////
void SYSTEMIDENTIFICATION::calculatePSD(){

	//  여기~~~ 
	if (!(m_bIsFFTReady==true)) return ;
		// throw( " No fft result ");

	int i,j,k;  //Iteration variable
	double ScaleFactor=nLBlockSize*nLBlockSize;

//	double ScaleFactor=double(nLBlockSize*SQR(m_tnsum_fft));


/*========================================================
//
//       Gxx,Gxy,Gyx,Gyy 
//       A.*conj(B)/BS
//
/*======================================================*/
	/*========================================================
	//        Gxx
	/*======================================================*/
	
	//       cr=ar*br-ai*bi;
	//       ci=ar*bi+ai*br;

	for (i=0; i< nNExc ; i++){
		for(j=0; j< nNExc ; j++){
			for(k=1; k< nLBlockSize/2 ; k++){

				m_Gxx[i][j][2*k]	+=	(m_Xfft[i][2*k]*m_Xfft[j][2*k]+m_Xfft[i][2*k+1]*m_Xfft[j][2*k+1])/ScaleFactor;
				m_Gxx[i][j][2*k+1]	+=	(-m_Xfft[i][2*k]*m_Xfft[j][2*k+1]+m_Xfft[i][2*k+1]*m_Xfft[j][2*k])/ScaleFactor;
			}
			m_Gxx[i][j][0]+=m_Xfft[i][0]*m_Xfft[j][0]/ScaleFactor;
			m_Gxx[i][j][1]+=m_Xfft[i][1]*m_Xfft[j][1]/ScaleFactor;
		}
	}
 
	/*========================================================
	//        Gyy
	/*======================================================*/
	for (i=0; i< nNPriResp ; i++){
		for(j=0; j< nNPriResp ; j++){
			for(k=1; k< nLBlockSize/2 ; k++){

				m_Gyy[i][j][2*k]	+=	(m_Yfft[i][2*k]*m_Yfft[j][2*k]+m_Yfft[i][2*k+1]*m_Yfft[j][2*k+1])/ScaleFactor;						   
				m_Gyy[i][j][2*k+1]	+=	(-m_Yfft[i][2*k]*m_Yfft[j][2*k+1]+m_Yfft[i][2*k+1]*m_Yfft[j][2*k])/ScaleFactor;

			}
			m_Gyy[i][j][0]	+=	m_Yfft[i][0]*m_Yfft[j][0]/ScaleFactor;
			m_Gyy[i][j][1]	+=	m_Yfft[i][1]*m_Yfft[j][1]/ScaleFactor;
		}
	}

	/*========================================================
	//        Gxy
	/*======================================================*/
	for (i=0; i< nNExc ; i++){
		for(j=0; j< nNPriResp ; j++){
			for(k=1; k< nLBlockSize/2 ; k++){

				m_Gxy[i][j][2*k]	+= (m_Xfft[i][2*k]*m_Yfft[j][2*k]+m_Xfft[i][2*k+1]*m_Yfft[j][2*k+1])/ScaleFactor;										
				m_Gxy[i][j][2*k+1]	+= (-m_Xfft[i][2*k]*m_Yfft[j][2*k+1]+m_Xfft[i][2*k+1]*m_Yfft[j][2*k])/ScaleFactor;
										
			}
			m_Gxy[i][j][0]	+=	m_Xfft[i][0]*m_Yfft[j][0]/ScaleFactor;
			m_Gxy[i][j][1]	+=	m_Xfft[i][1]*m_Yfft[j][1]/ScaleFactor;
		}
	}

	/*========================================================
	//        Gyx
	/*======================================================*/
	for (i=0; i< nNPriResp ; i++){
		for(j=0; j< nNExc ; j++){
			for(k=1; k< nLBlockSize/2 ; k++){
			
				m_Gyx[i][j][2*k]   += (m_Yfft[i][2*k]*m_Xfft[j][2*k]+m_Yfft[i][2*k+1]*m_Xfft[j][2*k+1])/ScaleFactor; 
				m_Gyx[i][j][2*k+1] += (-m_Yfft[i][2*k]*m_Xfft[j][2*k+1]+m_Yfft[i][2*k+1]*m_Xfft[j][2*k])/ScaleFactor; 
										
			}
			m_Gyx[i][j][0]	+=	m_Yfft[i][0]*m_Xfft[j][0]/ScaleFactor;
			m_Gyx[i][j][1]	+=	m_Yfft[i][1]*m_Xfft[j][1]/ScaleFactor;
		}
	}
	m_bIsPSDReady=true;
}

void SYSTEMIDENTIFICATION::CalculateH()
{
/*========================================================
//
//       Calculate FRF
//
/*======================================================*/
/*	calculatePSD();*/

	if (m_bIsPSDReady != true) return;
    /*=====================
 	//       H1
 	/*===================*/
 	
 	Mat3DDoub Gxx2N,InvGxx;
	
 	ComplexMat3DTORealMat3D(m_Gxx,Gxx2N);
	
  	CPeudoInverse3D PInvGxx(Gxx2N); 

	RealMat3DTOComplexMat3D(PInvGxx.GetPinverseMatrix3D(),InvGxx);

 	Matrix3DComplexProduct(m_Gyx,InvGxx,m_H);

	// 전달함수에 대한 Magnitude와 위상에 대한 계산이 필요함
	ComplexMatrixToAmplitudeNAngle(m_H,m_HAmplitude,m_HAngle);

	m_bIsHReady		=	true;
 
}


void SYSTEMIDENTIFICATION::CalculateINVH()
{

	Mat3DDoub H;
    /*=====================
	//       Inv H1
	/*===================*/
	ComplexMat3DTORealMat3D(m_H,H);

	CPeudoInverse3D PinvH(H); 

	RealMat3DTOComplexMat3D(PinvH.GetPinverseMatrix3D(),m_invH);

	m_bIsINVHReady    =    true;


}

void SYSTEMIDENTIFICATION::CalculateOrdinaryCOH()
{
/*========================================================

       Calculate Coherence


       cr=ar*br-ai*bi;
       ci=ar*bi+ai*br;

/*======================================================*/

    /*=========================================
    // 
	// Ordinary COHERENCE
	// 
	// Complex Value  -> MAGUNITUDE.
	// 
	// 0,BS/2+1,1,2,3....( X )
	// 0,1,2......BS/2+1 ( O )
	// 즉 다시 원래대로 순서를 바꾸었다.
	// 
	/*=========================================*/

	int i,j,k;

	int			iLast=nLBlockSize/2;
	double		num,den;


	for( k=1 ; k < nLBlockSize/2 ; k++ ){
		for ( i=0 ; i < nNExc ; i++ ){
			for ( j=0; j < nNPriResp ; j++ ){
				
				num=SQR( m_Gyx[j][i][k*2]*m_Gxy[i][j][k*2]
				- m_Gyx[j][i][k*2+1]*m_Gxy[i][j][k*2+1] )

					+SQR( m_Gyx[j][i][k*2]*m_Gxy[i][j][k*2+1]
				+ m_Gyx[j][i][k*2+1]*m_Gxy[i][j][k*2] );

				den=SQR( m_Gxx[i][i][k*2]*m_Gyy[j][j][k*2]
				- m_Gxx[i][i][k*2+1]*m_Gyy[j][j][k*2+1] )

					+SQR( m_Gxx[i][i][k*2]*m_Gyy[j][j][k*2+1]
				+ m_Gxx[i][i][k*2+1]*m_Gyy[j][j][k*2] );

				m_oCOH[j][i][k]	=pow((num/den),0.5); 

			}
		}
	}
	
	// 다시 한번 살펴보자~
	for ( i=0 ; i < nNExc ; i++ ){
		for ( j=0; j < nNPriResp ; j++ ){
			m_oCOH[j][i][0]	    =(m_Gyx[j][i][0]*m_Gxy[i][j][0])/(m_Gxx[i][i][0]*m_Gyy[j][j][0]);
			m_oCOH[j][i][iLast]	=(m_Gyx[j][i][1]*m_Gxy[i][j][1])/(m_Gxx[i][i][1]*m_Gyy[j][j][1]);
		}
	}

}


void SYSTEMIDENTIFICATION::GenerateWhiteNoise()
{

	double nScale_to_cF_alpha_to_one;
	double nDeltaFreq=1./double(nLBlockSize)*nSamplingFreq;

	m_WHN_driver.resize(nNExc,nSignalsPoints);

	/* =======================================================
	F=(1/nDelta_F:nDelta_F:Fs/2)';    
	cF_alpha_to_one=1./(F.^nAlpha);
	==========================================================*/

	VecDoub F(nLBlockSize,nDeltaFreq,true); // length(F)=BS/2 ; 1024/2=512;
	F.powerElementAlpha(dbWhiteNoiseAlpha);
	F.divideScalar(1, true); // 분자이면 nscalar/element 계산

	/* ============================================================
	nValue_iBorder_Frequency=cF_alpha_to_one(iBorderFre);
	cF_alpha_to_one(1:iBorderFre)=nValue_iBorder_Frequency; 	
	==============================================================*/
	F.assignElementOnRange(0,nWHiteNoiseBorderFreqIndex+1,F[nWHiteNoiseBorderFreqIndex]);
	F.assignElementOnRange(0,nStartFrqIndex,0);          // 1번 Entry 포함 2번 엔트리 제외 [1 2)
	F.assignElementOnRange(nStopFrqIndex+1,nLBlockSize,0);

	/* ============================================================
	 nScale_to_cF_alpha_to_one=nSTD_White_Noise^2 ...
                           /sum(cF_alpha_to_one)/2/nDelta_F;
	==============================================================*/
	nScale_to_cF_alpha_to_one=double(pow(dbWhiteNoiseSTD,2))
							 /double(F.sumAllElement())/(2.)/double(nDeltaFreq);


	/* ============================================================
	  cPSDss=2*nScale_to_cF_alpha_to_one*cF_alpha_to_one;
	  cPSDss=[0 ;cPSDss];  % Rejection of 0Hz ( DC Rejection ) 
	  cPSDds=cPSDss/2;
	  cSpectrum_Amplitude = sqrt(cPSDds*nBlock_Size*Fs);
	==============================================================*/
	F.multiScalar(nScale_to_cF_alpha_to_one); // CPSDds
	F.multiScalar(double(nLBlockSize*nSamplingFreq));
	F.sqrtElement();

	// Temparary Variable
	VecDoub tmpVec(nLBlockSize); 

	// Index
	int tmpIndex=0;


	// 채널 수
	for ( int k=0 ; k < nNExc ; k++)
	{
		//  Seed 초기화
		Ranq2 Rnumber(rand()); 
		// 채널당 화이트 수
		for (int i=0;i < /*  */ nWhiteNoiseAdded /*  */ ;i++)
		{
			// 화이트의 포인트 수
			for (int j=1 ; j < nLBlockSize/2 ;j++)
			{
				double tmp_Phase=2*PI*Rnumber.doub();
				tmpVec[2*j]=F[j]*cos(tmp_Phase);
				tmpVec[2*j+1]=F[j]*sin(tmp_Phase);  
			} 
			tmpVec[0]=0;
			tmpVec[1]=0; //처음값과 BS/2+1값은 0이다. 무조건~ 

			// IFFT
			realft(tmpVec,-1); 

			// 대입
		    for( int ii=0 ; ii < nLBlockSize ; ii++ )
			{
				m_WHN_driver[k][ii+i*nLBlockSize]=2./double(nLBlockSize)*tmpVec[ii];
			}
		} 
	}	

	/* ==========================================================
	 * 화이트 노이즈를 생선한 시점은 다음 단계는 모두 초기화된다.
	 * =========================================================*/
	m_bIsWhiteNoiseReady=true;
	// 플래그 초기화 or 리셋
	m_bIsRespDataFull	=false;
	m_bIsHReady		    =false;
	m_bIsINVHReady	=false;
	m_bIsFFTReady		=false;
	m_bIsPSDReady		=false;

}



void SYSTEMIDENTIFICATION::debugging()
{
	cout <<"m_WHN_driver"<<endl;
//	for (int i=0 ; i<55 ; i++) cout << m_WHN_driver[0][i] << endl;
	cout << m_WHN_driver[0][0] << " " << m_WHN_driver[0][1] << " " << m_WHN_driver[0][2] << " " << m_WHN_driver[0][3] << " " << m_WHN_driver[0][4] << " " << m_WHN_driver[0][5] << " " << m_WHN_driver[0][6] << " " << m_WHN_driver[0][7] << " " <<endl;
	cout <<" m_Xfft"<<endl;
	cout << m_Xfft[0][0] << " " << m_Xfft[0][1] << " " << m_Xfft[0][2] << " " << m_Xfft[0][3] << " " << m_Xfft[0][4] << " " << m_Xfft[0][5] << " " << m_Xfft[0][6] << " " << m_Xfft[0][7] << " " <<endl;
	cout << "m_Response "<<endl;
	cout << m_Response[0][0] << " " << m_Response[0][1] << " " << m_Response[0][2] << " " << m_Response[0][3] << " " << m_Response[0][4] << " " << m_Response[0][5] << " " << m_Response[0][6] << " " << m_Response[0][7] << " " <<endl;
 	cout <<" m_Yfft"<<endl;
 	cout << m_Yfft[0][0] << " " << m_Yfft[0][1] << " " << m_Yfft[0][2] << " " << m_Yfft[0][3] << " " << m_Yfft[0][4] << " " << m_Yfft[0][5] << " " << m_Yfft[0][6] << " " << m_Yfft[0][7] << " " <<endl;
 	cout << " m_Gxx"<<endl;
 	cout << m_Gxx[0][0][0] << " " << m_Gxx[0][0][1] << " " <<m_Gxx[0][0][2] << " " <<m_Gxx[0][0][3] << " " <<m_Gxx[0][0][4] << " " <<m_Gxx[0][0][5] << " " <<m_Gxx[0][0][6] << " " <<m_Gxx[0][0][7] << " " <<endl;
 	cout << " m_Gyx"<<endl;
 	cout << m_Gyx[0][0][0] << " " << m_Gyx[0][0][1] << " " <<m_Gyx[0][0][2] << " " <<m_Gyx[0][0][3] << " " <<m_Gyx[0][0][4] << " " <<m_Gyx[0][0][5] << " " <<m_Gyx[0][0][6] << " " <<m_Gyx[0][0][7] << " " <<endl;
	cout << " m_H "<<endl;
	cout << m_H[0][0][0] << " " << m_H[0][0][1] << " " <<m_H[0][0][2] << " " <<m_H[0][0][3] << " " <<m_H[0][0][4] << " " <<m_H[0][0][5] << " " <<m_H[0][0][6] << " " <<m_H[0][0][7] << " " <<endl;
	cout << " m_invH "<<endl;
	cout << m_invH[0][0][0] << " " << m_invH[0][0][1] << " " <<m_invH[0][0][2] << " " <<m_invH[0][0][3] << " " <<m_invH[0][0][4] << " " <<m_invH[0][0][5] << " " <<m_invH[0][0][6] << " " <<m_invH[0][0][7] << " " <<endl;
	cout << " m_invH Size " << endl;
	cout << m_invH.dim1() << " " << m_invH.dim2()<< " " << m_invH.dim3() << endl;
	cout << " WHN Size " <<endl;
	cout << m_WHN_driver.nrows() << " " << m_WHN_driver.ncols()<<endl;
}

MatDoub SYSTEMIDENTIFICATION::GetWhiteNoise  (){
	return m_WHN_driver;
}

MatDoub SYSTEMIDENTIFICATION::GetResponse()
{
	return m_Response;
}

Mat3DDoub SYSTEMIDENTIFICATION::GetInvHModel() const 
{
  return m_invH;
}

void SYSTEMIDENTIFICATION::GetHAmplitudeModel( Mat3DDoub & Amplitude,Mat3DDoub & Angle )
{
	Amplitude	=	m_HAmplitude;
	Angle		=	m_HAngle;
}

Mat3DDoub SYSTEMIDENTIFICATION::GetHModel() const
{
	return m_H;
}
