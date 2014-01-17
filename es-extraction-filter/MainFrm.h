// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include <streams.h>
#include "dshowutil.h"
#include "VideoExtractFilter.h"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

	INT_PTR			m_nFrameCnt, m_nShownPos;
	INT_PTR			m_nHistDrawnCnt;
	INT_PTR			m_nCurrentFrmIndex;
	INT_PTR			m_nScrollPos;

	CRect			m_HistRect;

	CEdit			m_wndByteEdit;

	void			DrawHistogram(CDC *pdc);
	void			ReleaseFilters(void);

// 특성입니다.
public:

	IFilterGraph		*m_pFG;
	IGraphBuilder		*m_pBuilder;
	IBaseFilter			*m_pSrcFilter;
	IMediaEventEx		*m_pMediaEvent;
	IMediaControl		*m_pMC;
	IMediaPosition		*m_pMediaPosition;
	CVideoExtract		*m_pExtFilter;

	DWORD				m_dwGraphRegister;

	CScrollBar			m_scbar;

	INT_PTR				m_nDoneCount;

// 작업입니다.
public:
	LRESULT OnGraph(WPARAM wp, LPARAM lp);
	LRESULT OnRemoveGraph(WPARAM wp, LPARAM lp);
	LRESULT OnCompressDone(WPARAM wp, LPARAM lp);

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnPlayback();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnIpReg();
};
