#pragma once


// CRealFrame

class CRealFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CRealFrame)
public:
	CRealFrame();

	// Ư���Դϴ�.
public:
	// �۾��Դϴ�.
public:

	// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// �����Դϴ�.
public:
	virtual ~CRealFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
