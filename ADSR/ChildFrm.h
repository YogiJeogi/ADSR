
// ChildFrm.h : CChildFrame 클래스의 인터페이스
// 정적맴버 변수를 선언함으로써 인덱스 역활

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 특성입니다.
public:
	// 제로 베이스 넘버링
	static int m_staticNumbering;
	// 타이틀 제목을 넣기 위한 것이다.
	static CString m_strTitle;
	int m_iMyNumber;




// 작업입니다.
public:

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildFrame();
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
};
