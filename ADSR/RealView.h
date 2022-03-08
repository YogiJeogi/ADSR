#pragma once
#include "nigraph.h"



// CRealView form view

// �� �׷��� �� �ش�ȴ�.
// ��ť��Ʈ�� �����͸� �ҷ��ͼ� ����Ÿ�� �����ش�.
// �׷������� ����Ǵ� ���콺 �޼����� ó���Ѵ�.

class CRealView : public CFormView
{
	DECLARE_DYNCREATE(CRealView)

protected:
	CRealView();           // protected constructor used by dynamic creation
	virtual ~CRealView();

public:
	enum { IDD = IDD_RealVIEW };

// ���� ����
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


