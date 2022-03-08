#pragma once
#include "NiIncludes.h"

// CResultVDoc document

class CResultVDoc : public CDocument
{
	DECLARE_DYNCREATE(CResultVDoc)

public:
	int nTYPE;
	int nSelectedSignal;
	int nCheckedViewMode;

public:
	bool				m_bWhiteDataReady;
	MatDoub				m_matTimeData;
	CNiReal64Matrix		m_TimeMatrix;

	// 뷰를 콘트롤할 인덱스를 넣어줄 변수
	// 선택된 채널을 간접적으로 알려준다.
	unsigned int        m_PreViewCtrlID1;
	unsigned int		m_PreViewCtrlID2;

	// 데이타 저장변수들이다.
	
	// TYPE 1 저장 변수들
	MatDoub				m_matSIWhiteNoise;
	MatDoub				m_matSIResponse;
	MatDoub				m_matTSDrive;

	// TYPE 1과 3 공통 변수들
	Mat3DDoub			m_matTSResponseSignals;
	Mat3DDoub			m_matTSDesiredSignals;
	Mat3DDoub			m_matTSTargetPSD; 
	Mat3DDoub			m_matTSRespPSD;

	// TYPE 2 저장 변수들
	Mat3DDoub			m_HAmplitude;
	Mat3DDoub			m_HAngle;

public:
	CResultVDoc();
	virtual ~CResultVDoc();
	void GetTimedata(MatDoub &t_TimeMatrix);

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
};
