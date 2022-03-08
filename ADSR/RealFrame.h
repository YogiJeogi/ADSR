#pragma once


// CRealFrame

class CRealFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CRealFrame)
public:
	CRealFrame();

	// 특성입니다.
public:
	// 작업입니다.
public:

	// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 구현입니다.
public:
	virtual ~CRealFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
