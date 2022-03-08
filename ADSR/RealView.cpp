// ModelView.cpp : implementation file
//

#include "stdafx.h"
#include "ADSR.h"
#include "RealView.h"
#include "RealVDoc.h"
#include "MainFrm.h"
#include "RealFrame.h"


// CRealView

IMPLEMENT_DYNCREATE(CRealView, CFormView)

CRealView::CRealView()
	: CFormView(CRealView::IDD)
	, m_nECount(0)
	, m_nRCount(0)
	, m_TypeOfGraph(99)
	, m_ViewECtrlID(0)
	, m_ViewRCtrlID(0)
	, m_IsIndex(false)
	, m_bTargetPSDReady(false)
{
}

CRealView::~CRealView()
{
}

void CRealView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH_Real, m_graph);
}

BEGIN_MESSAGE_MAP(CRealView, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRealView message handlers

void CRealView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if ( m_TypeOfGraph == 99 ) return;
	CNiGraph* tGp =(CNiGraph*)GetDlgItem(IDC_CWGRAPH_Real);
	if (!tGp)
	{
		return;
	}

	// ����� Ŭ���̾�Ʈ ��ǥ ����
	CRect tRect;
	GetClientRect(tRect);
    // ����� ��ǥ �������� �並 Fit��Ŵ���μ�
    // ��ũ�ѹٰ� �Ȼ������.
	SIZE tSize;
	tSize.cy=tRect.bottom;
	tSize.cx=tRect.right;
	if ( tSize.cy < 1 ) {tSize.cy=1; }
	if ( tSize.cx < 1 ) {tSize.cx=1; }
	GetParentFrame()->RecalcLayout();
	SetScaleToFitSize(tSize);

	tGp->SetWindowPos(&CWnd::wndBottom, tRect.left, tRect.top, tRect.right,
						tRect.bottom,SWP_SHOWWINDOW );
}

BOOL CRealView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	return CFormView::PreCreateWindow(cs);
}

void CRealView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

}

// CRealView diagnostics

#ifdef _DEBUG
void CRealView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CRealView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CRealView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	CRealVDoc		* pRealVDoc	 =	(CRealVDoc*)GetDocument();
	CMainFrame		* pMainFrame =	(CMainFrame*)AfxGetMainWnd();
	CRealFrame      * pRealFrame =  (CRealFrame*)GetParentFrame();

	//AfxMessageBox(" CRealView::OnUpdata called");
	/* ===========================================
	 * �� �ε����� �ϴ� �κ��̴�
	 * ó�� ��ť��Ʈ�� ������ �ѹ� ȣ��ȴ�. bool
	 * ===========================================*/
	if ( m_IsIndex== false)
	{
		m_TypeOfGraph=pRealVDoc->m_TypeOfGraph;
		m_nRCount=pRealVDoc->m_nRCount;
		m_nECount=pRealVDoc->m_nECount;

		if ( m_TypeOfGraph == 1 )
		{
//			AfxMessageBox("TyepOfGraph 1 Called");
			m_ViewRCtrlID=pMainFrame->m_aryRListSel[pRealVDoc->m_PreViewRCtrlID];
			m_ViewECtrlID=pMainFrame->m_aryEListSel[pRealVDoc->m_PreViewECtrlID];
			pRealFrame->SetWindowText(System.SS.FRONTEND.m_arrstrResponseChannelName[m_ViewRCtrlID].c_str());
		}
		else 
		{
			// �信 ��ť��Ʈ�� ����� �ӽ� ID�� �̿��Ͽ� ������ �信 ID�� �ο��Ѵ�.
			m_ViewRCtrlID=pMainFrame->m_aryRListSel[pRealVDoc->m_PreViewRCtrlID];
			// ������ ID�� �̿��Ͽ� ������ Ÿ��Ʋ�� �����.
			// �� Ÿ��Ʋ�� �� ���ϵ� �������� Ÿ��Ʋ�� �ǰڴ�.
			// ���� :: SetTitle�� ������ ��ü�� Ÿ��Ʋ���� ������ Ÿ��Ʋ�� �ƴϴ�.
			pRealFrame->SetWindowText(System.SS.FRONTEND.m_arrstrResponseChannelName[m_ViewRCtrlID].c_str());
		}
		m_IsIndex=true;
	}// �ε��� ��


	// ����Ÿ ó�� �κ��̴�.
	//void CopyColumn( unsigned int columnIndex
	//				 , CNiVectorT<DataType>& destVector ) const

	/*  ========================================== 
	 *          �׷��� �׸��� ����
	 *   �� �䰡 ���� m_TypeOfGraph ������ ����
	 *   �׿� �´� �׸��� �׸��� �ȴ�.
	 *   =========================================*/

	if ( m_TypeOfGraph == 0 )
	{
		if(pRealVDoc->m_bType0dataReady==true)
		{
			g_CriticalSection.Lock();
			pRealVDoc->m_RespData.CopyRow(m_ViewRCtrlID,m_RespDataVec);
			g_CriticalSection.Unlock();

			//AfxMessageBox(" Plot called");
			//if ( pResultDoc->m_bWhiteDataReady==true )
			m_graph.PlotY(m_RespDataVec);
			//m_graph.EnableWindow(false);
		}
	}
	
	if ( m_TypeOfGraph == 1)
	{
		if(pRealVDoc->m_bType1dataReady==true)
		{
			CNiReal64Vector tVec(pRealVDoc->m_HAmplitude.dim3()
				,pRealVDoc->m_HAmplitude[m_ViewRCtrlID][m_ViewECtrlID]);
	
			m_SIHAmplitude=tVec;
// 			m_SIHAmplitude.SetSize(pRealVDoc->m_HAmplitude.dim3());
// 			for ( int jj=0; jj < pRealVDoc->m_HAmplitude.dim3() ; jj++)
// 				m_SIHAmplitude[jj]=pRealVDoc->m_HAmplitude[m_ViewRCtrlID][m_ViewECtrlID][jj];
// 			
			m_graph.Axes.Item("YAxis-1").Log=true;
		    m_graph.PlotY(tVec);
		}
	}

	if ( m_TypeOfGraph == 2)
	{
		// TS
		if(pRealVDoc->m_bType2dataReady==true)
		{
			if (m_bTargetPSDReady==false)
			{
				m_TSTargetPSDVec.SetSize(pRealVDoc->m_matTSTargetPSD.dim3());
				for ( int jj=0; jj < pRealVDoc->m_matTSTargetPSD.dim3() ; jj++)
					m_TSTargetPSDVec[jj]=pRealVDoc->m_matTSTargetPSD[1][m_ViewRCtrlID][jj];
				m_bTargetPSDReady=true;
			}
	
			m_TSRespPSDVec.SetSize(pRealVDoc->m_matTSRespPSD.ncols());
			for ( int jj=0; jj < pRealVDoc->m_matTSRespPSD.ncols() ; jj++)
				m_TSRespPSDVec[jj]=pRealVDoc->m_matTSRespPSD[m_ViewRCtrlID][jj];

			

// 			m_graph.Plots.Item(1).PlotY(m_TSTargetPSDVec);
// 			m_graph.Plots.Item(2).PlotY(m_TSRespPSDVec);
			m_graph.Plots.Item(1).PlotY(m_TSRespPSDVec);
			m_graph.Plots.Item(2).PlotY(m_TSTargetPSDVec);
			m_graph.Plots.Item(1).YAxis.Log=true;
			m_graph.Plots.Item(1).YAxis.AutoScale=true;
			m_graph.Plots.Item(2).YAxis.Log=true;
			m_graph.Plots.Item(2).YAxis.AutoScale=true;
	

		}
	}


}
