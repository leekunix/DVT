#include <streams.h>
#include "IAMGB.h"
#include <atlbase.h>
#include <atlstr.h>

CGraphBuilderCB::CGraphBuilderCB(void) : CUnknown(NAME("GraphBuilderCallback"), NULL)
{
	AddRef();
}

STDMETHODIMP CGraphBuilderCB::NonDelegatingQueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IAMGraphBuilderCallback))
		return GetInterface((IAMGraphBuilderCallback*)this, ppv);
	
	return CUnknown::NonDelegatingQueryInterface(iid, ppv);
}

STDMETHODIMP CGraphBuilderCB::SelectedFilter(IMoniker *moniker)
{
	HRESULT hr = S_OK;

	// Get the friendly name of the filter
	CComPtr<IPropertyBag> property_bag;
	hr = moniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&property_bag);
	if (FAILED(hr))
		return E_FAIL;

	VARIANT varName;
	VariantInit(&varName);
	hr = property_bag->Read(L"FriendlyName", &varName, 0);
	CString friendly_name;
	if (SUCCEEDED(hr) && varName.bstrVal)
		friendly_name = varName.bstrVal;
	else
		friendly_name = _T("Unknown");
	VariantClear(&varName);

	int ret = 0;
	ret = friendly_name.CompareNoCase(_T("Microsoft DTV-DVD Audio Decoder"));
	if( ret == 0 )
		return E_FAIL;

	ret = friendly_name.CompareNoCase(_T("Microsoft DTV-DVD Video Decoder"));
	if( ret == 0 )
		return E_FAIL;

	ret = friendly_name.CompareNoCase(_T("Microsoft MPEG-1/DD Audio Decoder"));
	if( ret == 0 )
		return E_FAIL;
	
	ret = friendly_name.CompareNoCase(_T("ACM Wrapper"));
	if( ret == 0 )
	{
		ret++;
	}

	ret = friendly_name.Find(_T("DMO"));
	if( ret != -1 )
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CGraphBuilderCB::CreatedFilter(IBaseFilter *pFil)
{
	return S_OK;
}

