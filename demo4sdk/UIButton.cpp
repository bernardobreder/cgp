
#include "UI.h"
#include <Windows.h>
#include <ShObjIdl.h>
#include <wchar.h>

UIButton::UIButton(wchar_t* text) : UIComponent()
{
	this->text = _wcsdup(text);
}

UIButton::~UIButton()
{
	UIComponent::~UIComponent();
	delete text;
}

UIComponent* UIButton::setParentHWnd(HWND hParentWnd)
{
	UIComponent::setParentHWnd(hWnd);
	hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, L"BUTTON", text,
		WS_VISIBLE | WS_CHILD,
		x, y, width, height, hParentWnd, (HMENU)codeWnd, GetModuleHandle(NULL), NULL);
	HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	return this;
}

UIButton* UIButton::setText(const wchar_t* text) {
	delete this->text;
	this->text = _wcsdup(text);
	if (hWnd) {
		SetWindowText(hWnd, this->text);
	}
	return this;
}