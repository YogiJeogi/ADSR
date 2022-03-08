#pragma once
#include "nigraph.h"



// CRealView form view

// 모델 그래프 뷰어에 해당된다.
// 다큐먼트의 데이터를 불러와서 데이타를 보여준다.
// 그래프에서 적용되는 마우스 메세지를 처리한다.

class CRealView : public CFormView
{
	DECLARE_DYNCREATE(CRealView)

protected:
	CRealView();           // protected constructor used by dynamic creation
	virtual ~CRealView();

public:
	enum { IDD = IDD_RealVIEW };

// 개인 정의
public:
	bool	m_IsIndex;
	int		m_TypeOfGraph;
	int		m_nECount;
	int		m_nRCount;
	int		m_ViewRCtrlID;
	int		m_ViewECtrlID;

	NI::CNiReal64Vector m_RespDataVec;
	NI::CNiReal64Vector m_SIHAmplitude;
	NI::CNiReal64Vector m_SIHAngle;
	NI::CNiReal64Vector m_TSRespPSDVec;
	NI::CNiReal64Vector m_TSTargetPSDVec;
	
	bool m_bTargetPSDReady;

public:
	NI::CNiGraph m_graph;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();


////////////////////////////////////////////////
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

};


