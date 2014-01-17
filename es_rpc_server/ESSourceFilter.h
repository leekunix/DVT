#pragma once

class CESSourceFilter : public CSource
{
public:
	HANDLE	m_hFile;
	CTransServer *m_pSrv;

	CESSourceFilter(CTransServer *pSrv, HRESULT *phr);
	~CESSourceFilter();
};

class CESSourcePin : public CSourceStream
{
public:
    CESSourcePin(HRESULT *phr, CESSourceFilter *pParent, LPCWSTR pPinName);
	~CESSourcePin();

	CESSourceFilter *m_pFilter;

	REFERENCE_TIME	m_rtStart;

	// plots a ball into the supplied video frame
	HRESULT FillBuffer(IMediaSample *pms);

	// Ask for buffers of the size appropriate to the agreed media type
	HRESULT DecideBufferSize(IMemAllocator *pIMemAlloc,
							ALLOCATOR_PROPERTIES *pProperties);

	// Set the agreed media type, and set up the necessary ball parameters
	//HRESULT SetMediaType(const CMediaType *pMediaType);

	// Because we calculate the ball there is no reason why we
	// can't calculate it in any one of a set of formats...
	HRESULT CheckMediaType(const CMediaType *pMediaType);
	HRESULT GetMediaType(int iPosition, CMediaType *pmt);

	// Resets the stream time to zero
	HRESULT OnThreadCreate(void);

    // Quality control notifications sent to us
    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);
};
