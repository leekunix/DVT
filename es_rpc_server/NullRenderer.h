#pragma once

[uuid("23FC5855-1F0D-4b4f-B6C8-9C44303FE0EA")]
class CNullVideoRenderer : public CBaseRenderer
{
protected:
	HRESULT CheckMediaType(const CMediaType* pmtIn);
	HRESULT SetMediaType(const CMediaType *pmtIn);
	virtual HRESULT DoRenderSample(IMediaSample* pSample);

public:

	virtual ~CNullVideoRenderer();

	BITMAPINFOHEADER	bmiHeader;
	HANDLE				m_Targetfile;

	int					m_fmr_cnt;

	CNullVideoRenderer(LPUNKNOWN pUnk, HRESULT* phr);
};
