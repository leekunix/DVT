#include <streams.h>
#include <initguid.h>
#include <assert.h>

#include "FrameInfo.h"
#include "ESSourceFilter.h"
#include "moreuuids.h"

#pragma warning(disable:4710)

// {387AD9EE-8A19-4b52-A092-BC7F33EB56BD}
DEFINE_GUID(CLSID_ESSourceFiler, 
	0x387ad9ee, 0x8a19, 0x4b52, 0xa0, 0x92, 0xbc, 0x7f, 0x33, 0xeb, 0x56, 0xbd);

//extern GUID			videoType;
//extern __int64		AvgTimePerFrame;
//extern int			gVideoWidth;
//extern int			gVideoHeight;
//extern list<CFrmInfo*> frmInfoList;

CESSourceFilter::CESSourceFilter(CTransServer *pSrv, HRESULT *phr) :
	CSource(NAME("ESSourceFiler"), 0, CLSID_ESSourceFiler)
{
    assert(phr);
    CAutoLock cAutoLock(&m_cStateLock);

	m_pSrv = pSrv;
	m_hFile = INVALID_HANDLE_VALUE;

    m_paStreams = (CSourceStream **) new CESSourcePin*[1];
    if(m_paStreams == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;

        return;
    }

    m_paStreams[0] = new CESSourcePin(phr, this, L"ES Source Pin");
    if(m_paStreams[0] == NULL)
    {
        if(phr)
            *phr = E_OUTOFMEMORY;

        return;
    }
}

CESSourceFilter::~CESSourceFilter()
{
	if( m_hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

CESSourcePin::CESSourcePin(HRESULT *phr,
                         CESSourceFilter *pParent,
                         LPCWSTR pPinName) :
    CSourceStream(NAME("ESSource"),phr, pParent, pPinName)
{
    assert(phr);
	//CAutoLock cAutoLock(&m_cSharedState);

	m_pFilter = pParent;

} // (Constructor)

CESSourcePin::~CESSourcePin()
{
}

HRESULT CESSourcePin::DecideBufferSize(IMemAllocator *pAlloc,
                                      ALLOCATOR_PROPERTIES *pProperties)
{
    CheckPointer(pAlloc,E_POINTER);
    CheckPointer(pProperties,E_POINTER);

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    HRESULT hr = NOERROR;

    VIDEOINFO *pvi = (VIDEOINFO *) m_mt.Format();
    pProperties->cBuffers = 4;
    pProperties->cbBuffer = pvi->bmiHeader.biSizeImage;

    //pProperties->cBuffers = 4;
    //pProperties->cbBuffer = 4*1024*1024;

    assert(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if(FAILED(hr))
    {
        return hr;
    }

    // Is this allocator unsuitable

    if(Actual.cbBuffer < pProperties->cbBuffer)
    {
        return E_FAIL;
    }

	return NOERROR;
} // DecideBufferSize

HRESULT CESSourcePin::GetMediaType(int iPosition, CMediaType *pmt)
{
    CheckPointer(pmt,E_POINTER);
	const GUID type = m_pFilter->m_pSrv->videoType;

    CAutoLock cAutoLock(m_pFilter->pStateLock());
    if( (iPosition < 0) || (iPosition > 0 ) )
    {
        return E_INVALIDARG;
    }

	pmt->InitMediaType();

    VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pmt->AllocFormatBuffer(sizeof(VIDEOINFOHEADER));
    if(NULL == pvi)
        return(E_OUTOFMEMORY);

    ZeroMemory(pvi, sizeof(VIDEOINFOHEADER));

	pvi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pvi->bmiHeader.biWidth = m_pFilter->m_pSrv->m_VideoWidth;
	pvi->bmiHeader.biHeight = m_pFilter->m_pSrv->m_VideoHeight;
	pvi->bmiHeader.biBitCount = 24;
	pvi->bmiHeader.biPlanes = 1;
	pvi->bmiHeader.biSizeImage = pvi->bmiHeader.biWidth * 
								pvi->bmiHeader.biHeight * 
								(pvi->bmiHeader.biBitCount/8);


	if( type == MEDIASUBTYPE_XVID ||
		type == MEDIASUBTYPE_xvid )
	{
		pvi->bmiHeader.biCompression = 'DIVX';
	}
	else if( type == MEDIASUBTYPE_AVC1 ||
			type == MEDIASUBTYPE_avc1 ||
			type == MEDIASUBTYPE_H264 ||
			type == MEDIASUBTYPE_h264 )
	{
		pvi->bmiHeader.biCompression = '1CVA';
	}
	else
	{
		return E_INVALIDARG;
	}

	SetRectEmpty(&(pvi->rcSource)); // we want the whole image area rendered.
	SetRectEmpty(&(pvi->rcTarget)); // no particular destination rectangle

	pmt->SetType(&MEDIATYPE_Video);
	pmt->SetFormatType(&FORMAT_VideoInfo);
	pmt->SetVariableSize();

	// Work out the GUID for the subtype from the header info.
	const GUID SubTypeGUID = type;
	pmt->SetSubtype(&SubTypeGUID);

	pvi->AvgTimePerFrame = m_pFilter->m_pSrv->AvgTimePerFrame;
	return NOERROR;
}

HRESULT CESSourcePin::CheckMediaType(const CMediaType *pMediaType)
{
	CheckPointer(pMediaType,E_POINTER);

	// we only output video
	if ( *(pMediaType->Type()) != MEDIATYPE_Video )
	{                                                  
		return E_INVALIDARG;
	}

	// Check for the subtypes we support
	const GUID *SubType = pMediaType->Subtype();
	if ( SubType == NULL )
		return E_INVALIDARG;

	if ( *SubType != m_pFilter->m_pSrv->videoType )
	{
		return E_INVALIDARG;
	}

	// Get the format area of the media type
	VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *) pMediaType->Format();
	if( pvi == NULL )
		return E_INVALIDARG;

	return S_OK;  // This format is acceptable.
} // CheckMediaType

HRESULT CESSourcePin::OnThreadCreate(void)
{
	m_rtStart = 10000*1000;	// 1 second
	return S_OK;
}

HRESULT CESSourcePin::FillBuffer(IMediaSample *pms)
{
	DWORD length = 0;
	DWORD dwRead = 0;

	BOOL tf = ReadFile(m_pFilter->m_hFile, &length, 4, &dwRead, 0);
	if( dwRead == 4 && tf == TRUE )
	{
		BYTE *pData;
		long actual_len = 0;
		pms->GetPointer(&pData);
		long lDataLen = pms->GetSize();

		ZeroMemory(pData, lDataLen);
		tf = ReadFile(m_pFilter->m_hFile, pData, length, &dwRead, 0);
		if( dwRead != length || tf != TRUE )
			return S_FALSE;

		actual_len += dwRead;

		pms->SetSyncPoint(TRUE);

		REFERENCE_TIME tSampleTime = m_pFilter->m_pSrv->AvgTimePerFrame + m_rtStart;
		pms->SetTime((REFERENCE_TIME *) &m_rtStart,(REFERENCE_TIME *) &tSampleTime);
		pms->SetActualDataLength(actual_len);
		m_rtStart = tSampleTime;
	}
	else
	{
		return S_FALSE;
	}

	return S_OK;
}

STDMETHODIMP CESSourcePin::Notify(IBaseFilter * pSender, Quality q)
{
    // Adjust the repeat rate.
    if(q.Proportion<=0)
    {
        //m_iRepeatTime = 1000;        // We don't go slower than 1 per second
    }
    else
    {
	}

    // skip forwards
   // if(q.Late > 0)
    //    m_rtSampleTime += q.Late;

    return NOERROR;
}
