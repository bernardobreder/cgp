
#include "UI.h"
#include <Windows.h>
#include <ShObjIdl.h>
#include <wchar.h>

UILabel::UILabel(wchar_t* text) : UIComponent()
{
	this->text = _wcsdup(text);
}

UILabel::~UILabel()
{
	UIComponent::~UIComponent();
	delete text;
}

UIComponent* UILabel::setParentHWnd(HWND hParentWnd)
{
	UIComponent::setParentHWnd(hWnd);
	hWnd = CreateWindow(L"STATIC", text,
		WS_VISIBLE | WS_CHILD,
		x, y, width, height, hParentWnd, (HMENU)codeWnd, GetModuleHandle(NULL), NULL);
	HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	return this;
}

UILabel* UILabel::setText(const wchar_t* text) {
	delete this->text;
	this->text = _wcsdup(text);
	if (hWnd) {
		SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)this->text);
	}
	return this;
}