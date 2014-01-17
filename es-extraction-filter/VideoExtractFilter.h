#pragma once

class CExtractInputPin;

enum
{
	VIDEO_XVID,
	VIDEO_DIVX,
	VIDEO_DIV3,
	VIDEO_H264,
	VIDEO_MP43,
	VIDEO_MAX
};


//class CVideoExtract : public CBaseFilter
class CVideoExtract : public CBaseRenderer
{
protected:
	HRESULT CheckMediaType(const CMediaType* pmt);

public:
	CVideoExtract(LPUNKNOWN pUnk, HRESULT* phr);
	~CVideoExtract();

	CCritSec		m_Lock;
	CCritSec		m_ReceiveLock;
	HANDLE			m_hFile;

	DWORD			m_nDSI_Length;
	PBYTE			m_dsi_info;

	bool			m_bWriteSeq;

	//bool			m_IsAVC;
	int				m_nVideoType;

	int				m_nSyncFrameCount;

	REFERENCE_TIME	avg_segment_length;
	REFERENCE_TIME	current_segment_st;
	REFERENCE_TIME	current_segment_length;
	int				num_intra_frm;

	int				m_idx;
	int				m_PrevIdx;

	REFERENCE_TIME	m_duration;
	int				m_partitions;

	CRPCThread		*pRpc;

	CExtractInputPin *m_pPin;          // A simple rendered input pin

	//bool SetFileName(LPCTSTR pszFileName);

	GUID			m_subtype;
	__int64			m_fps;
	int				m_width;
	int				m_height;
	bool			m_bFirstTimeWrite;

	virtual HRESULT SetMediaType(const CMediaType *pmt);
	HRESULT DoRenderSample(IMediaSample *pMediaSample);
	virtual HRESULT EndOfStream();
};
