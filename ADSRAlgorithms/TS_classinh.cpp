#include "TS_class.h"


void TARGETSIMULATION::TSInitialization( const SYSTEMSETTING & SS )
{
	TargetSimulationBase::TSInitialization(SS);
	
	// TO DO
	m_nTempLgthSavedRespData				=	0;

	m_fIsRespDataFull						=	false;
	m_fIsRespPSDReady						=	false;
	m_fIsIterationComplete					=   false;

	m_matIterData.AssignConstant(nNExc+nNRespC,50,nNSegment,0.);
	m_matDriveSignalPre.Assign	(nNExc  ,nNBlockSize*nLBlockSize ,0.);
}

void TARGETSIMULATION::TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals )
{
	TargetSimulationBase::TSInitialization(SS, matInvH, matDesiredSignals);

	// TO DO
	m_nTempLgthSavedRespData				=	0;

	m_fIsRespDataFull						=	false;
	m_fIsRespPSDReady						=	false;
	m_fIsIterationComplete					=   false;

	m_matIterData.AssignConstant(nNExc+nNRespC,50,nNSegment,0.);
	m_matDriveSignalPre.Assign	(nNExc  ,nNBlockSize*nLBlockSize ,0.);
}

void TARGETSIMULATION::TakeResponseSignals( MatDoub & matResponse ,bool IsFirst/*false*/ )
{

	if ( IsFirst == true )
	{
		m_nTempLgthSavedRespData	=	0;
		m_nNBlockPerformedInPSD		=	0;
		m_fIsRespPSDReady			=	false;
		m_matRealTimeRespSignals.Assign(nNRespC,nNBlockSize*nLBlockSize,	0.);
		m_pInstRealTimePSD=new SpectOlap2D(/*Channel*/ nNRespC,/*BS/2*/ nLBlockSize/2,nShiftPoints,nSamplingFreq,nNTarSignalPoint);
	}

	if (m_nTempLgthSavedRespData == nNTarSignalPoint) 
	{
		throw ( " matResponse Data Full");
	}
	/*========================================================
	//
	//        RESPONSE DATA 저장
	//        
	//        matResponse 받은 것을 계속 쌓아둠
	//
	/*======================================================*/
	// Respose 데이타를 m_ResponseSignals에 Push_back
	// 예외 처리문 필요
	if ( matResponse.ncols() > 0 && nNRespC==matResponse.nrows()){
		// 입력데이타와 저장된 데이타의 합이 타켓포인트보다 큰가를 판단
		// -> 작다면
		if ( (m_nTempLgthSavedRespData+matResponse.ncols()) < nNTarSignalPoint ){
			for (int j=0 ; j < nNRespC ; j++)
				for (int k=0 ; k < matResponse.ncols() ; k++ ){
					m_matRealTimeRespSignals[j][k+m_nTempLgthSavedRespData]=matResponse[j][k];
				}
			// Save Index Increment
			m_nTempLgthSavedRespData += matResponse.ncols();
		}
		// -> 같거나 크다면
		else{
			for (int j=0 ; j< nNRespC ; j++)
				for (int k=0 ; k < (nNTarSignalPoint-m_nTempLgthSavedRespData) ; k++ ){
					m_matRealTimeRespSignals[j][k+m_nTempLgthSavedRespData]=matResponse[j][k];
				}
			// Save Index Increment
			m_nTempLgthSavedRespData = nNTarSignalPoint;
			// Set Up Flag
			m_fIsRespDataFull=true;
		}
	}
	else { throw(" matResponse Data NOT match or No DATA"); }

	//================ Complete Data Acqusition =================//
	if ( m_fIsRespDataFull == true )
	{
		TargetSimulationBase::ImportResponseSignals(m_matRealTimeRespSignals);
		IterationResultSave(); // 기존 데이타를 배열에 저장.
	}
}

void TARGETSIMULATION::RealResponseProcessing()
{
	// Temparary Matrix
	MatDoub matRespSignal;

	matRespSignal.Assign(nNRespC,nLBlockSize,0.);
	
	m_fIsRespPSDReady=false;
	//================ 동시에 PSD =================//
	int cycle=m_nTempLgthSavedRespData/nLBlockSize-m_nNBlockPerformedInPSD;
	for ( int i=0; i < cycle ; i++ )
	{
		for ( int j=0 ; j < nNRespC ;j++)
		{
			for ( int k=0 ; k < nLBlockSize ; k++){
				matRespSignal[j][k]	
				=m_matRealTimeRespSignals[j][k+i*nLBlockSize+m_nNBlockPerformedInPSD*nLBlockSize];
			}
		}
		m_pInstRealTimePSD->adddataseg(matRespSignal,hann);
		m_fIsRespPSDReady=true;
	}
	// 마지막 BS보다 작은 데이타 한번 처리하기 위한 것이다.
	if ( m_fIsRespDataFull==true)
	{
		for ( int j=0 ; j < nNRespC ;j++)
		{
			for ( int k=0; k <(nNTarSignalPoint-(nNBlockSize-1)*nLBlockSize);k++ )
			{
				matRespSignal[j][k]	
				=m_matResponseSignals[0][j][k+(nNBlockSize-1)*nLBlockSize];
			}
		}
		m_pInstRealTimePSD->adddataseg(matRespSignal,hann);

		m_matRealTimePSD =	m_pInstRealTimePSD->spectrum();
		m_nNBlockPerformedInPSD += 1;
		m_fIsRespPSDReady=true;
		m_fIsIterationComplete=true;

		return;
	}

	m_nNBlockPerformedInPSD +=  cycle;
	if(m_fIsRespPSDReady==true)
	{
		m_matRealTimePSD		 =	m_pInstRealTimePSD->spectrum();
	}
}

void TARGETSIMULATION::GenerateFirstDriveSignals( const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight )
{
	TargetSimulationBase::GenerateFirstDriveSignals(vecExcWeight,vecErrorWeight);
	m_nIterationNumber=0;
	m_fIsRespDataFull=false;
	m_fIsRespPSDReady=false;
	m_fIsIterationComplete=false;
}

void TARGETSIMULATION::GenerateUpdatedDriveSignals( const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight )
{
	// 업데이트 드라이브 생성 전
	if (m_fIsIterationComplete==true)
	{
		m_matDriveSignalPre=m_matDriveSignals;

		TargetSimulationBase::GenerateUpdatedDriveSignals(vecExcWeight,vecErrorWeight);
		// 업데이트 드라이브 생성 후
		m_nIterationNumber+=1;
	}
	else
	{
		m_matDriveSignals=m_matDriveSignalPre;

		TargetSimulationBase::GenerateUpdatedDriveSignals(vecExcWeight,vecErrorWeight);
		// 업데이트 드라이브 생성 후
	}

	m_fIsRespDataFull=false;
	m_fIsRespPSDReady=false;
	m_fIsIterationComplete=false;
}

void TARGETSIMULATION::IterationResultSave()
{
	int i,j,k;

	if( m_nIterationNumber > 50 && (m_nIterationNumber%50)==1 )
	{
		Mat3DDoub tmp=m_matIterData;
		m_matIterData.AssignConstant
			(nNExc+nNRespC,50*int( m_nIterationNumber/50+1),nNSegment,0.);
	
		for ( i=0 ; i < nNExc+nNRespC ; i++ )
			for ( j=0 ; j < m_nIterationNumber-1 ; j++ )
				for ( k=0 ; k < nNSegment ; k++)
					m_matIterData[i][j][k]=tmp[i][j][k];

	}
	// 드라이브 시그널 저장
	for ( i=0 ; i < nNExc ; i++ )
		for ( k=0 ; k < nNSegment ; k++)
			m_matIterData[i][m_nIterationNumber][k]=m_matDriveSignals[i][k];

	// Unfiltered 응답 시그널 저장
	for ( i=nNExc ; i < nNExc+nNRespC ; i++ )
		for ( k=0 ; k < nNSegment ; k++ )
			m_matIterData[i][m_nIterationNumber][k]=m_matResponseSignals[0][i-nNExc][k];
}

void TARGETSIMULATION::IterationResultLoad(int IterationNumber)
{
	if( IterationNumber >= m_nIterationNumber) throw("잘못넣은거임!!미래값임");
	
	int i,k;
	MatDoub tmpResp;
	tmpResp.Assign(nNRespC,nNSegment,0.);
	
	// 드라이브 로딩
	for ( i=0 ; i < nNExc ; i++ )
		for ( k=0 ; k < nNSegment ; k++)
			m_matDriveSignals[i][k]=m_matIterData[i][IterationNumber][k];

	// Unfiltered 응답 시그널 로딩
	for ( i=nNExc ; i < nNExc+nNRespC ; i++ )
		for ( k=0 ; k < nNSegment ; k++ )
			tmpResp[i-nNExc][k]=m_matIterData[i][IterationNumber][k];

	// PSD와 에러 시그널 구함~
	ImportResponseSignals(tmpResp);

	// 이터레이션 넘버 수정
	m_nIterationNumber=IterationNumber;
}

MatDoub TARGETSIMULATION::GetRealTimePSDRespSignals()
{
	return m_matRealTimePSD;
}

int TARGETSIMULATION::GetIterationNumber()
{
	return m_nIterationNumber;
}

TARGETSIMULATION::~TARGETSIMULATION()
{
	if (m_pInstRealTimePSD !=NULL ) delete m_pInstRealTimePSD;
}

TARGETSIMULATIONEX::TARGETSIMULATIONEX() 
{
	m_nElement=5;
}


void TARGETSIMULATIONEX::TSInitialization( const SYSTEMSETTING & SS )
{
	TARGETSIMULATION::TSInitialization(SS);

	// TO DO
	// 통계처리 저장 데이타 초기화
	m_matStatisticDesiredSignal.Assign(nNTarSignals*2,m_nElement,0.);
	m_matStatisticData.AssignConstant(nNExc+nNRespC*2+nNTarSignals*2,50,m_nElement,0.);
	
}

void TARGETSIMULATIONEX::TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals )
{
	TARGETSIMULATION::TSInitialization(SS,matInvH,matDesiredSignals);

	// TO DO
	// 통계처리 저장 데이타 초기화
	m_matStatisticDesiredSignal.Assign(nNTarSignals*2,m_nElement,0.);
	m_matStatisticData.AssignConstant(nNExc+nNRespC*2+nNTarSignals*2,50,m_nElement,0.);
	
}

void TARGETSIMULATIONEX::GenerateFirstDriveSignals( const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight )
{
	TARGETSIMULATION::GenerateFirstDriveSignals(vecExcWeight,vecErrorWeight);
	
	// TO DO
	// 드라이브 신호 통계처리
	if ( m_fIsDriveSignalsReady==true )
	{
		VecDoub tmpSt;
		VecDoub Data;
		Data.assign(nNTarSignalPoint,0.);
		
		for ( int i=0; i < nNExc ; i++ )
		{
			for ( int k=0; k < nNTarSignalPoint ; k++ )
				Data[k]=m_matDriveSignals[i][k];

			tmpSt=CalculateStatistic(Data);

			for ( int k=0; k < m_nElement ; k++ )
			m_matStatisticData[i][0][k]=tmpSt[k];
		}
	}
}

void TARGETSIMULATIONEX::GenerateUpdatedDriveSignals( const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight )
{
	TARGETSIMULATION::GenerateUpdatedDriveSignals(vecExcWeight,vecErrorWeight);
	
	// TO DO
	// 드라이브 신호 통계처리
	int i,j,k;

	// 이터레이션이 50배수가 초과하면 매트릭스 확장
	if( m_nIterationNumber > 50 && (m_nIterationNumber%50)==1 )
	{
		Mat3DDoub tmp=m_matStatisticData;
		m_matStatisticData.AssignConstant
			(nNExc+nNRespC*2+nNTarSignals*2,50*int( m_nIterationNumber/50+1),nNSegment,0.);

		for ( i=0 ; i < nNExc+nNRespC*2+nNTarSignals*2 ; i++ )
			for ( j=0 ; j < m_nIterationNumber-1 ; j++ )
				for ( k=0 ; k < nNSegment ; k++)
					m_matStatisticData[i][j][k]=tmp[i][j][k];
	}

	if ( m_fIsDriveSignalsReady==true )
	{
		VecDoub tmpSt;
		VecDoub Data;
		Data.assign(nNTarSignalPoint,0.);

		for ( int i=0; i < nNExc ; i++ )
		{
			for ( int k=0; k < nNTarSignalPoint ; k++ )
				Data[k]=m_matDriveSignals[i][k];

			tmpSt=CalculateStatistic(Data);

			for ( int k=0; k < m_nElement ; k++ )
				m_matStatisticData[i][m_nIterationNumber][k]=tmpSt[k];
		}

	}
}

void TARGETSIMULATIONEX::TakeResponseSignals( MatDoub & matResponse ,bool IsFirst/*false*/ )
{
	TARGETSIMULATION::TakeResponseSignals(matResponse,IsFirst);

	// TO DO
	// 응답 신호 통계 처리.
	if( m_fIsRespDataFull ==true )
	{
		// 응답 신호 통계처리
		VecDoub tmpSt;
		VecDoub Data;
		Data.assign(nNTarSignalPoint,0.);

		for ( int i=0; i < nNRespC ; i++ )
		{
			for ( int k=0; k < nNTarSignalPoint ; k++ )
				Data[k]=m_matResponseSignals[0][i][k];

			tmpSt=CalculateStatistic(Data);

			for ( int k=0; k < m_nElement ; k++ )
				m_matStatisticData[i+nNExc][m_nIterationNumber][k]=tmpSt[k];
		}

		for ( int i=0; i < nNRespC ; i++ )
		{
			for ( int k=0; k < nNTarSignalPoint ; k++ )
				Data[k]=m_matResponseSignals[1][i][k];

			tmpSt=CalculateStatistic(Data);

			for ( int k=0; k < m_nElement ; k++ )
				m_matStatisticData[i+nNExc+nNRespC][m_nIterationNumber][k]=tmpSt[k];
		}

		for ( int i=0; i < nNTarSignals ; i++ )
		{
			for ( int k=0; k < nNTarSignalPoint ; k++ )
				Data[k]=m_matErrorSignals[0][i][k];

			tmpSt=CalculateStatistic(Data);

			for ( int k=0; k < m_nElement ; k++ )
				m_matStatisticData[i+nNExc+nNRespC*2][m_nIterationNumber][k]=tmpSt[k];
		}

		for ( int i=0; i < nNTarSignals ; i++ )
		{
			for ( int k=0; k < nNTarSignalPoint ; k++ )
				Data[k]=m_matErrorSignals[1][i][k];

			tmpSt=CalculateStatistic(Data);

			for ( int k=0; k < m_nElement ; k++ )
				m_matStatisticData[i+nNExc+nNRespC*2+nNTarSignals][m_nIterationNumber][k]=tmpSt[k];
		}
	}
}

void TARGETSIMULATIONEX::ImportDesiredSignals( MatDoub & matDesiredSignals )
{
	TargetSimulationBase::ImportDesiredSignals(matDesiredSignals);

	// 목표 신호 통계 처리.
	VecDoub tmpSt;
	VecDoub Data;
	Data.assign(nNTarSignalPoint,0.);

	for ( int i=0; i < nNTarSignals ; i++ )
	{
		for ( int k=0; k < nNTarSignalPoint ; k++ )
			Data[k]=m_matDesiredSignals[0][i][k];

		tmpSt=CalculateStatistic(Data);

		for ( int k=0; k < m_nElement ; k++ )
			m_matStatisticDesiredSignal[i][k]=tmpSt[k];
	}

	for ( int i=0; i < nNTarSignals ; i++ )
	{
		for ( int k=0; k < nNTarSignalPoint ; k++ )
			Data[k]=m_matDesiredSignals[1][i][k];

		tmpSt=CalculateStatistic(Data);

		for ( int k=0; k < m_nElement ; k++ )
			m_matStatisticDesiredSignal[i+nNTarSignals][k]=tmpSt[k];
	}

}


VecDoub TARGETSIMULATIONEX::CalculateStatistic( VecDoub & Data )
{
	VecDoub tmp(5);
	double Min=0;
	double Max=0;
	double Mean=0;
	double Variable=0;
	double Std=0;

	for ( int i=0 ; i< Data.size() ; i++ )
	{
	  Min = Data[i] < Min ? Data[i] : Min;
	  Max = Data[i] > Max ? Data[i] : Max;
	  Mean += Data[i];
	}
	Mean=Mean/Data.size();

	for ( int i=0 ; i< Data.size() ; i++ )
	  Variable += SQR(Data[i]-Mean);

	Variable = Variable/Data.size();
	Std=pow(Variable,0.5);

	tmp[0]=Min;
	tmp[1]=Max;
	tmp[2]=Mean;
	tmp[3]=Variable;
	tmp[4]=Std;

	return tmp;
}


Mat3DDoub & TARGETSIMULATIONEX::GetStatisticData()
{
	return	m_matStatisticData;
}

MatDoub & TARGETSIMULATIONEX::GetDesiredStatisticData()
{
	return	m_matStatisticDesiredSignal;
}