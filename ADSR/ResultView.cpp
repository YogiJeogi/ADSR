// TimeView.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "ResultView.h"
#include "ResultDoc.h"
#include "NiIncludes.h"
#include "ChildFrm.h"


// CResultView

IMPLEMENT_DYNCREATE(CResultView, CFormView)

CResultView::CResultView()
	: CFormView(CResultView::IDD)
	,m_ViewCtrlID1(0)
	,m_ViewCtrlID2(0)
	,m_IsIndex(false)
{
}


CResultView::~CResultView()
{
}

void CResultView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH_Time,    m_GraphTime);
	DDX_Control(pDX, IDC_CWGRAPH_PSD,     m_GraphPSD);
	DDX_Control(pDX, IDC_CWGRAPH_XYPLOT,  m_GraphXY);
	DDX_Control(pDX, IDC_LIST_STATISTICS, m_CtrlListStatistic);
}

BEGIN_MESSAGE_MAP(CResultView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CResultView diagnostics

#ifdef _DEBUG
void CResultView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CResultView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CResultView message handlers

void CResultView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// 변경된 클라이언트 좌표 구함
	CRect tRect;
	GetClientRect(tRect);
	
	SIZE tSize;
	tSize.cy=tRect.bottom;
	tSize.cx=tRect.right;
	if ( tSize.cy < 1 ) {tSize.cy=1; }
	if ( tSize.cx < 1 ) {tSize.cx=1; }
	// 다시 그려줌
	GetParentFrame()->RecalcLayout();
	// 뷰 리사이즈
	// 변경된 좌표 기준으로 뷰를 Fit시킴으로서 스크롤바가 안생기게함.
	SetScaleToFitSize(tSize);


	if ( m_IsIndex==true )
	{
		if( m_nTYPE!=2)	TypeSizing();
		else Type2Sizing();
	}
}

// 도큐먼트의 UpdataAllView 처리 부분이다.
void CResultView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CResultVDoc		*pResultVDoc	=(CResultVDoc*)GetDocument();

	/* ==========================
	 * 뷰 인덱스를 하는 부분이다
	 * ==========================*/
	if ( m_IsIndex== false)
	{
		m_GraphTime.SetWindowPos(&wndTop,0,0,0,0,SWP_HIDEWINDOW);
		m_GraphPSD.SetWindowPos(&wndTop,0,0,0,0,SWP_HIDEWINDOW);
		m_GraphXY.SetWindowPos(&wndTop,0,0,0,0,SWP_HIDEWINDOW);
		m_CtrlListStatistic.SetWindowPos(&wndTop,0,0,0,0,SWP_HIDEWINDOW);

		m_ViewCtrlID1=pResultVDoc->m_PreViewCtrlID1;
		m_ViewCtrlID2=pResultVDoc->m_PreViewCtrlID2;
		m_nTYPE=pResultVDoc->nTYPE;
		m_nSelectedSignal=pResultVDoc->nSelectedSignal;
		SpecifiedViewMode(pResultVDoc->nCheckedViewMode);

		if ( m_nTYPE==3)
		GetParentFrame()->SetWindowText(System.SS.FRONTEND.m_arrstrResponseChannelName[m_ViewCtrlID1].c_str());

		m_IsIndex=true;
		if ( m_nTYPE==2)
		{m_GraphTime.ShowWindow(SW_SHOW); m_GraphPSD.ShowWindow(SW_SHOW);}
	}

	PlotGraph();
}

BOOL CResultView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::PreCreateWindow(cs);
}


// 도큐먼트가 뷰에 붙은 후에 호출된다.
void CResultView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void CResultView::SpecifiedViewMode(int nCheckedViewMode)
{
	if( (nCheckedViewMode&1) == 1 )
	{
		m_bTimeHistory=true;
		m_GraphTime.SetWindowPos(&wndTop,0,0,0,0,SWP_SHOWWINDOW);
	}
	if( (nCheckedViewMode&2) == 2 ) 
	{
		m_bPSD=true;
		m_GraphPSD.SetWindowPos(&wndTop,0,0,0,0,SWP_SHOWWINDOW);
	}
	if( (nCheckedViewMode&4) == 4 )
	{
		m_bXY=true;
		m_GraphXY.SetWindowPos(&wndTop,0,0,0,0,SWP_SHOWWINDOW);
	}
	if( (nCheckedViewMode&8) == 8 )
	{
		m_bStatistic=true;
		m_CtrlListStatistic.SetWindowPos(&wndTop,0,0,0,0,SWP_SHOWWINDOW);
	}
}

void CResultView::TypeSizing(void)
{
	CRect rectV;
	GetClientRect(&rectV);
	int VH=rectV.Height();
	int VW=rectV.Width();

	if ( m_bTimeHistory==true && m_bPSD==true)
	{
		if( m_bXY==true && m_bStatistic==true )
		{
			m_GraphTime.SetWindowPos		(&wndTop,0		,0		,VW*7/10,VH/2,0);
			m_GraphPSD.SetWindowPos			(&wndTop,0		,VH/2	,VW/2,VH/2,0);
			m_CtrlListStatistic.SetWindowPos(&wndTop,VW*7/10,0		,VW*3/10,VH/2,0);
			m_GraphXY.SetWindowPos			(&wndTop,VW/2	,VH/2	,VW/2,VH/2,0);
		}
		else if( m_bXY==true || m_bStatistic==true )
		{
			m_GraphTime.SetWindowPos		(&wndTop,0		,0		,VW		,VH/2,0);
			m_GraphPSD.SetWindowPos			(&wndTop,0		,VH/2	,VW*7/10,VH/2,0);
			m_CtrlListStatistic.SetWindowPos(&wndTop,VW*7/10,VH/2	,VW*3/10,VH/2,0);
			m_GraphXY.SetWindowPos			(&wndTop,VW*7/10,VH/2	,VW*3/10,VH/2,0);
		}
		else
		{
			m_GraphTime.SetWindowPos		(&wndTop,0		,0		,VW,  VH/2,0);
			m_GraphPSD.SetWindowPos			(&wndTop,0		,VH/2	,VW,  VH/2,0);
		}
	}
	else if ( m_bTimeHistory==true || m_bPSD==true)
	{
		if( m_bXY==true && m_bStatistic==true )
		{
			m_GraphTime.SetWindowPos		(&wndTop,0		,0		,VW,  VH/2,0);
			m_GraphPSD.SetWindowPos			(&wndTop,0		,0		,VW,  VH/2,0);
			m_CtrlListStatistic.SetWindowPos(&wndTop,VW*7/10,VH/2	,VW/2,VH/2,0);
			m_GraphXY.SetWindowPos			(&wndTop,0		,VH/2	,VW*7/10,VH/2,0);
		}
		else if( m_bXY==true || m_bStatistic==true )
		{
			m_GraphTime.SetWindowPos		(&wndTop,0		,0		,VW*7/10,VH,0);
			m_GraphPSD.SetWindowPos			(&wndTop,0		,0		,VW*7/10,VH,0);
			m_CtrlListStatistic.SetWindowPos(&wndTop,VW*7/10,0		,VW/2,VH,0);
			m_GraphXY.SetWindowPos			(&wndTop,VW*7/10,0		,VW/2,VH,0);
		}
		else
		{
			m_GraphTime.SetWindowPos		(&wndTop,0		,0		,VW,VH,0);
			m_GraphPSD.SetWindowPos			(&wndTop,0		,0		,VW,VH,0);
		}
	}
	else
	{
		if( m_bXY==true && m_bStatistic==true )
		{
			m_CtrlListStatistic.SetWindowPos(&wndTop,VW*7/10,0		,VW/2,VH,0);
			m_GraphXY.SetWindowPos			(&wndTop,0		,0		,VW*7/10,VH,0);
		}
		else if( m_bXY==true || m_bStatistic==true )
		{
			m_CtrlListStatistic.SetWindowPos(&wndTop,0		,0		,VW,VH,0);
			m_GraphXY.SetWindowPos			(&wndTop,0		,0		,VW,VH,0);
		}
		else
		{

		}
	}
}



void CResultView::Type2Sizing(void)
{
	CRect rectV;
	GetClientRect(&rectV);
	int VH=rectV.Height();
	int VW=rectV.Width();
	m_GraphTime.SetWindowPos	(&wndTop,0		,0		,VW,  VH/2,0);
	m_GraphPSD.SetWindowPos		(&wndTop,0		,VH/2	,VW,  VH/2,0);
}

void CResultView::PlotGraph(void)
{
	CResultVDoc		*pResultVDoc	=(CResultVDoc*)GetDocument();

	if ( m_nTYPE==1 && m_nSelectedSignal==100)
	{
		CNiReal64Vector tVec(theApp.m_SIWhiteNoiseTimeData.ncols()
			,theApp.m_SIWhiteNoiseTimeData[m_ViewCtrlID1]);
		m_GraphTime.PlotY(tVec);
	}
	if ( m_nTYPE==1 && m_nSelectedSignal==101)
	{
		CNiReal64Vector tVec(pResultVDoc->m_matTSDrive.ncols()
			,pResultVDoc->m_matTSDrive[m_ViewCtrlID1]);
		m_GraphTime.PlotY(tVec);
	}
	if ( m_nTYPE==1 && m_nSelectedSignal==102)
	{
		CNiReal64Vector tVec(pResultVDoc->m_matSIResponse.ncols()
			,pResultVDoc->m_matSIResponse[m_ViewCtrlID1]);
		m_GraphTime.PlotY(tVec);
	}
	if ( m_nTYPE==1 && m_nSelectedSignal==103) // TS Response
	{
		CNiReal64Vector tVec(pResultVDoc->m_matTSResponseSignals.dim3()
			,pResultVDoc->m_matTSResponseSignals[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec2(pResultVDoc->m_matTSRespPSD.dim3()
			,pResultVDoc->m_matTSRespPSD[0][m_ViewCtrlID1]);

		m_TSUnfilRespVec=tVec;
		m_TSUnfilRespPSDVec=tVec2;
		m_GraphTime.PlotY(tVec);
		m_GraphPSD.PlotY(tVec2);
	}
	if ( m_nTYPE==1 && m_nSelectedSignal==104) // Desired Response
	{
		CNiReal64Vector tVec(pResultVDoc->m_matTSDesiredSignals.dim3()
			,pResultVDoc->m_matTSDesiredSignals[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec2(pResultVDoc->m_matTSTargetPSD.dim3()
			,pResultVDoc->m_matTSTargetPSD[0][m_ViewCtrlID1]);

		m_TSUnfilRespVec=tVec;
		m_TSUnfilRespPSDVec=tVec2;
		m_GraphTime.PlotY(tVec);
		m_GraphPSD.PlotY(tVec2);
	}
	if ( m_nTYPE==2 && m_nSelectedSignal==200) // H Model
	{
		CNiReal64Vector tVec(pResultVDoc->m_HAmplitude.dim3()
			,pResultVDoc->m_HAmplitude[m_ViewCtrlID1][m_ViewCtrlID2]);
		CNiReal64Vector tVec2(pResultVDoc->m_HAngle.dim3()
			,pResultVDoc->m_HAngle[m_ViewCtrlID1][m_ViewCtrlID2]);

		m_SIHAmplitude=tVec;
		m_SIHAngle=tVec2;
		m_GraphTime.PlotY(tVec);
		m_GraphPSD.PlotY(tVec2);
	}

	if ( m_nTYPE==3 && m_nSelectedSignal==300) // Desired
	{
		CNiReal64Vector tVec(pResultVDoc->m_matTSDesiredSignals.dim3()
			,pResultVDoc->m_matTSDesiredSignals[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec2(pResultVDoc->m_matTSDesiredSignals.dim3()
			,pResultVDoc->m_matTSDesiredSignals[1][m_ViewCtrlID1]);
		m_GraphTime.Plots.Item(1).PlotY(tVec);
		m_GraphTime.Plots.Item(2).PlotY(tVec2);
		m_GraphXY.PlotXvsY(tVec,tVec2);

		CNiReal64Vector tVec3(pResultVDoc->m_matTSTargetPSD.dim3()
			,pResultVDoc->m_matTSTargetPSD[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec4(pResultVDoc->m_matTSTargetPSD.dim3()
			,pResultVDoc->m_matTSTargetPSD[1][m_ViewCtrlID1]);
		m_GraphPSD.Plots.Item(1).PlotY(tVec3);
		m_GraphPSD.Plots.Item(2).PlotY(tVec4);
		m_GraphPSD.Plots.Item(1).YAxis.Log=true;
		m_GraphPSD.Plots.Item(1).YAxis.AutoScale=true;
		m_GraphPSD.Plots.Item(2).YAxis.Log=true;
		m_GraphPSD.Plots.Item(2).YAxis.AutoScale=true;
	}
	if ( m_nTYPE==3 && m_nSelectedSignal==301) // Response
	{
		CNiReal64Vector tVec(pResultVDoc->m_matTSResponseSignals.dim3()
			,pResultVDoc->m_matTSResponseSignals[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec2(pResultVDoc->m_matTSResponseSignals.dim3()
			,pResultVDoc->m_matTSResponseSignals[1][m_ViewCtrlID1]);
		m_GraphTime.Plots.Item(1).PlotY(tVec);
		m_GraphTime.Plots.Item(2).PlotY(tVec2);
		m_GraphXY.PlotXvsY(tVec,tVec2);

		CNiReal64Vector tVec3(pResultVDoc->m_matTSRespPSD.dim3()
			,pResultVDoc->m_matTSRespPSD[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec4(pResultVDoc->m_matTSRespPSD.dim3()
			,pResultVDoc->m_matTSRespPSD[1][m_ViewCtrlID1]);
		m_GraphPSD.Plots.Item(1).PlotY(tVec3);
		m_GraphPSD.Plots.Item(2).PlotY(tVec4);
		m_GraphPSD.Plots.Item(1).YAxis.Log=true;
		m_GraphPSD.Plots.Item(1).YAxis.AutoScale=true;
		m_GraphPSD.Plots.Item(2).YAxis.Log=true;
		m_GraphPSD.Plots.Item(2).YAxis.AutoScale=true;
	}
	if ( m_nTYPE==3 && m_nSelectedSignal==302) // Unfilterd
	{
		CNiReal64Vector tVec1(pResultVDoc->m_matTSDesiredSignals.dim3()
			,pResultVDoc->m_matTSDesiredSignals[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec2(pResultVDoc->m_matTSResponseSignals.dim3()
			,pResultVDoc->m_matTSResponseSignals[0][m_ViewCtrlID1]);
		m_GraphTime.Plots.Item(1).PlotY(tVec2);
		m_GraphTime.Plots.Item(2).PlotY(tVec1);
		m_GraphXY.PlotXvsY(tVec1,tVec2);

		CNiReal64Vector tVec3(pResultVDoc->m_matTSTargetPSD.dim3()
			,pResultVDoc->m_matTSTargetPSD[0][m_ViewCtrlID1]);
		CNiReal64Vector tVec4(pResultVDoc->m_matTSRespPSD.dim3()
			,pResultVDoc->m_matTSRespPSD[0][m_ViewCtrlID1]);
		m_GraphPSD.Plots.Item(1).PlotY(tVec4);
		m_GraphPSD.Plots.Item(2).PlotY(tVec3);
		m_GraphPSD.Plots.Item(1).YAxis.Log=true;
		m_GraphPSD.Plots.Item(1).YAxis.AutoScale=true;
		m_GraphPSD.Plots.Item(2).YAxis.Log=true;
		m_GraphPSD.Plots.Item(2).YAxis.AutoScale=true;
	}
	if ( m_nTYPE==3 && m_nSelectedSignal==303) // Filterd
	{
		CNiReal64Vector tVec1(pResultVDoc->m_matTSDesiredSignals.dim3()
			,pResultVDoc->m_matTSDesiredSignals[1][m_ViewCtrlID1]);
		CNiReal64Vector tVec2(pResultVDoc->m_matTSResponseSignals.dim3()
			,pResultVDoc->m_matTSResponseSignals[1][m_ViewCtrlID1]);
		m_GraphTime.Plots.Item(1).PlotY(tVec2);
		m_GraphTime.Plots.Item(2).PlotY(tVec1);
		m_GraphXY.PlotXvsY(tVec1,tVec2);

		CNiReal64Vector tVec3(pResultVDoc->m_matTSTargetPSD.dim3()
			,pResultVDoc->m_matTSTargetPSD[1][m_ViewCtrlID1]);
		CNiReal64Vector tVec4(pResultVDoc->m_matTSRespPSD.dim3()
			,pResultVDoc->m_matTSRespPSD[1][m_ViewCtrlID1]);
		m_GraphPSD.Plots.Item(1).PlotY(tVec4);
		m_GraphPSD.Plots.Item(2).PlotY(tVec3);
		m_GraphPSD.Plots.Item(1).YAxis.Log=true;
		m_GraphPSD.Plots.Item(1).YAxis.AutoScale=true;
		m_GraphPSD.Plots.Item(2).YAxis.Log=true;
		m_GraphPSD.Plots.Item(2).YAxis.AutoScale=true;
	}
}
