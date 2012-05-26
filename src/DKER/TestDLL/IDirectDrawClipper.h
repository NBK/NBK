#ifndef IDirectDrawClipper_H
#define IDirectDrawClipper_H

HRESULT WINAPI IDirectDrawClipperHooked_QueryInterface(IDirectDrawClipper* p0, REFIID p1, LPVOID FAR * p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_QueryInterface = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].QueryInterface(p0,p1,p2);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.QueryInterface(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirectDrawClipperHooked_GetClipList(IDirectDrawClipper* p0, LPRECT p1, LPRGNDATA p2, LPDWORD p3)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_GetClipList = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].GetClipList(p0,p1,p2,p3);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.GetClipList(p0,p1,p2,p3);

	return res;
}

HRESULT WINAPI IDirectDrawClipperHooked_GetHWnd(IDirectDrawClipper* p0, HWND FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_GetHWnd = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].GetHWnd(p0,p1);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.GetHWnd(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDrawClipperHooked_Initialize(IDirectDrawClipper* p0, LPDIRECTDRAW p1, DWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_Initialize = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].Initialize(p0,p1,p2);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.Initialize(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirectDrawClipperHooked_IsClipListChanged(IDirectDrawClipper* p0, BOOL FAR * p1)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_IsClipListChanged = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].IsClipListChanged(p0,p1);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.IsClipListChanged(p0,p1);

	return res;
}

HRESULT WINAPI IDirectDrawClipperHooked_SetClipList(IDirectDrawClipper* p0, LPRGNDATA p1, DWORD p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_SetClipList = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].SetClipList(p0,p1,p2);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.SetClipList(p0,p1,p2);

	return res;
}

HRESULT WINAPI IDirectDrawClipperHooked_SetHWnd(IDirectDrawClipper* p0, DWORD p1, HWND p2)
{
	HRESULT res = E_FAIL;

	CLogger::addEntry("Calling IDirectDrawClipperHooked_SetHWnd = "+intToStr((int)p0));

	bool found = (originalIDirectDrawClipperVtbls.find(p0) != originalIDirectDrawClipperVtbls.end());
	if (found)
		res = originalIDirectDrawClipperVtbls[p0].SetHWnd(p0,p1,p2);
	else
		res = originalIDirectDrawClipperVtbls.begin()->second.SetHWnd(p0,p1,p2);

	return res;
}

#endif // IDirectDrawClipper