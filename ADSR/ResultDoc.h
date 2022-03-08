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

	// �並 ��Ʈ���� �ε����� �־��� ����
	// ���õ� ä���� ���������� �˷��ش�.
	unsigned int        m_PreViewCtrlID1;
	unsigned int		m_PreViewCtrlID2;

	// ����Ÿ ���庯�����̴�.
	
	// TYPE 1 ���� ������
	MatDoub				m_matSIWhiteNoise;
	MatDoub				m_matSIResponse;
	MatDoub				m_matTSDrive;

	// TYPE 1�� 3 ���� ������
	Mat3DDoub			m_matTSResponseSignals;
	Mat3DDoub			m_matTSDesiredSignals;
	Mat3DDoub			m_matTSTargetPSD; 
	Mat3DDoub			m_matTSRespPSD;

	// TYPE 2 ���� ������
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
