#pragma once
#include "nigraph.h"
#include "afxcmn.h"



// CResultView form view

class CResultView : public CFormView
{
	DECLARE_DYNCREATE(CResultView)

protected:
	CResultView();           // protected constructor used by dynamic creation

	virtual ~CResultView();

public:
	enum { IDD = IDD_ResultVIEW };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


//////////////////////////////////////////////////
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

public:

	// �׷����� ���� ��Ʈ���̴�.
	NI::CNiGraph m_GraphTime;
	NI::CNiGraph m_GraphPSD;
	NI::CNiGraph m_GraphXY;
	CListCtrl	 m_CtrlListStatistic;

	// ���� ������ ��Ÿ���� ������
	unsigned int m_ViewCtrlID1;
	unsigned int m_ViewCtrlID2;
	int			 m_nTYPE;
	int			 m_nSelectedSignal;

	// üũ��ư�� ���� ����
	bool         m_bTimeHistory;
	bool		 m_bPSD;
	bool		 m_bXY;
	bool		 m_bStatistic;

	// ó�� �ε����� �Ҷ�
	bool         m_IsIndex;

	// ����Ÿ ����
	NI::CNiReal64Vector m_TSUnfilTargetVec;
	NI::CNiReal64Vector m_TSFilTargetVec;
	NI::CNiReal64Vector m_TSUnfilTargetPSDVec;
	NI::CNiReal64Vector m_TSFilTargetPSDVec;

	NI::CNiReal64Vector m_TSUnfilRespVec;
	NI::CNiReal64Vector m_TSFilRespVec;
	NI::CNiReal64Vector m_TSUnfilRespPSDVec;
	NI::CNiReal64Vector m_TSFilRespPSDVec;

	NI::CNiReal64Vector m_RespDataVec;

	NI::CNiReal64Vector m_SIHAmplitude;
	NI::CNiReal64Vector m_SIHAngle;


protected:

public:
	
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
	void SpecifiedViewMode(int nCheckedViewMode);
	void TypeSizing(void);
	void Type2Sizing(void);
	void PlotGraph(void);
};


