#pragma once
#include <ShellAPI.h>

struct IDropListener
{
	virtual void OnDrop(LPCTSTR pszName) =0;
};

class CDropTarget :public IDropTarget
{
	IDropListener *m_pListener;
public:
	CDropTarget(IDropListener *pListener):m_pListener(pListener)
	{
		nRef = 1;
	}

	virtual ~CDropTarget() {}

	//////////////////////////////////////////////////////////////////////////
	// IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */  void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		HRESULT hr = S_FALSE;
		if (riid == __uuidof(IUnknown))
			*ppvObject = (IUnknown*)this, hr = S_OK;
		else if (riid == __uuidof(IDropTarget))
			*ppvObject = (IDropTarget*)this, hr = S_OK;
		if (SUCCEEDED(hr)) AddRef();
		return hr;

	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void) { return ++nRef; }

	virtual ULONG STDMETHODCALLTYPE Release(void) {
		ULONG uRet = --nRef;
		if (uRet == 0) delete this;
		return uRet;
	}

	//////////////////////////////////////////////////////////////////////////
	// IDropTarget

	virtual HRESULT STDMETHODCALLTYPE DragEnter(
		/* [unique][in] */  IDataObject *pDataObj,
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */  DWORD *pdwEffect)
	{
		*pdwEffect = DROPEFFECT_LINK;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DragOver(
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */  DWORD *pdwEffect)
	{
		*pdwEffect = DROPEFFECT_LINK;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE DragLeave(void)
	{
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Drop(
		/* [unique][in] */  IDataObject *pDataObj,
		/* [in] */ DWORD grfKeyState,
		/* [in] */ POINTL pt,
		/* [out][in] */  DWORD *pdwEffect)
	{
		FORMATETC format =
		{
			CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL
		};
		STGMEDIUM medium;
		if (FAILED(pDataObj->GetData(&format, &medium)))
		{
			return S_FALSE;
		}

		HDROP hdrop = static_cast<HDROP>(GlobalLock(medium.hGlobal));

		if (!hdrop)
		{
			return S_FALSE;
		}

		bool success = false;
		TCHAR filename[MAX_PATH];
		success = !!DragQueryFile(hdrop, 0, filename, MAX_PATH);
		DragFinish(hdrop);
		GlobalUnlock(medium.hGlobal);

		if (success && m_pListener)
		{
			m_pListener->OnDrop(filename);
		}

		*pdwEffect = DROPEFFECT_LINK;
		return S_OK;
	}
protected:
	int nRef;
};
