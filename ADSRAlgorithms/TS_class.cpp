#include "TS_class.h"

TargetSimulationBase::TargetSimulationBase()
{
	m_pInstUnfiltRespPSD=NULL;
	m_pInstFiltRespPSD=NULL;

	m_pInstFiltDesiredPSD=NULL;
	m_pInstUnfiltDesiredPSD=NULL;
}

void TargetSimulationBase::TSInitialization( const SYSTEMSETTING & SS )
{
	// 임시 변수 선언
	nNExc				=   SS.FRONTEND.m_nInput_Channels;
	nNPriResp			=   SS.FRONTEND.m_nResponse_Channels;
	nNSlavResp			=	SS.FRONTEND.m_nSlave_Channels;
	nSamplingFreq		=   SS.FRONTEND.m_nSamplingFreq;
	nStartFrqIndex	=   SS.SPECTRAL.m_nStartFreqIndex;
	nStopFrqIndex		=   SS.SPECTRAL.m_nStopFreqIndex;
	nShiftPoints         =	SS.SPECTRAL.m_nPshifted;
	nLBlockSize			=	SS.SPECTRAL.m_nBlocksize;
	nNTarSignals		=	SS.TS.m_nNumberOfChannelOfDesiredSignals;
	nNTarSignalPoint	=	SS.TS.m_nNumberOfPointinDesiredSignals;  // 목표신호의 포인수 수이다. 다만 BS의 배수는 아니다.
	nNBlockSize			=	SS.TS.m_nNumberOfBlockSize;
	nNRespC				=	nNPriResp + nNSlavResp;
	nNSegment			=	nLBlockSize*nNBlockSize;

	pWhetherSubTargetExist = new bool[nNSlavResp];

	for ( int i=0; i < nNSlavResp ; i++ ){
		pWhetherSubTargetExist[i]=SS.TS.m_bSubMatchedDesiredSignals[i];
	}

	// Member Variables Initialization
	m_matDesiredSignals.AssignConstant (2,nNTarSignals,nNBlockSize*nLBlockSize,0.);
	m_matResponseSignals.AssignConstant(2,nNRespC	  ,nNBlockSize*nLBlockSize,	0.);
	m_matErrorSignals.AssignConstant   (2,nNTarSignals,nNBlockSize*nLBlockSize,0.);

	m_matConvInvH.AssignConstant(nNExc	,nNPriResp,	nLBlockSize*2,0.);
	m_matDriveSignals.Assign	(nNExc  ,nNBlockSize*nLBlockSize ,0.);

	FilterDesign();

	// Flag Initialization
	m_fIsFirstDrive							=	true;
	m_fIsDriveSignalsReady					=   false;

	// Set up Initialization Flag
	m_fIsTSInitialized=true;
}

void TargetSimulationBase::TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals )
{
	// 임시 변수 선언
	nNExc				=   SS.FRONTEND.m_nInput_Channels;
	nNPriResp			=   SS.FRONTEND.m_nResponse_Channels;
	nNSlavResp			=	SS.FRONTEND.m_nSlave_Channels;
	nSamplingFreq		=   SS.FRONTEND.m_nSamplingFreq;
	nStartFrqIndex		=   SS.SPECTRAL.m_nStartFreqIndex;
	nStopFrqIndex		=   SS.SPECTRAL.m_nStopFreqIndex;
	nShiftPoints         =	SS.SPECTRAL.m_nPshifted;
	nLBlockSize			=	SS.SPECTRAL.m_nBlocksize;
	nNTarSignals		=	SS.TS.m_nNumberOfChannelOfDesiredSignals;
	nNTarSignalPoint	=	SS.TS.m_nNumberOfPointinDesiredSignals;  // 원 목표신호의 포인수다. 다만 BS의 배수는 아니다.
	nNBlockSize			=	SS.TS.m_nNumberOfBlockSize;
	nNRespC				=	nNPriResp + nNSlavResp;
	nNSegment			=	nLBlockSize*nNBlockSize;

	pWhetherSubTargetExist = new bool[nNSlavResp];

	for ( int i=0; i < nNSlavResp ; i++ )
	{
		pWhetherSubTargetExist[i]=SS.TS.m_bSubMatchedDesiredSignals[i];
	}

	// Member Variables Initialization
	m_matDesiredSignals.AssignConstant (2,nNTarSignals,nNBlockSize*nLBlockSize,0.);
	m_matResponseSignals.AssignConstant(2,nNRespC	  ,nNBlockSize*nLBlockSize,	0.);
	m_matErrorSignals.AssignConstant   (2,nNTarSignals,nNBlockSize*nLBlockSize,0.);

	m_matConvInvH.AssignConstant(nNExc	,nNPriResp,	nLBlockSize*2,0.);
	m_matDriveSignals.Assign	(nNExc  ,nNBlockSize*nLBlockSize ,0.);

	FilterDesign();

	// Flag Initialization
	m_fIsFirstDrive							=	true;
	m_fIsDriveSignalsReady					=   false;

	// Set up Initialization Flag
	m_fIsTSInitialized=true;

	// Insert Parameters into Member Variables
	ImportInverseModel(matInvH);
	ImportDesiredSignals(matDesiredSignals);



}

void TargetSimulationBase::FilterDesign()
{
	// 필터 윈도우
	m_vecFilter.assign(2*nLBlockSize,0.);

	int BS=nLBlockSize;
	m_nkdelay=nLBlockSize-(nLBlockSize/2+1);
	m_nFIRQtap=nLBlockSize-1;

	VecDoub vecShpFilter;
	
	vecShpFilter.assign(nLBlockSize,0.);


	// FIR Filter Design
	if ( nStopFrqIndex == (nSamplingFreq/2*nLBlockSize/nSamplingFreq))
	{
		for (int k=nStartFrqIndex ; k < nStopFrqIndex ; k++ )
			vecShpFilter[2*k]=1.;
		vecShpFilter[1]=1.;
	}
	else
	{
		for (int k=nStartFrqIndex ; k < nStopFrqIndex+1 ; k++ )
			vecShpFilter[2*k]=1.;
	}

	//
	realft(vecShpFilter,-1);

	for ( int k=0; k < m_nkdelay+1 ; k++ )
	{
			m_vecFilter[m_nkdelay+k]=vecShpFilter[k]	* 2./nLBlockSize /*IFFT Scale*/;
			m_vecFilter[m_nkdelay-k]=vecShpFilter[k]	* 2./nLBlockSize /*IFFT Scale*/;
	}

	// Window Method
	for ( int i=0 ; i < m_nFIRQtap ; i++ )
		m_vecFilter[i] *= blackman(i,m_nFIRQtap);

	realft(m_vecFilter,1);

}

void TargetSimulationBase::PadingDesiredSignals( int point )
{
	int j,k;

	for (j=0 ; j < nNTarSignals ; j++){
		for ( k=0 ; k < point ; k++)
		{
			m_matDesiredSignals[0][j][k]=m_matDesiredSignals[0][j][k]*k/point;
			m_matDesiredSignals[0][j][nNTarSignalPoint-k]
						=m_matDesiredSignals[0][j][nNTarSignalPoint-k]*k/point;
		}
	}
}

void TargetSimulationBase::PadingRespSignals( int point )
{
	int j,k;

	for (j=0 ; j < nNRespC ; j++){
		for ( k=0 ; k < point ; k++)
		{
			m_matResponseSignals[0][j][k]=m_matResponseSignals[0][j][k]*k/point;
			m_matResponseSignals[0][j][nNTarSignalPoint-k]
			=m_matResponseSignals[0][j][nNTarSignalPoint-k]*k/point;
		}
	}
}

void TargetSimulationBase::ImportSystemsetting( SYSTEMSETTING & SS )
{
	// Was TS Object Initialized? 
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

// 	// Save SYSTEM Setting
// 	m_SST	=	SS;

	// Set up First TS Flag
	m_fIsFirstDrive	=	true;
}

void TargetSimulationBase::ImportInverseModel(const Mat3DDoub & matInvH)
{
	// Was TS Object Initialized? 
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Index Variables 
	int i,j,k;

	// Temparary Matrix
	Mat3DDoub matInverseH;
	MatDoub   matPartialInvH;
	MatDoub   matPartialInvH2BS;

	// Subsitution
	matInverseH=matInvH;

	// Initialization
	matPartialInvH.Assign(nNPriResp,nLBlockSize,0.);
	matPartialInvH2BS.Assign(nNPriResp,nLBlockSize*2,0.);

	// Bandwidth 
	for ( i=0 ; i < nNExc ; i++)
		for ( j=0 ; j < nNPriResp ; j++)
			for ( k=1 ; k <  nStartFrqIndex ; k++ ){
				matInverseH[i][j][k*2]=0.;
				matInverseH[i][j][k*2+1]=0.;
			}

	for ( i=0 ; i < nNExc ; i++)
		for ( j=0 ; j < nNPriResp ; j++)
			for ( k=nStopFrqIndex+1 ; k < nLBlockSize/2 ; k++ ){
				matInverseH[i][j][k*2]=0.;
				matInverseH[i][j][k*2+1]=0.;
			}
	if ( nStartFrqIndex > 0) 	
		for ( i=0 ; i < nNExc ; i++)
			for ( j=0 ; j < nNPriResp ; j++)
				matInverseH[i][j][0]=0.;

	if ( nStopFrqIndex < nLBlockSize/2 ) 	
		for ( i=0 ; i < nNExc ; i++)
			for ( j=0 ; j < nNPriResp ; j++)
				matInverseH[i][j][1]=0.;	
    // End Bandwidth

	// 	// 디버깅
	cout << " matInverseH[0][0][i] Value : " << endl;
	cout << matInverseH[0][0][0] << " " << matInverseH[0][0][1] << " " <<matInverseH[0][0][2] << " " << matInverseH[0][0][3] << " "<<endl;
	cout << matInverseH[0][0][4] << " " << matInverseH[0][0][5] << " " <<matInverseH[0][0][6] << " " << matInverseH[0][0][7] << " "<<endl;
	cout << endl;




	for ( i=0 ; i < nNExc ; i++)
	{
		for ( j=0; j < nNPriResp ; j++ )
			for( k=0; k < nLBlockSize ; k++ )
				matPartialInvH[j][k]=matInverseH[i][j][k];
		
		// Impulse Respose H ( IFFT )
		for ( j=0; j < nNPriResp ; j++ )
			rlftin3D(matPartialInvH[j],nLBlockSize,-1);
			
		// DELAY
		for ( j=0; j < nNPriResp ; j++ ){
			for ( int k=0; k < m_nkdelay ; k++ )
				matPartialInvH2BS[j][k]=matPartialInvH[j][nLBlockSize-m_nkdelay+k]	* 2./nLBlockSize /*IFFT Scale*/;		
			for ( int k=0; k < (nLBlockSize-m_nkdelay) ; k++ )
				matPartialInvH2BS[j][m_nkdelay+k]=matPartialInvH[j][k]	* 2./nLBlockSize /*IFFT Scale*/;
		}



		// 	// 디버깅
		cout << " matPartialInvH2BS[0][0][i] Value : " << endl;
		cout << matPartialInvH2BS[0][0] << " " << matPartialInvH2BS[0][1] << " " <<matPartialInvH2BS[0][2] << " " << matPartialInvH2BS[0][3] << " "<<endl;
		cout << matPartialInvH2BS[0][4] << " " << matPartialInvH2BS[0][5] << " " <<matPartialInvH2BS[0][6] << " " << matPartialInvH2BS[0][7] << " "<<endl;
		cout << endl;

		// FFT
		for ( j=0; j < nNPriResp ; j++ )
			rlftin3D(matPartialInvH2BS[j],nLBlockSize*2,1);

		// 	// 디버깅
		cout << " matPartialInvH2BS[0][0][i] FFT Value : " << endl;
		cout << matPartialInvH2BS[0][0] << " " << matPartialInvH2BS[0][1] << " " <<matPartialInvH2BS[0][2] << " " << matPartialInvH2BS[0][3] << " "<<endl;
		cout << matPartialInvH2BS[0][4] << " " << matPartialInvH2BS[0][5] << " " <<matPartialInvH2BS[0][6] << " " << matPartialInvH2BS[0][7] << " "<<endl;
		cout << endl;

		// 차원 대입
		for ( j=0; j < nNPriResp ; j++ )
			for( k=0; k < nLBlockSize*2 ; k++ )
				m_matConvInvH[i][j][k]=matPartialInvH2BS[j][k];

		// 임시 저장요소 0으로 초기화.
		matPartialInvH2BS.Assign(nNPriResp,nLBlockSize*2,0.);
	}

	// Set up First TS Flag
	m_fIsFirstDrive	=	true;

}
void TargetSimulationBase::ImportDesiredSignals( MatDoub & matDesiredSignals )
{
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Temparary Variables 
	int j,k;

	// m_DesiredSignals의 첫번째 요소에 원래 신호를 집어 넣는다.
	for (j=0 ; j < nNTarSignals ; j++){
		for ( k=0 ; k < nNTarSignalPoint ; k++){
			m_matDesiredSignals[0][j][k]=matDesiredSignals[j][k];
		}
	}
	PadingDesiredSignals(nLBlockSize);
	// filtering matDesiredSignals and PSD
	ProcessRawDesiredSignals();
	m_fIsFirstDrive=true;

}

void TargetSimulationBase::ImportDesiredSignals( MatDoub & matDesiredSignals, int nPaddingpoints )
{
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Temparary Variables 
	int j,k;

	// m_DesiredSignals의 첫번째 요소에 원래 신호를 집어 넣는다.
	for (j=0 ; j < nNTarSignals ; j++){
		for ( k=0 ; k < nNTarSignalPoint ; k++){
			m_matDesiredSignals[0][j][k]=matDesiredSignals[j][k];
		}
	}
	PadingDesiredSignals(nPaddingpoints);
	// filtering matDesiredSignals and PSD
	ProcessRawDesiredSignals();
	m_fIsFirstDrive=true;
}

void TargetSimulationBase::ProcessRawDesiredSignals()
{
	/*========================================================
	//
	//       Desired Signal 데이타 처리
	//        
	//       1 . 신호 필터
	//       2 . 전체 PSD
	//
	/*======================================================*/

	// Was TS Object Initialized? 
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Spectral PSD Instance Initialization
	if( m_pInstUnfiltDesiredPSD != NULL ) delete m_pInstUnfiltDesiredPSD;
	m_pInstUnfiltDesiredPSD=new SpectOlap2D(/*채널 수*/ nNTarSignals,/*BS/2*/ nLBlockSize/2,nShiftPoints,nSamplingFreq,nNTarSignalPoint);
	if( m_pInstFiltDesiredPSD != NULL ) delete m_pInstFiltDesiredPSD;	
	m_pInstFiltDesiredPSD=new SpectOlap2D(/*채널 수*/ nNTarSignals,/*BS/2*/ nLBlockSize/2,nShiftPoints,nSamplingFreq,nNTarSignalPoint);

	// FFT -> Modify FFT -> IFFT -> insert m_DesiredSignlas[1][j][k];
    //                           -> PSD -> insert into m_matFiltDesiredPSD;
	//	m_matDesiredSignals.AssignConstant (2,nNTarSignals,nNBlockSize*nLBlockSize,0.);
	
	// 변수 선언
	int i,j,k;
	Doub real;
	Doub imag;

	MatDoub matTarget,matTargetPre,matTargetforPSD;
	MatDoub matFiltDesiredPSD,matUnfiltDesiredPSD;

	matTargetforPSD.Assign(nNTarSignals,nLBlockSize,0.);
	
	m_nFIRMpoint=2*nLBlockSize-(m_nFIRQtap-1);
	int Overlapaddcount= int(nNTarSignalPoint)/int(m_nFIRMpoint)+1;

	matTargetPre.Assign(nNTarSignals,(m_nFIRMpoint*Overlapaddcount+m_nFIRQtap-1),0.);

	int processedpointsindex=0;

	for ( i=0 ; i < Overlapaddcount ; i++ )
	{
		// 임시 저장 변수 초기화
		matTarget.Assign(nNTarSignals,nLBlockSize*2,0.);

		// 목표 신호 일부분 임시 저장 객체에 저장
		if ( i !=(Overlapaddcount-1) ){
			for ( j=0 ; j < nNTarSignals ; j++)
				for ( k=0 ; k < m_nFIRMpoint ; k++)
					matTarget[j][k]=m_matDesiredSignals[0][j][k+processedpointsindex];
		}
		else
		{
			for ( j=0 ; j < nNTarSignals ; j++)
				for ( k=0 ; k < nNTarSignalPoint-processedpointsindex ; k++)
					matTarget[j][k]=m_matDesiredSignals[0][j][k+processedpointsindex];			
		}


		// FFT Desired Signals
		for ( j=0 ; j < nNTarSignals ; j++)
			rlftin3D(matTarget[j],nLBlockSize*2,1);

		// Convolution
		for ( j=0 ; j < nNTarSignals ; j++){
			for ( k=1 ; k < nLBlockSize ; k++){
				real	=	matTarget[j][k*2]* m_vecFilter[k*2]
							-matTarget[j][k*2+1]*m_vecFilter[k*2+1];

				imag	=	matTarget[j][k*2]* m_vecFilter[k*2+1]
							+matTarget[j][k*2+1]*m_vecFilter[k*2];

				matTarget[j][k*2]=real;
				matTarget[j][k*2+1]=imag;
			}
			matTarget[j][0]=matTarget[j][0]*m_vecFilter[0];
			matTarget[j][1]=matTarget[j][1]*m_vecFilter[1];
		}

		// IFFT Result From Above
		for ( j=0 ; j < nNTarSignals ; j++)
			rlftin3D(matTarget[j],nLBlockSize*2,-1);

 
		// M만큼 m_DriveSignals에 대입
		for ( j=0 ; j < nNTarSignals ; j++ ){
			for ( k=0 ; k < nLBlockSize*2 ; k++){
				matTargetPre[j][k+processedpointsindex]
				+= matTarget[j][k]*2./double(nLBlockSize*2);
			} 
		}
		processedpointsindex += m_nFIRMpoint;

	}

	// 최종 matTargetPre의 kdelay 만큼 딜레이
	for ( j=0 ; j < nNTarSignals ; j++ )
		for ( k=0 ; k < nNTarSignalPoint ; k++)
			m_matDesiredSignals[1][j][k]=matTargetPre[j][k+m_nkdelay];


	//  PSD	
	matTargetforPSD.Assign(nNTarSignals,nLBlockSize,0.);

	for ( i=0 ; i < nNBlockSize ; i++ ){
		for ( j=0 ; j < nNTarSignals ; j++ ){		
			for ( k=0 ; k < nLBlockSize ; k++)
				matTargetforPSD[j][k]=m_matDesiredSignals[1][j][k+i*nLBlockSize];
		}
		m_pInstFiltDesiredPSD->adddataseg(matTargetforPSD,hann);

		for ( j=0 ; j < nNTarSignals ; j++ ){
			for ( k=0 ; k < nLBlockSize ; k++)
				matTargetforPSD[j][k]=m_matDesiredSignals[0][j][k+i*nLBlockSize];
		}
		m_pInstUnfiltDesiredPSD->adddataseg(matTargetforPSD,hann);
	}

	// Insert into m_matFiltDesiredPSD;
	matFiltDesiredPSD		=	m_pInstFiltDesiredPSD->spectrum();
	matUnfiltDesiredPSD		=   m_pInstUnfiltDesiredPSD->spectrum();
	m_vecPSDFreq			=	m_pInstFiltDesiredPSD->frequencies();

	m_matDesiredPSD.AssignConstant(2,matFiltDesiredPSD.nrows(),matFiltDesiredPSD.ncols(),0.);

	for (int j=0;j < matUnfiltDesiredPSD.nrows(); j++)
		for (int k=0; k < matUnfiltDesiredPSD.ncols(); k++)
			m_matDesiredPSD[0][j][k]=matUnfiltDesiredPSD[j][k];

	for (int j=0;j < matFiltDesiredPSD.nrows(); j++)
		for (int k=0; k < matFiltDesiredPSD.ncols(); k++)
			m_matDesiredPSD[1][j][k]=matFiltDesiredPSD[j][k];
}

void TargetSimulationBase::GenerateFirstDriveSignals( const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight )
{
	// Was TS Object Initialized? 
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	m_fIsDriveSignalsReady=false;

	// Index Variable
	int i,j,k;

	MatDoub   matDrivesignals;			// [ Input  X ((NumberofBS+1)*LengthBS) ]
	MatDoub	  matDriveSignals2BS;		// [ Input  X 1 X 2BS ]
	MatDoub   matTarget;

	m_nINVHQtap=nLBlockSize;
	m_nINVHMpoint=2*nLBlockSize-(m_nINVHQtap-1);
	int Overlapaddcount= int(nNTarSignalPoint)/int(m_nINVHMpoint)+1;

	matDrivesignals.Assign(nNExc,(m_nINVHMpoint*Overlapaddcount+m_nINVHQtap-1),0.);

	int processedpointsindex=0;

	for ( i=0 ; i < Overlapaddcount ; i++ )
	{
		// 임시 저장 변수 초기화
		matTarget.Assign(nNPriResp,nLBlockSize*2,0.);

		// 목표 신호 일부분 임시 저장 객체에 저장
		if ( i !=(Overlapaddcount-1) ){
			for ( j=0 ; j < nNPriResp ; j++)
				for ( k=0 ; k < m_nINVHMpoint ; k++)
					matTarget[j][k]=vecErrorWeight[j]*m_matDesiredSignals[1][j][k+processedpointsindex];
		}
		else
		{
			for ( j=0 ; j < nNPriResp ; j++)
				for ( k=0 ; k < nNTarSignalPoint-processedpointsindex ; k++)
					matTarget[j][k]=vecErrorWeight[j]*m_matDesiredSignals[1][j][k+processedpointsindex];			
		}


		// FFT Desired Signals
		for ( j=0 ; j < nNPriResp ; j++)
			rlftin3D(matTarget[j],nLBlockSize*2,1);

		// FFT Convolution (m_matConvInvH*matDriveSignals2BS)
		Matrix3DComplexProduct(m_matConvInvH,matTarget,matDriveSignals2BS);

		// IFFT Result From Above
		for ( j=0 ; j < nNExc ; j++)
			rlftin3D(matDriveSignals2BS[j],nLBlockSize*2,-1);


		// M만큼 m_DriveSignals에 대입
		for ( j=0 ; j < nNExc ; j++ ){
			for ( k=0 ; k < nLBlockSize*2 ; k++){
				matDrivesignals[j][k+processedpointsindex]
				+= matDriveSignals2BS[j][k]*2./double(nLBlockSize*2);
			} 
		}

		processedpointsindex += m_nINVHMpoint;
	}

// 	// 최종 matTargetPre의 kdelay 만큼 딜레이
	for ( j=0 ; j < nNExc ; j++ )
		for ( k=0 ; k < nNTarSignalPoint ; k++)
			m_matDriveSignals[j][k]=vecExcWeight[j]*matDrivesignals[j][k+m_nkdelay];
	// 최종 matTargetPre의 kdelay 만큼 딜레이


	// Flag.
	m_fIsFirstDrive=false;
	m_fIsDriveSignalsReady=true;
}


void TargetSimulationBase::GenerateUpdatedDriveSignals( const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight )
{
	// Was TS Object Initialized? 
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");
	if ( m_fIsFirstDrive == true ) return;

	// Index Variable
	int i,j,k;

	MatDoub   matDrivesignals;			// [ Input  X ((NumberofBS+1)*LengthBS) ]
	MatDoub	  matDriveSignals2BS;		// [ Input  X 1 X 2BS ]
	MatDoub   matTarget;

	m_nINVHQtap=nLBlockSize;
	m_nINVHMpoint=2*nLBlockSize-(m_nINVHQtap-1);
	int Overlapaddcount= int(nNTarSignalPoint)/int(m_nINVHMpoint)+1;

	matDrivesignals.Assign(nNExc,(m_nINVHMpoint*Overlapaddcount+m_nINVHQtap-1),0.);

	int processedpointsindex=0;

	for ( i=0 ; i < Overlapaddcount ; i++ )
	{
		// 임시 저장 변수 초기화
		matTarget.Assign(nNPriResp,nLBlockSize*2,0.);

		// 목표 신호 일부분 임시 저장 객체에 저장
		if ( i !=(Overlapaddcount-1) ){
			for ( j=0 ; j < nNPriResp ; j++)
				for ( k=0 ; k < m_nINVHMpoint ; k++)
					matTarget[j][k]=vecErrorWeight[j]*m_matErrorSignals[1][j][k+processedpointsindex];
		}
		else
		{
			for ( j=0 ; j < nNPriResp ; j++)
				for ( k=0 ; k < nNTarSignalPoint-processedpointsindex ; k++)
					matTarget[j][k]=vecErrorWeight[j]*m_matErrorSignals[1][j][k+processedpointsindex];			
		}


		// FFT Desired Signals
		for ( j=0 ; j < nNPriResp ; j++)
			rlftin3D(matTarget[j],nLBlockSize*2,1);

		// FFT Convolution (m_matConvInvH*matDriveSignals2BS)
		Matrix3DComplexProduct(m_matConvInvH,matTarget,matDriveSignals2BS);

		// IFFT Result From Above
		for ( j=0 ; j < nNExc ; j++)
			rlftin3D(matDriveSignals2BS[j],nLBlockSize*2,-1);


		// M만큼 m_DriveSignals에 대입
		for ( j=0 ; j < nNExc ; j++ ){
			for ( k=0 ; k < nLBlockSize*2 ; k++){
				matDrivesignals[j][k+processedpointsindex]
				+= matDriveSignals2BS[j][k]*2./double(nLBlockSize*2);
			} 
		}

		processedpointsindex += m_nINVHMpoint;
	}

// 	// 최종 matTargetPre의 kdelay 만큼 딜레이
	for ( j=0 ; j < nNExc ; j++ )
		for ( k=0 ; k < nNTarSignalPoint ; k++)
 			m_matDriveSignals[j][k]=vecExcWeight[j]*(m_matDriveSignals[j][k]+matDrivesignals[j][k+m_nkdelay]);

	m_fIsDriveSignalsReady=true;
}

void TargetSimulationBase::ImportResponseSignals( MatDoub & matRespSignals )
{
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Temparary Variables 
	int j,k;

	// m_DesiredSignals의 첫번째 요소에 원래 신호를 집어 넣는다.
	for (j=0 ; j < nNRespC ; j++){
		for ( k=0 ; k < nNTarSignalPoint ; k++){
			m_matResponseSignals[0][j][k]=matRespSignals[j][k];
		}
	}

	/*PadingRespSignals(nLBlockSize);*/
	// filtering matResponseSignals and PSD
	ProcessRawResponseSignals();
	// Error
	ProcessRawErrorSignals();
}

void TargetSimulationBase::ImportResponseSignals( MatDoub & matRespSignals , int nPaddingpoints )
{
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Temparary Variables 
	int j,k;

	// m_DesiredSignals의 첫번째 요소에 원래 신호를 집어 넣는다.
	for (j=0 ; j < nNRespC ; j++){
		for ( k=0 ; k < nNTarSignalPoint ; k++){
			m_matResponseSignals[0][j][k]=matRespSignals[j][k];
		}
	}

	PadingRespSignals(nPaddingpoints);
	// filtering matResponseSignals and PSD
	ProcessRawResponseSignals();
	// Error
	ProcessRawErrorSignals();

}

void TargetSimulationBase::ProcessRawResponseSignals()
{
	/*========================================================
	//
	//       matResponse Signal 데이타 처리
	//        
	//       1 . 신호 필터.
	//       2 . 필터하여 타임 데이타 생성 목적.
	//
	/*======================================================*/

	// Was TS Object Initialized? 
	if ( !(m_fIsTSInitialized) ) throw("TS was NOT initialized");

	// Spectral PSD Instance Initialization
	if( m_pInstUnfiltRespPSD != NULL ) delete m_pInstUnfiltRespPSD;
	m_pInstUnfiltRespPSD=new SpectOlap2D(/*채널 수*/ nNRespC,/*BS/2*/ nLBlockSize/2,nShiftPoints,nSamplingFreq,nNTarSignalPoint);
	if( m_pInstFiltRespPSD != NULL ) delete m_pInstFiltRespPSD;	
	m_pInstFiltRespPSD=new SpectOlap2D(/*채널 수*/ nNRespC,/*BS/2*/ nLBlockSize/2,nShiftPoints,nSamplingFreq,nNTarSignalPoint);

	// FFT -> Modify FFT -> IFFT -> insert m_DesiredSignlas[1][j][k];
    //                           -> PSD -> insert into m_matFiltDesiredPSD;

	// 변수 선언
	int i,j,k;
	Doub real;
	Doub imag;

	// 임시 저장 계산 객체
	MatDoub matFiltRespPSD,matUnfiltRespPSD,matResponseforPSD;
	MatDoub matTarget,matTargetPre,matTargetforPSD;


	matTargetforPSD.Assign(nNTarSignals,nLBlockSize,0.);

	m_nFIRMpoint=2*nLBlockSize-(m_nFIRQtap-1);
	int Overlapaddcount= int(nNTarSignalPoint)/int(m_nFIRMpoint)+1;

	matTargetPre.Assign(nNTarSignals,(m_nFIRMpoint*Overlapaddcount+m_nFIRQtap-1),0.);

	int processedpointsindex=0;

	for ( i=0 ; i < Overlapaddcount ; i++ )
	{
		// 임시 저장 변수 초기화
		matTarget.Assign(nNTarSignals,nLBlockSize*2,0.);

		// 목표 신호 일부분 임시 저장 객체에 저장
		if ( i !=(Overlapaddcount-1) ){
			for ( j=0 ; j < nNTarSignals ; j++)
				for ( k=0 ; k < m_nFIRMpoint ; k++)
					matTarget[j][k]=m_matResponseSignals[0][j][k+processedpointsindex];
		}
		else
		{
			for ( j=0 ; j < nNTarSignals ; j++)
				for ( k=0 ; k < nNTarSignalPoint-processedpointsindex ; k++)
					matTarget[j][k]=m_matResponseSignals[0][j][k+processedpointsindex];			
		}


		// FFT Desired Signals
		for ( j=0 ; j < nNTarSignals ; j++)
			rlftin3D(matTarget[j],nLBlockSize*2,1);

		// Convolution
		for ( j=0 ; j < nNTarSignals ; j++){
			for ( k=1 ; k < nLBlockSize ; k++){
				real	=	matTarget[j][k*2]* m_vecFilter[k*2]
				-matTarget[j][k*2+1]*m_vecFilter[k*2+1];

				imag	=	matTarget[j][k*2]* m_vecFilter[k*2+1]
				+matTarget[j][k*2+1]*m_vecFilter[k*2];

				matTarget[j][k*2]=real;
				matTarget[j][k*2+1]=imag;
			}
			matTarget[j][0]=matTarget[j][0]*m_vecFilter[0];
			matTarget[j][1]=matTarget[j][1]*m_vecFilter[1];
		}

		// IFFT Result From Above
		for ( j=0 ; j < nNTarSignals ; j++)
			rlftin3D(matTarget[j],nLBlockSize*2,-1);


		// M만큼 m_DriveSignals에 대입
		for ( j=0 ; j < nNTarSignals ; j++ ){
			for ( k=0 ; k < nLBlockSize*2 ; k++){
				matTargetPre[j][k+processedpointsindex]
				+= matTarget[j][k]*2./double(nLBlockSize*2);
			} 
		}
		processedpointsindex += m_nFIRMpoint;

	}

	// 최종 matTargetPre의 kdelay 만큼 딜레이
	for ( j=0 ; j < nNTarSignals ; j++ )
		for ( k=0 ; k < nNTarSignalPoint ; k++)
			m_matResponseSignals[1][j][k]=matTargetPre[j][k+m_nkdelay];


	//  PSD	
	matTargetforPSD.Assign(nNTarSignals,nLBlockSize,0.);

	for ( i=0 ; i < nNBlockSize ; i++ ){
		for ( j=0 ; j < nNTarSignals ; j++ ){		
			for ( k=0 ; k < nLBlockSize ; k++)
				matTargetforPSD[j][k]=m_matResponseSignals[1][j][k+i*nLBlockSize];
		}
		m_pInstFiltRespPSD->adddataseg(matTargetforPSD,hann);

		for ( j=0 ; j < nNTarSignals ; j++ ){
			for ( k=0 ; k < nLBlockSize ; k++)
				matTargetforPSD[j][k]=m_matResponseSignals[0][j][k+i*nLBlockSize];
		}
		m_pInstUnfiltRespPSD->adddataseg(matTargetforPSD,hann);
	}

	matFiltRespPSD			=	m_pInstFiltRespPSD->spectrum(); 
	matUnfiltRespPSD		=   m_pInstUnfiltRespPSD->spectrum();
	
	m_matRespPSD.AssignConstant(2,matFiltRespPSD.nrows(),matFiltRespPSD.ncols(),0.);

	for (int j=0;j < matUnfiltRespPSD.nrows(); j++)
		for (int k=0; k < matUnfiltRespPSD.ncols(); k++)
			m_matRespPSD[0][j][k]=matUnfiltRespPSD[j][k];

	for (int j=0;j < matFiltRespPSD.nrows(); j++)
		for (int k=0; k < matFiltRespPSD.ncols(); k++)
			m_matRespPSD[1][j][k]=matFiltRespPSD[j][k];

}

void TargetSimulationBase::ProcessRawErrorSignals()
{
	// 주 응답 채널 에러 계산
	for ( int j=0 ; j < nNPriResp ; j++ )
		for ( int k=0 ; k < nNSegment ; k++ )
		{
			m_matErrorSignals[0][j][k]=m_matDesiredSignals[0][j][k]-m_matResponseSignals[0][j][k];
			m_matErrorSignals[1][j][k]=m_matDesiredSignals[1][j][k]-m_matResponseSignals[1][j][k];
		}
	// 부 응답 채널 에러 계산
	int isub=0;
	for ( int j=0 ; j < nNSlavResp; j++)
	{
		if ( pWhetherSubTargetExist[j]==true )
		{
			for ( int k=0 ; k < nNSegment ; k++ ){
				m_matErrorSignals[0][nNPriResp+isub][k]
						=   m_matDesiredSignals[0][nNPriResp+isub][k]
						   -m_matResponseSignals[0][nNPriResp+j][k];
				m_matErrorSignals[1][nNPriResp+isub][k]
						=   m_matDesiredSignals[1][nNPriResp+isub][k]
						   -m_matResponseSignals[1][nNPriResp+j][k];
			}
			isub +=1;
		}
	}
}
MatDoub TargetSimulationBase::GetDriveSignals()
{
	return m_matDriveSignals;
}

Mat3DDoub TargetSimulationBase::GetResponseSignals()
{
	return m_matResponseSignals;
}

Mat3DDoub TargetSimulationBase::GetDesiredSignals()
{
	return m_matDesiredSignals;
}

Mat3DDoub TargetSimulationBase::GetPSD3DDesiredSignals()
{
	return m_matDesiredPSD;
}

Mat3DDoub TargetSimulationBase::GetPSD3DResponseSignals()
{
	return m_matRespPSD;
}

TargetSimulationBase::~TargetSimulationBase()
{
	if ( m_pInstUnfiltRespPSD !=NULL ) delete m_pInstUnfiltRespPSD;

	if ( m_pInstFiltDesiredPSD != NULL ) delete m_pInstFiltDesiredPSD;

	if ( m_pInstFiltRespPSD !=NULL ) delete m_pInstFiltRespPSD;

	if ( m_pInstUnfiltDesiredPSD != NULL ) delete m_pInstUnfiltDesiredPSD;

	if ( pWhetherSubTargetExist!=NULL ) delete pWhetherSubTargetExist;
}

