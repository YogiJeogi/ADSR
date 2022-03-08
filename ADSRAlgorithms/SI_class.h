#pragma once
#ifndef __SI_CLASS_H__
#define __SI_CLASS_H__

// Define Header
#include "BaseHD.h"

#define PI 3.141592653589793238

#define __HEAP__
// TO DO

// Common Class Definition
class ADSRbase 
{
public:
	int			nNExc;	
	int			nNPriResp;	
	int			nNSlavResp;
	int			nNRespC;
	int			nNTarSignals;
	int			nNTarSignalPoint;
	int			nLBlockSize; 
	int			nNBlockSize;
	int			nNSegment;
	int			nStartFrqIndex;  	// 주파수를 뜻하는 포인트다. 0Based , 0이라면 0Hz 뜻함 [0 1]주파수 영역
	int			nStopFrqIndex;		// [0 1]주파수 영역
	int			nSignalsPoints;
	int			nShiftPoints; 
	int			nWindow;
	int         nWhiteNoiseAdded;    
	int			nSamplingFreq;
	int			nCycAvg;
	int         nWhiteNoiseBorderFreq;
	int			nWHiteNoiseBorderFreqIndex;
	double		dbWhiteNoiseAlpha;
	double      dbWhiteNoiseSTD;

};
// Define Class

///////////////////////////////////////////
///////////////////////////////////////////
class SYSTEMIDENTIFICATION : private ADSRbase 
{
private : 

	bool        m_bIsFFTReady;
	bool		m_bIsPSDReady;

	int			m_nLSavedRespData; // 얼마만큼 Response를 저장했는지의 정보
	int         m_nLShiftedData;   // shift Point의 배수 값

	int         m_tnsum_avg;
	int         m_tnsum_fft;
	int			m_tnsum_psd;             // PSD 합의 수



	//==========================//
	// 중간 계산된 데이타 저장용//
	//==========================//
	MatDoub				Xavg,Yavg;


//	MatDoub				m_Xfft, m_Yfft;			// 신호를 Cyclic Avg 결과
//	Mat3DDoub			m_Gxx,m_Gyy,m_Gxy,m_Gyx;// Xavg와 Yavg PSD결과
	Mat3DDoub			m_H;					// [Output X Input X BS ]
	Mat3DDoub			m_HAmplitude;
	Mat3DDoub			m_HAngle;
	VecDoub				m_HFrequency;

	Mat3DDoub			m_invH;					// [Input  X Ouput X BS ]
	Mat3DDoub			m_oCOH;
	Mat3DDoub			m_mCOH;					// Not yet

public:
	// Input Output Raw Data;
	MatDoub		m_Xfft, m_Yfft;			// 신호를 Cyclic Avg 결과
	Mat3DDoub	m_Gxx,m_Gyy,m_Gxy,m_Gyx;// Xavg와 Yavg PSD결과
	MatDoub		m_WHN_driver;           // [Input X Data]
	MatDoub		m_Response;             // [Output X Data]
public  :

	bool m_bIsRespDataFull;
	bool m_bIsWhiteNoiseReady;
	bool m_bIsHReady;
	bool m_bIsINVHReady;

	SYSTEMIDENTIFICATION();
	SYSTEMIDENTIFICATION(SYSTEMSETTING  &SS);

	void SIInitialization( const SYSTEMSETTING & SS );

	void GenerateWhiteNoise();

	//=================//
	// 데이타 처리 함수//
	//=================//
private:
	/*void CalculateFFT();*/
	void calculatePSD();
public:
	void CalculateH(); // FFT와 PSD를 수행한다.(아직 미완)
	void CalculateINVH(); 
	void CalculateOrdinaryCOH(); // 아직 미완

	//==================================//
	// Importing and Exporting Functions//
	//==================================//
	// 데이타를 얻자마자 avg와 PSD을 한다.
	void InsertResponseSignals( MatDoub & Response , bool IsFirst );

	// 사용자 요청에 의한 Export
	MatDoub		GetWhiteNoise();
	MatDoub		GetResponse();

	virtual Mat3DDoub	GetHModel() const ;

	Mat3DDoub	GetInvHModel () const;
	Mat3DDoub	GetOrdinaryCoherence() const;
	Mat3DDoub	GetMultiCoherence() const;
	void		GetHAmplitudeModel(Mat3DDoub & Amplitude,Mat3DDoub & Angle);

	void debugging();

};

class SYSTEMIDENTIFICATIONEX : public SYSTEMIDENTIFICATION
{
	
};
#endif