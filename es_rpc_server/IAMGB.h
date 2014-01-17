#pragma once

// http://blog.naver.com/PostView.nhn?blogId=tmplover&logNo=10075386703

class CGraphBuilderCB : public IAMGraphBuilderCallback, public CUnknown
{
public:
     CGraphBuilderCB(void);

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID, void**);

	HRESULT STDMETHODCALLTYPE SelectedFilter( /* [in] */ IMoniker *pMon);
	HRESULT STDMETHODCALLTYPE CreatedFilter( /* [in] */ IBaseFilter *pFil);
};
