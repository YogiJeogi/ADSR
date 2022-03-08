#include "preferST.h"


void _SpectralAnalysisSetting::SetSpectralAnalysisParam( int StartFreq,int StopFreq,int Blocksize,int CyclicAveg, int Window, int PercentOverlap,int SamplingFreq )
{

		m_nStartFreq=StartFreq;
		m_nStopFreq=StopFreq;
		m_nBlocksize=Blocksize;
		m_nCyclicAveg=CyclicAveg;
		m_nWindow=Window;
		m_nPercentofOverlap=PercentOverlap;

		// ����� ���� �̿��� �ٸ� ���� ���
		m_nStartFreqIndex=Blocksize/SamplingFreq*StartFreq;  // Zero Based Index (ex. bs=8,fs=4,startfreq=1hz��� 2�ε����� ������ 0Hz 0.5Hz 1Hz�� ���Ѵ�.
		m_nStopFreqIndex=Blocksize/SamplingFreq*StopFreq;	 // Zero Based Index
		m_nPshifted=int(Blocksize*(1.-double(PercentOverlap)/100.));

}

void _SystemIdentificationSetting::SetSystemIdentificationParam( double Time, int BorderFreq, double Alpha, double STD, int Blocksize, int SamplingFreq )
{

        // ���� ����
        m_dbWhiteNoiseTime=Time;
		m_nWhiteNoiseBorderFreq=BorderFreq;
		m_dbWhiteNoiseAlpha=Alpha;
		m_dbWhiteNoiseSTD=STD;

		// ���� ���
		m_nWhiteNoiseAdded				=	(int)(Time	*	SamplingFreq/Blocksize);
        m_nSignalsPoints				=	Blocksize	*	m_nWhiteNoiseAdded;
		m_nWhiteNoiseBorderFreqIndex	=	Blocksize/SamplingFreq*BorderFreq;

}
