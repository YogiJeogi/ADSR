#pragma once
#ifndef __PREFERST_H__
#define __PREFERST_H__

#include "typedef.h"
#include <string>

// 파일 입출력을 위한 기본형식 지정
struct _FileSystem{

	unsigned int	NumberofSignals;
	int				COLUMNWIDTH[100];
	int				COLUMNOFFSET[100];
	string			CHANNELNAME[100];
	unsigned int	LENGTH[100];
	string			UNIT[100];
	double			MINIMUM[100];
	double			MAXIMUM[100];
	double			START[100];
	double			DELTA[100];
};


// SYSTEM 세팅 지정
enum { HANN , BARTLETT, WELCH, SQARE };

struct _FrontEndSetting {

	int      m_nInput_Channels;
	int      m_nResponse_Channels;
	int      m_nSlave_Channels;
	int      m_nSamplingFreq;

	string	 m_arrstrInputChannelName[12];
	int		 m_arrindexInputChannelHW[12];
	string   m_arrstrResponseChannelName[16];
	int      m_arrindexResponseChannelHW[16];
	double   m_adbResponseChannelScale[16];
	string   m_astrResponseChannelScale[16];

	double   m_Scale_Factor;

};

struct _SpectralAnalysisSetting {

	int  	 m_nBlocksize;
	int		 m_nCyclicAveg;
	int      m_nWindow;
	int      m_nPercentofOverlap;
	int      m_nPshifted;

	int      m_nStartFreq;
	int      m_nStopFreq;       
	int      m_nStartFreqIndex; // 0-based index
	int      m_nStopFreqIndex;  // 0-based Index

	//************************************
	// Method:    SetSpectralAnalysis
	// FullName:  _SpectralAnalysisSetting::SetSpectralAnalysis
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: int StartFreq
	// Parameter: int StopFreq
	// Parameter: int Blocksize
	// Parameter: int CyclicAveg
	// Parameter: int Window
	// Parameter: int PercentOverlap
	// Parameter: int SamplingFreq
	//************************************
	void SetSpectralAnalysisParam(int StartFreq,int StopFreq,int Blocksize,int CyclicAveg, int Window, int PercentOverlap,int SamplingFreq);

};

struct _SystemIdentificationSetting {

	int         m_nSignalsPoints;             // 차후 계산
	int			m_nWhiteNoiseBorderFreq;
	int			m_nWhiteNoiseBorderFreqIndex; // 0 based index 차후 계산
	int         m_nWhiteNoiseAdded;           // 차후 계산
	double		m_dbWhiteNoiseTime;
	double		m_dbWhiteNoiseAlpha;
	double		m_dbWhiteNoiseSTD;

	void SetSystemIdentificationParam( double Time, int BorderFreq, double Alpha, double STD, int Blocksize, int SamplingFreq );

};

struct _TargetSimulationSetting {

	double				m_dbDesiredSignalsTime;
	int					m_nNumberOfChannelOfDesiredSignals;  // Primary Channel+ slave
	int					m_nNumberOfPointinDesiredSignals;
	int					m_nNumberOfBlockSize;
	bool                m_bSubMatchedDesiredSignals[50];
	int					m_nSubMatchedDesiredSignals;

};

struct SYSTEMSETTING {

	_FrontEndSetting					FRONTEND;
	_SpectralAnalysisSetting			SPECTRAL;
	_SystemIdentificationSetting		SI;
	_TargetSimulationSetting			TS;

}; 

#endif