#pragma once

// CRealVDoc document

class CRealVDoc : public CDocument
{
	DECLARE_DYNCREATE(CRealVDoc)

public:
	CRealVDoc();
	virtual ~CRealVDoc();
/*	void ModifyFrameTitle(int iModelView);*/

// 개인 정의
public:

	int		m_TypeOfGraph;
	int		m_nECount;
	int		m_nRCount;
	int     m_PreViewRCtrlID;
	int     m_PreViewECtrlID;

	NI::CNiReal64Matrix m_RespData;

	Mat3DDoub m_HAmplitude;
	Mat3DDoub m_HAngle;

	Mat3DDoub m_matTSTargetPSD;

	MatDoub m_matTSRespPSD;

	bool    m_bType0dataReady;
	bool    m_bType1dataReady;
	bool    m_bType2dataReady;

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
