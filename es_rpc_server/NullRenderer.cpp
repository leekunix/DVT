#include "stdafx.h"
#include <streams.h>
#include <dvdmedia.h>
#include <initguid.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "NullRenderer.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

CNullVideoRenderer::CNullVideoRenderer(LPUNKNOWN pUnk, HRESULT* phr) 
	: CBaseRenderer(__uuidof(this), NAME("Video Capture Renderer"), pUnk, phr)
{
	char path[MAX_PATH+2];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(0, path, MAX_PATH);
	_splitpath_s( path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );
	sprintf_s(path,MAX_PATH,"%s%s_target_.yuv",drive,dir);
	//sprintf_s(path,MAX_PATH,"_target_.yuv");

	m_Targetfile = CreateFile(
								//NAME("_target_.yuv"),
								path,
								GENERIC_WRITE,
								0,
								0,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								0);
	m_fmr_cnt = 0;
}

CNullVideoRenderer::~CNullVideoRenderer()
{
	if( INVALID_HANDLE_VALUE != m_Targetfile )
		CloseHandle(m_Targetfile);

	m_Targetfile = INVALID_HANDLE_VALUE;
}

HRESULT CNullVideoRenderer::SetMediaType(const CMediaType *pmtIn)
{
	REFERENCE_TIME  AvgTimePerFrame = 0;
	ULONG size = sizeof(VIDEOINFOHEADER);
	ULONG size2 = sizeof(VIDEOINFOHEADER2);
	ULONG size_mpeg2 = sizeof(MPEG2VIDEOINFO);

	if( pmtIn->subtype == MEDIASUBTYPE_YV12 )
	{
		if( (pmtIn->formattype == FORMAT_VideoInfo) &&
			(pmtIn->cbFormat >= sizeof(VIDEOINFOHEADER) ) &&
			( pmtIn->pbFormat != 0) )
		{
			VIDEOINFOHEADER	*pInput = (VIDEOINFOHEADER*)pmtIn->Format();
			AvgTimePerFrame = pInput->AvgTimePerFrame;
			bmiHeader = pInput->bmiHeader;
		}
		else if( (pmtIn->formattype == FORMAT_MPEG2_VIDEO) &&
					(pmtIn->cbFormat >= size_mpeg2) &&
					( pmtIn->pbFormat != 0) )
		{
			MPEG2VIDEOINFO *mp2v = (MPEG2VIDEOINFO*)pmtIn->pbFormat;
			bmiHeader = mp2v->hdr.bmiHeader;
		}
		else if( (pmtIn->formattype == FORMAT_VideoInfo2) &&
				(pmtIn->cbFormat >= sizeof(VIDEOINFOHEADER2) ) &&
				( pmtIn->pbFormat != 0) )
		{
			VIDEOINFOHEADER2 *pInput = (VIDEOINFOHEADER2*)pmtIn->Format();
			AvgTimePerFrame = pInput->AvgTimePerFrame;
			bmiHeader = pInput->bmiHeader;
		}

		return S_OK;
	} // if( pmtIn->subtype == MEDIASUBTYPE_YV12 )

	return E_FAIL;
}

HRESULT CNullVideoRenderer::CheckMediaType(const CMediaType* pmtIn)
{
	REFERENCE_TIME  AvgTimePerFrame = 0;
	if( pmtIn->majortype == MEDIATYPE_Video )
	{
		ULONG size = sizeof(VIDEOINFOHEADER);
		ULONG size2 = sizeof(VIDEOINFOHEADER2);
		ULONG size_mpeg2 = sizeof(MPEG2VIDEOINFO);

		if( pmtIn->subtype == MEDIASUBTYPE_YV12 )
		{
			if( (pmtIn->formattype == FORMAT_VideoInfo) &&
				(pmtIn->cbFormat >= sizeof(VIDEOINFOHEADER) ) &&
				( pmtIn->pbFormat != 0) )
			{
				VIDEOINFOHEADER	*pInput = (VIDEOINFOHEADER*)pmtIn->Format();
				AvgTimePerFrame = pInput->AvgTimePerFrame;
				bmiHeader = pInput->bmiHeader;
			}
			else if( (pmtIn->formattype == FORMAT_MPEG2_VIDEO) &&
						(pmtIn->cbFormat >= size_mpeg2) &&
						( pmtIn->pbFormat != 0) )
			{
				MPEG2VIDEOINFO *mp2v = (MPEG2VIDEOINFO*)pmtIn->pbFormat;
				bmiHeader = mp2v->hdr.bmiHeader;
			}
			else if( (pmtIn->formattype == FORMAT_VideoInfo2) &&
					(pmtIn->cbFormat >= sizeof(VIDEOINFOHEADER2) ) &&
					( pmtIn->pbFormat != 0) )
			{
				VIDEOINFOHEADER2 *pInput = (VIDEOINFOHEADER2*)pmtIn->Format();
				AvgTimePerFrame = pInput->AvgTimePerFrame;
				bmiHeader = pInput->bmiHeader;
			}
			return S_OK;
		} // if( pmtIn->subtype == MEDIASUBTYPE_YV12 )
	}

	// 8bit : dst->biSizeImage	= ((w+3) & ~3) * h;

	return E_FAIL;
}

HRESULT CNullVideoRenderer::DoRenderSample(IMediaSample* pSample)
{
	HRESULT hr = 0;

	REFERENCE_TIME st, et;
	long len = pSample->GetActualDataLength();
	PBYTE pdata = 0;
	
	pSample->GetPointer(&pdata);

	hr = pSample->GetTime(&st,&et);
	if( hr != S_OK )
	{
		return S_OK;
	}

	DWORD dwWritten = 0;
	if( m_Targetfile != INVALID_HANDLE_VALUE && m_fmr_cnt < 500 )
	//if( m_Targetfile != INVALID_HANDLE_VALUE )
	{
		int luma = bmiHeader.biWidth * bmiHeader.biHeight;
		int u_size = luma/4;

		PBYTE u_data = pdata + luma + u_size;
		PBYTE v_data = pdata + luma;
		
		WriteFile(m_Targetfile,pdata,luma,&dwWritten,0);
		WriteFile(m_Targetfile,u_data,u_size,&dwWritten,0);
		WriteFile(m_Targetfile,v_data,u_size,&dwWritten,0);

		m_fmr_cnt++;
	}

	return S_OK;
}
