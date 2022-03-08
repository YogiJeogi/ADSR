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
	int			nStartFrqIndex;  	// ���ļ��� ���ϴ� ����Ʈ��. 0Based , 0�̶�� 0Hz ���� [0 1]���ļ� ����
	int			nStopFrqIndex;		// [0 1]���ļ� ����
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

	int			m_nLSavedRespData; // �󸶸�ŭ Response�� �����ߴ����� ����
	int         m_nLShiftedData;   // shift Point�� ��� ��

	int         m_tnsum_avg;
	int         m_tnsum_fft;
	int			m_tnsum_psd;             // PSD ���� ��



	//==========================//
	// �߰� ���� ����Ÿ �����//
	//==========================//
	MatDoub				Xavg,Yavg;


//	MatDoub				m_Xfft, m_Yfft;			// ��ȣ�� Cyclic Avg ���
//	Mat3DDoub			m_Gxx,m_Gyy,m_Gxy,m_Gyx;// Xavg�� Yavg PSD���
	Mat3DDoub			m_H;					// [Output X Input X BS ]
	Mat3DDoub			m_HAmplitude;
	Mat3DDoub			m_HAngle;
	VecDoub				m_HFrequency;

	Mat3DDoub			m_invH;					// [Input  X Ouput X BS ]
	Mat3DDoub			m_oCOH;
	Mat3DDoub			m_mCOH;					// Not yet

public:
	// Input Output Raw Data;
	MatDoub		m_Xfft, m_Yfft;			// ��ȣ�� Cyclic Avg ���
	Mat3DDoub	m_Gxx,m_Gyy,m_Gxy,m_Gyx;// Xavg�� Yavg PSD���
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
	// ����Ÿ ó�� �Լ�//
	//=================//
private:
	/*void CalculateFFT();*/
	void calculatePSD();
public:
	void CalculateH(); // FFT�� PSD�� �����Ѵ�.(���� �̿�)
	void CalculateINVH(); 
	void CalculateOrdinaryCOH(); // ���� �̿�

	//==================================//
	// Importing and Exporting Functions//
	//==================================//
	// ����Ÿ�� ���ڸ��� avg�� PSD�� �Ѵ�.
	void InsertResponseSignals( MatDoub & Response , bool IsFirst );

	// ����� ��û�� ���� Export
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