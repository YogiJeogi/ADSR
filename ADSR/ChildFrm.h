
// ChildFrm.h : CChildFrame Ŭ������ �������̽�
// �����ɹ� ������ ���������ν� �ε��� ��Ȱ

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Ư���Դϴ�.
public:
	// ���� ���̽� �ѹ���
	static int m_staticNumbering;
	// Ÿ��Ʋ ������ �ֱ� ���� ���̴�.
	static CString m_strTitle;
	int m_iMyNumber;




// �۾��Դϴ�.
public:

// �������Դϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CChildFrame();
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
};
