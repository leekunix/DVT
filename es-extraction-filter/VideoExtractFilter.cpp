#include "stdafx.h"

#include <streams.h>
#include <initguid.h>
#include <stdio.h>

#include "es-extraction-filter.h"
#include "VideoExtractFilter.h"
#include "moreuuids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// {AA59EF9C-A21A-44f9-97DF-11F4BF71DC89}
DEFINE_GUID(CLSID_VideoExtract, 
	0xaa59ef9c, 0xa21a, 0x44f9, 0x97, 0xdf, 0x11, 0xf4, 0xbf, 0x71, 0xdc, 0x89);

CVideoExtract::CVideoExtract(LPUNKNOWN pUnk,
                         HRESULT *phr) :
	CBaseRenderer(CLSID_VideoExtract, NAME("CVideoExtracFilter"), pUnk, phr)
{
	m_hFile = INVALID_HANDLE_VALUE;
	
	m_nVideoType = VIDEO_MAX;
	m_nDSI_Length = 0;
	m_bWriteSeq = false;
	m_idx = 0;
	m_PrevIdx = 0;
	current_segment_st = -1;
	pRpc = 0;
	m_dsi_info = 0;
	m_nDSI_Length = 0;
	m_nSyncFrameCount = 0;
	num_intra_frm = 0;
}

CVideoExtract::~CVideoExtract()
{
	if( m_dsi_info != 0 )
		delete [] m_dsi_info;

	if( m_hFile != INVALID_HANDLE_VALUE )
		CloseHandle(m_hFile);
}

HRESULT CVideoExtract::CheckMediaType(const CMediaType* pmtIn)
{
	if( pmtIn->majortype == MEDIATYPE_Video )
	{
		if( pmtIn->subtype == MEDIASUBTYPE_XVID ||
			pmtIn->subtype == MEDIASUBTYPE_xvid
			)
		{
			ULONG size1 = sizeof(MPEG2VIDEOINFO);
			ULONG size2 = sizeof(VIDEOINFOHEADER2);
			ULONG size3 = sizeof(VIDEOINFOHEADER);

			if( (pmtIn->formattype == FORMAT_MPEG2_VIDEO) &&
				(pmtIn->cbFormat >= size1 ) )
			{
				MPEG2VIDEOINFO *pv = (MPEG2VIDEOINFO*)pmtIn->pbFormat;
				DbgLog((LOG_TRACE, 1, TEXT("AvgTimePerFrame : %lld\r\n"),
					pv->hdr.AvgTimePerFrame));
			}			
			else if( (pmtIn->formattype == FORMAT_VideoInfo2) &&
					(pmtIn->cbFormat >= size2 ) )
			{
				VIDEOINFOHEADER2 *pv = (VIDEOINFOHEADER2*)pmtIn->pbFormat;
				DbgLog((LOG_TRACE, 1, TEXT("AvgTimePerFrame : %lld\r\n"),
					pv->AvgTimePerFrame));
			}
			else if( (pmtIn->formattype == FORMAT_VideoInfo) &&
					(pmtIn->cbFormat >= size3 ) )
			{
				VIDEOINFOHEADER *pv = (VIDEOINFOHEADER*)pmtIn->pbFormat;
				pv->bmiHeader.biSize;
				DbgLog((LOG_TRACE, 1, TEXT("AvgTimePerFrame : %lld\r\n"),
					pv->AvgTimePerFrame));
			}
			m_nVideoType = VIDEO_XVID;
			return S_OK;
		}
		else if( pmtIn->subtype == MEDIASUBTYPE_DIV3 )
		{
			m_nVideoType = VIDEO_DIV3;
			return S_OK;
		}
		else if( pmtIn->subtype == MEDIASUBTYPE_DIVX ||
				pmtIn->subtype == MEDIASUBTYPE_divx
			)
		{
			m_nVideoType = VIDEO_DIVX;
			return S_OK;
		}
		else if( pmtIn->subtype == MEDIASUBTYPE_MP43 ||
				pmtIn->subtype == MEDIASUBTYPE_mp43 )
		{
			m_nVideoType = VIDEO_MP43;
			ULONG size1 = sizeof(MPEG2VIDEOINFO);
			ULONG size2 = sizeof(VIDEOINFOHEADER2);
			ULONG size3 = sizeof(VIDEOINFOHEADER);
			int AvgTimePerFrame = 0;
			if( (pmtIn->formattype == FORMAT_MPEG2_VIDEO) &&
				(pmtIn->cbFormat >= size1 ) &&
				( pmtIn->pbFormat != 0) )
			{
				MPEG2VIDEOINFO *pv = (MPEG2VIDEOINFO*)pmtIn->pbFormat;
				AvgTimePerFrame = pv->hdr.AvgTimePerFrame;
			}
			else if (	( pmtIn->formattype == FORMAT_VideoInfo ) &&
						( pmtIn->cbFormat >= size3 ) &&
						( pmtIn->pbFormat != 0) )
			{
				VIDEOINFOHEADER *pv = (VIDEOINFOHEADER*)pmtIn->pbFormat;
				AvgTimePerFrame = pv->AvgTimePerFrame;
			}
			else if (	( pmtIn->formattype == FORMAT_VideoInfo2 ) &&
						(pmtIn->cbFormat >= size2 ) &&
						( pmtIn->pbFormat != 0) )
			{
				VIDEOINFOHEADER2 *pv = (VIDEOINFOHEADER2*)pmtIn->pbFormat;
				AvgTimePerFrame = pv->AvgTimePerFrame;
			}
			return S_OK;
		}
		else if( pmtIn->subtype == MEDIASUBTYPE_AVC1 ||
				pmtIn->subtype == MEDIASUBTYPE_avc1 ||
				pmtIn->subtype == MEDIASUBTYPE_H264 ||
				pmtIn->subtype == MEDIASUBTYPE_h264 
			)
		{
			ULONG size1 = sizeof(MPEG2VIDEOINFO);
			ULONG size2 = sizeof(VIDEOINFOHEADER2);
			ULONG size3 = sizeof(VIDEOINFOHEADER);
			int AvgTimePerFrame = 0;

			if( (pmtIn->formattype == FORMAT_MPEG2_VIDEO) &&
				(pmtIn->cbFormat >= size1 ) &&
				( pmtIn->pbFormat != 0) )
			{
				MPEG2VIDEOINFO *pv = (MPEG2VIDEOINFO*)pmtIn->pbFormat;
				PBYTE seq = (PBYTE)&pv->dwSequenceHeader[0];
				if( *(DWORD*)seq != 0x01000000 )	// mp4 case
				{
					/*
					m_dsi_info = new BYTE[1024*512];
					int len = (seq[0] << 8) | seq[1];
					int i, r_off, w_off = 4;
					while( seq[2] != 0x67 )
					{
						seq += len;
						len = (seq[0] << 8) | seq[1];
					}
					*(DWORD*)m_dsi_info = 0x01000000;
					for( i = 0 ; i < len ; i++ )
					{
						m_dsi_info[i+w_off] = seq[i+2];
					}

					m_nDSI_Length = 4+len;

					*(DWORD*)(m_dsi_info+len+w_off) = 0x01000000;
					w_off += (i+4);

					len += 2;
					r_off = len;
					len = (seq[len] << 8) | seq[len+1];
					for( i = 0 ; i < len ; i++ )
					{
						m_dsi_info[i+w_off] = seq[r_off+i+2];
					}

					m_nDSI_Length += 4+len;
					*/
				}
				else if( *(DWORD*)seq == 0x01000000 )
				{
					//m_nDSI_Length = pv->cbSequenceHeader;
					//m_dsi_info = new BYTE[1024*512];
					//memcpy(m_dsi_info,seq,m_nDSI_Length);
				}
			}
			else if (	( pmtIn->formattype == FORMAT_VideoInfo ) &&
						( pmtIn->cbFormat >= size3 ) &&
						( pmtIn->pbFormat != 0) )
			{
				VIDEOINFOHEADER *pv = (VIDEOINFOHEADER*)pmtIn->pbFormat;
				AvgTimePerFrame = pv->AvgTimePerFrame;
			}
			else if (	( pmtIn->formattype == FORMAT_VideoInfo2 ) &&
						(pmtIn->cbFormat >= size2 ) &&
						( pmtIn->pbFormat != 0) )
			{
				VIDEOINFOHEADER2 *pv = (VIDEOINFOHEADER2*)pmtIn->pbFormat;
				AvgTimePerFrame = pv->AvgTimePerFrame;
			}

			//m_hFile = CreateFile(_T("es.avc"),
			//					GENERIC_WRITE,
			//					FILE_SHARE_READ,
			//					0,
			//					CREATE_ALWAYS,
			//					FILE_ATTRIBUTE_NORMAL,
			//					0);
			m_nVideoType = VIDEO_H264;
			//m_IsAVC = true;

			//return VFW_E_TYPE_NOT_ACCEPTED;
			return S_OK;
		}
	}
	return E_FAIL;
}

HRESULT CVideoExtract::SetMediaType(const CMediaType *pmt)
{
	avg_segment_length = 0;
	m_duration = 0;
	m_partitions = theApp.m_rpc_server_list.GetCount();

	ULONG size_mpeg2 = sizeof(MPEG2VIDEOINFO);
	ULONG size_vf = sizeof(VIDEOINFOHEADER);
	ULONG size_vf2 = sizeof(VIDEOINFOHEADER2);

	if( (pmt->formattype == FORMAT_MPEG2_VIDEO) &&
		(pmt->cbFormat >= size_mpeg2) )
	{
		MPEG2VIDEOINFO *mp2v = (MPEG2VIDEOINFO*)pmt->pbFormat;
		m_fps = mp2v->hdr.AvgTimePerFrame;
		m_width = mp2v->hdr.bmiHeader.biWidth;
		m_height = mp2v->hdr.bmiHeader.biHeight;

		if( pmt->subtype == MEDIASUBTYPE_AVC1 ||
			pmt->subtype == MEDIASUBTYPE_avc1 ||
			pmt->subtype == MEDIASUBTYPE_H264 ||
			pmt->subtype == MEDIASUBTYPE_h264 )
		{
			PBYTE seq = (PBYTE)&mp2v->dwSequenceHeader[0];
			m_nDSI_Length = mp2v->cbSequenceHeader;
			if( *(DWORD*)seq == 0x01000000 )
			{			
				m_dsi_info = new BYTE[1024*512];
				memcpy(m_dsi_info,seq,m_nDSI_Length);
			}
			else if( *(DWORD*)seq != 0x01000000 )
			{
				m_dsi_info = new BYTE[mp2v->cbSequenceHeader+1024];

				int len = (seq[0] << 8) | seq[1];
				int i, r_off, w_off = 4;
				while( seq[2] != 0x67 )
				{
					seq += len;
					len = (seq[0] << 8) | seq[1];
				}
				*(DWORD*)m_dsi_info = 0x01000000;
				for( i = 0 ; i < len ; i++ )
				{
					m_dsi_info[i+w_off] = seq[i+2];
				}

				m_nDSI_Length = 4+len;

				*(DWORD*)(m_dsi_info+len+w_off) = 0x01000000;
				w_off += (i+4);

				len += 2;
				r_off = len;
				len = (seq[len] << 8) | seq[len+1];
				for( i = 0 ; i < len ; i++ )
				{
					m_dsi_info[i+w_off] = seq[r_off+i+2];
				}

				m_nDSI_Length += 4+len;
			}
		}
	}	
	else if( (pmt->formattype == FORMAT_VideoInfo2) &&
			(pmt->cbFormat >= size_vf2) )
	{
		VIDEOINFOHEADER2 *vh2 = (VIDEOINFOHEADER2*)pmt->pbFormat;
		m_fps = vh2->AvgTimePerFrame;
		m_width = vh2->bmiHeader.biWidth;
		m_height = vh2->bmiHeader.biHeight;
	}
	else if( (pmt->formattype == FORMAT_VideoInfo) &&
				(pmt->cbFormat >= size_vf) )
	{
		VIDEOINFOHEADER *vh = (VIDEOINFOHEADER*)pmt->pbFormat;
		m_fps = vh->AvgTimePerFrame;
		m_width = vh->bmiHeader.biWidth;
		m_height = vh->bmiHeader.biHeight;
	}

	m_subtype = pmt->subtype;
	if( pmt->subtype == MEDIASUBTYPE_AVC1 ||
		pmt->subtype == MEDIASUBTYPE_avc1 ||
		pmt->subtype == MEDIASUBTYPE_H264 ||
		pmt->subtype == MEDIASUBTYPE_h264 )
	{
		m_subtype = MEDIASUBTYPE_H264;
	}

	m_bFirstTimeWrite = true;

    return __super::SetMediaType(pmt);
}

HRESULT CVideoExtract::EndOfStream()
{
	if( pRpc != 0 )
	{
		//DoEncode(pRpc->handleBinding);
		pRpc->EncodeStart();
	}

	return __super::EndOfStream();
}

HRESULT CVideoExtract::DoRenderSample(IMediaSample *pSample)
{
	CheckPointer(pSample,E_POINTER);

	HRESULT hr  = 0;
	CAutoLock lock(&m_ReceiveLock);
	PBYTE pbData = 0;

	// Has the filter been stopped yet?
	//if (m_hFile == INVALID_HANDLE_VALUE)
	//{
	//	return NOERROR;
	//}

	char dbg_str[256];

	hr = pSample->IsPreroll();
	if ( S_OK == hr )
	{
		return hr;
	}

	hr = pSample->IsSyncPoint();
	int bKeyFrame = hr == S_OK ? 1 : 0;

	LONG dwToWrite = pSample->GetActualDataLength();
	hr = pSample->GetPointer(&pbData);
	if (FAILED(hr))
	{
		return hr;
	}

	if( bKeyFrame == 1 )
	{
		m_nSyncFrameCount++;
		//if( m_nSyncFrameCount < 100 ||  m_nSyncFrameCount > 200 )
		//{
		//	//sprintf_s((char*)dbg_str,256,"m_nSyncFrameCount=%d\n",m_nSyncFrameCount);
		//	//OutputDebugStringA((LPCSTR)dbg_str);

		//	if( pRpc != 0 )
		//	{
		//		//DoEncode(pRpc->handleBinding);
		//		pRpc->EncodeStart();
		//		pRpc = 0;
		//	}
		//	return S_OK;
		//}
	}

	REFERENCE_TIME tStart, tStop;
	pSample->GetTime(&tStart, &tStop);

	DWORD dwWritten = 0;
	DWORD	s_code = 0x01000000;
	if( m_nVideoType == VIDEO_H264 )
	{
		int nal_len = 0;
		int nal_offset = 0;

		int start_code = (pbData[0] << 24) | (pbData[1] << 16) 
						| (pbData[2] << 8) | pbData[3];

		if( start_code != 1 )
		{
			// nal length
			do
			{
				nal_len = (pbData[nal_offset+0] << 24) | (pbData[nal_offset+1] << 16) 
							| (pbData[nal_offset+2] << 8) | pbData[nal_offset+3];
				nal_len += 4;

				*(DWORD*)(pbData+nal_offset) = s_code;

				nal_offset += nal_len;
			}while( dwToWrite > nal_offset );
		}
	}

	//sprintf_s((char*)dbg_str,256,"%lld(ms), Bytes(%u)\n",
	//			p->m_llStartTime,
	//			dwToWrite);
	//OutputDebugStringA((LPCSTR)dbg_str);

	if( bKeyFrame == 1 )
		num_intra_frm++;
	
	if ( current_segment_st == -1 && bKeyFrame == 1 )
	{
		num_intra_frm = 0;
		m_idx = m_PrevIdx = 0;
		current_segment_st = tStart;

		POSITION pos = 0;
		pos = theApp.m_rpc_server_list.FindIndex(m_idx);
		pRpc = (CRPCThread*)theApp.m_rpc_server_list.GetAt(pos);
		pRpc->m_fps = m_fps;

		//SetVideoType(pRpc->handleBinding,
		SetVideoType(pRpc->m_ctx,
						(VIDEO_MEDIA_TYPE*)&m_subtype,
						m_fps,
						m_width,
						m_height);
		pRpc->OpenRemoteFile(m_idx);
		if( m_nDSI_Length > 0 )
		{
			m_bFirstTimeWrite = true;
			//SetPicturesData(pRpc->handleBinding, m_dsi_info, m_nDSI_Length);
		}
	}		

	INT_PTR cnt = theApp.m_rpc_server_list.GetCount();
	if( (cnt > 0) && (current_segment_st != -1) )
	{
		bool bOpenFile=false;
		POSITION pos = 0;

		if (bKeyFrame == 1)
		{
			current_segment_length = (tStart - current_segment_st)/10000;	// ms unit

			sprintf_s((char*)dbg_str,256,"%lld, num_intra_frm=%d\n",
						current_segment_length, num_intra_frm);
			OutputDebugStringA((LPCSTR)dbg_str);

			if ( current_segment_length > avg_segment_length )
			{
				pos = theApp.m_rpc_server_list.FindIndex(m_idx+1);
				if( pos != 0 )
				{
					bOpenFile = true;
					m_idx++;
				}

				current_segment_st = tStart;
			}
		}

		if( bOpenFile == true )
		{
			pos = theApp.m_rpc_server_list.FindIndex(m_idx);
			if( m_PrevIdx != m_idx )
			{
				if( pRpc != 0 )
				{
					//DoEncode(pRpc->handleBinding);
					pRpc->EncodeStart();
				}

				pRpc = (CRPCThread*)theApp.m_rpc_server_list.GetAt(pos);
				pRpc->m_fps = m_fps;
				//SetVideoType(pRpc->handleBinding,
				SetVideoType(pRpc->m_ctx,
								(VIDEO_MEDIA_TYPE*)&m_subtype,
								m_fps,
								m_width,
								m_height);
				pRpc->OpenRemoteFile(m_idx);
				if( m_nDSI_Length > 0 )
				{
					m_bFirstTimeWrite = true;
					//SetPicturesData(pRpc->handleBinding, m_dsi_info, m_nDSI_Length);
				}

				m_PrevIdx = m_idx;
			}
		}

		if( pRpc != 0 )
		{
			SetPictureInfo(
						///* [in] */ pRpc->handleBinding,
						pRpc->m_ctx,
						/* [in] */ tStart,
						/* [in] */ tStop,
						/* [in] */ dwToWrite,
						/* [in] */ bKeyFrame);
		}

		if( m_bFirstTimeWrite == true )
		{
			m_bFirstTimeWrite = false;
			PBYTE pNals = new BYTE[m_nDSI_Length + dwToWrite];
			memcpy(pNals, m_dsi_info, m_nDSI_Length);
			memcpy(pNals+m_nDSI_Length, pbData, dwToWrite);
			
			//SetPicturesData(pRpc->handleBinding, pNals, m_nDSI_Length+dwToWrite);
			SetPicturesData(pRpc->m_ctx, pNals, m_nDSI_Length+dwToWrite);
			delete [] pNals;
		}
		else
		{
			if( pRpc != 0 )
			{
				//SetPicturesData(pRpc->handleBinding, pbData, dwToWrite);
				SetPicturesData(pRpc->m_ctx, pbData, dwToWrite);
			}
		}
	}

	theApp.m_cs.Lock();

	if( theApp.m_dwCurrentMax < dwWritten )
		theApp.m_dwCurrentMax = dwWritten;

	TNalInfo *p = 0;
	p = new TNalInfo(dwToWrite);
	p->m_llStartTime = tStart;
	theApp.m_list.AddTail(p);
	theApp.m_cs.Unlock();

	return S_OK;
}
