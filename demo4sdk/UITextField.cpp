
#include "UI.h"
#include <Windows.h>
#include <ShObjIdl.h>

UITextField::UITextField() : UIComponent()
{
	
}


UITextField::~UITextField() 
{
	UIComponent::~UIComponent();
}

UIComponent* UITextField::setParentHWnd(HWND hParentWnd)
{
	UIComponent::setParentHWnd(hWnd);
	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y, width, height, hParentWnd, (HMENU)codeWnd, GetModuleHandle(NULL), NULL);
	HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	SetWindowText(hWnd, L"TESDTE");

	return this;
}