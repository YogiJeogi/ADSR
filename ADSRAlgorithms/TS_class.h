#pragma once
#ifndef __TS_CLASS_H__
#define __TS_CLASS_H__

// Define Header
#include "BaseHD.h"
#include "SI_class.h"


#define PI 3.141592653589793238

// Define Class
class TargetSimulationBase : protected ADSRbase
{
// protected:
// 	int			nNExc;					// 가진채널 수를 뜻한다.
// 	int			nNPriResp;				// 주응답채널 수를 뜻한다.
// 	int			nNSlavResp;				// 부응답채널 수를 뜻한다.
// 	int			nNTarSignals;			// 목표신호가 있는 채널 수를 뜻한다. 주응답 + (부응답 중 목표신호가 있는것)
// 	int			nNTarSignalPoint;		// 목표신호의 포인수 수이다. 다만 BS의 배수는 아니다.
// 	int			nNRespC;				// 전체 응답 채널을 뜻한다.
// 	int			nLBlockSize;			// 블록사이즈 길이를 뜻한다.
// 	int			nNBlockSize;			// 블록사이즈 길이의 정수배
// 	int			nNSegment;				// BS수와 BS길이의 곱이다. 즉 BS의 정수배의 길이다. Target신호가 제로패딩이 된 신호.	
// 	int			nLowBandPoint;			// 주파수를 뜻하는 포인트다.
// 	int			nUpperBandPoint;		// 주파수를 나타내는 포인드.
// 	int			nPointShift;
// 	int			nSamplingFreq;
// 
protected:
	bool*		pWhetherSubTargetExist; // 부응답채널에 목표신호가 매칭되었는지의 여부 BOOL
		 
private:
	// 필터 설계
	int m_nkdelay;
	int m_nFIRQtap;
	int m_nFIRMpoint;
	int m_nINVHQtap;
	int m_nINVHMpoint;

	VecDoub		 m_vecFilter;
	VecDoub      m_vecFilterFre;

	SpectOlap2D* m_pInstUnfiltRespPSD;
	SpectOlap2D* m_pInstFiltRespPSD;

	SpectOlap2D* m_pInstFiltDesiredPSD;
	SpectOlap2D* m_pInstUnfiltDesiredPSD;

protected: 

	MatDoub     m_matDriveSignals;			// [ Input  X (NumberofBS*LengthBS) ]
	Mat3DDoub	m_matDesiredSignals;		// [ DesiredOutput X (NumberofBS*LengthBS) ]
	Mat3DDoub   m_matResponseSignals;		// [ Pr+Slave Output X (NumberofBS*LengthBS) ]
	Mat3DDoub	m_matErrorSignals;			// [ DesiredOutput X (NumberofBS*LengthBS) ]

	// Desired Signal
	Mat3DDoub	m_matDesiredPSD;	// 0 Index -> Unfilt , 1 Index -> Filt
	
	// Response Signal
	Mat3DDoub	m_matRespPSD;

	VecDoub		m_vecPSDFreq;

	Mat3DDoub	m_matConvInvH;     // [ Input X Output X BS*2 ]
	

public  :

	// FLAG
	// GUI에서 RESP를 계측하던 중 멈출경우 m_flag_is_takeresposesignals_initialized Set up 해줘야한다.
	// 첫번째 드라이버 생성과 구동 후 업데이트 할경우 m_flag_is_firstTimeTS Set Off
	// GUI에서 m_flag_is_responsedata_full확인해야한다.
	// GUI에서 m_flag_PSDReady를 확인해야한다.
	// GUI에서 m_flag_overMaximumVoltage 확인해야한다.
	bool         m_fIsTSInitialized;
	bool		 m_fIsFirstDrive;			// 드라이버 생성이 처음인가.
	bool         m_fIsDriveSignalsReady;	// 드라이버 신호가 생성되었는가.

public:
	//METHOD
	TargetSimulationBase ();

	virtual void TSInitialization( const SYSTEMSETTING & SS); // 필수적 다음 두단계	ImportInverseModel(matInvH);ImportDesiredSignals(matDesiredSignals);
	virtual void TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals );

	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals );  //완 +PSD
	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals, int nPaddingpoints);
	void ImportSystemsetting(SYSTEMSETTING & SS);		  //완
	void ImportInverseModel(const Mat3DDoub & matInvH);  //완	
	void ImportResponseSignals(MatDoub & matRespSignals );
	void ImportResponseSignals(MatDoub & matRespSignals , int nPaddingpoints);


private:
	void FilterDesign();
	void PadingDesiredSignals(int point);
	void PadingRespSignals(int point);
	//======================//
	// 내부 데이타 처리 함수//
	//======================//
	void ProcessRawDesiredSignals();  //완
	void ProcessRawResponseSignals(); //완
	void ProcessRawErrorSignals();	  //완

public:
	// 반 완 / vecExcWeight 준비 안됨.
	virtual void GenerateFirstDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);
	virtual void GenerateUpdatedDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);

	// 사용자 요청에 의한 Export
	//  Get Data
	MatDoub GetDriveSignals();		     
	Mat3DDoub GetPSD3DDesiredSignals();      
	Mat3DDoub GetPSD3DResponseSignals();     
	Mat3DDoub GetResponseSignals();
	Mat3DDoub GetDesiredSignals();

	void Debugging();
	~TargetSimulationBase();

};

// 실시간 응답 처리를 위한 자식 클래스
class TARGETSIMULATION : public TargetSimulationBase
{

private:

	// 리얼타임 데이타 저장용 확인 데이타  
	int				m_nTempLgthSavedRespData;
	int				m_nNBlockPerformedInPSD;
	// 리얼타임 데이타 저장 데이타 
	MatDoub			m_matRealTimeRespSignals;
	MatDoub			m_matRealTimePSD;
	MatDoub			m_matDriveSignalPre;

	SpectOlap2D*	m_pInstRealTimePSD; 

	// 이터레이션 현재까지 결과를 과거를 포함 모두 저장 배열.
	// 1Index 신호종류, 2Index Iteration수, 3Index Data
	
	Mat3DDoub		m_matIterData;
	
	bool			m_fIsIterationComplete;
public:
	// 현재 이터레이션 수행 작업을 나타내는 상태값
	int				m_nIterationNumber;

public:
	//외부에서 확인하기 위한 상태값
	// 데이타를 다 받았는지여부
	bool			m_fIsRespDataFull;
	// PSD가 준비가 되었는지 여부 
	bool			m_fIsRespPSDReady;	

private:
	// 이터레이션 결과 저장.
	void IterationResultSave(); 
	//업데이트드라이브함수내에서 업데이트 전에 사용됨.
public:
	// 과거 이터레이션 결과 값을 불러옴. 드라이브, 응답, 에러 PSD
	void IterationResultLoad(int IterationNumber); // 사용자 요청에 의해 롤백될 수 있어서 퍼블릭!

public:
	virtual void TSInitialization( const SYSTEMSETTING & SS);
	virtual void TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals );

	// 드라이브 생성뿐만 아니라 그 때의 응답 데이타를 매트릭스에 저장된다.
	virtual void GenerateFirstDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);
	virtual void GenerateUpdatedDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);

	//=============================//
	// 실시간 응답 데이타 처리 함수//
	//=============================//
	virtual void TakeResponseSignals( MatDoub & matResponse ,bool IsFirst/*false*/ );
	void RealResponseProcessing();//합치려하였으나 NI 이벤트 처리 속도를 고려하여 분리.

public:
	// 실시간 사용자 요청에 의한 Export 
	// Get Data
	MatDoub GetRealTimePSDRespSignals();     //미완
	VecDoub GetFrequency();              //미구현
	int     GetIterationNumber();

public:
	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals ) abstract;
	
	// 추후
	void ResamplingTargetSignals();//미완

	~TARGETSIMULATION();
};


// 통계 처리가 들어간 자식 클래스
class TARGETSIMULATIONEX : public TARGETSIMULATION
{
private: 
	int				m_nElement; // 통계 요소 갯수;
private:
	MatDoub			m_matStatisticDesiredSignal;
	Mat3DDoub		m_matStatisticData;
					// 이 데이타는 이터레이션에 따른 변화를 계속 저장함 . 목표신호 제외;
private:
	VecDoub CalculateStatistic(VecDoub & Data);
public:
	Mat3DDoub & GetStatisticData();
	MatDoub   & GetDesiredStatisticData();
public:
	TARGETSIMULATIONEX();
	virtual void TSInitialization( const SYSTEMSETTING & SS);
	virtual void TSInitialization( const SYSTEMSETTING & SS, Mat3DDoub & matInvH, MatDoub & matDesiredSignals );

	virtual void GenerateFirstDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);
	virtual void GenerateUpdatedDriveSignals(const VecDoub & vecExcWeight ,const VecDoub & vecErrorWeight);

	virtual void TakeResponseSignals( MatDoub & matResponse ,bool IsFirst=false );
	virtual void ImportDesiredSignals(MatDoub & matDesiredSignals );

};
#endif