
#include "UI.h"
#include <Windows.h>
#include <ShObjIdl.h>
#include <richedit.h>	

UIRichText::UIRichText() : UIComponent()
{
	
}

UIRichText::~UIRichText() 
{
	UIComponent::~UIComponent();
}

UIComponent* UIRichText::setParentHWnd(HWND hParentWnd)
{
	UIComponent::setParentHWnd(hWnd);
	LoadLibrary(TEXT("Msftedit.dll"));
	hWnd = CreateWindow(L"RICHEDIT50W", TEXT(""),
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		x, y, width, height,
		hParentWnd, (HMENU)codeWnd, NULL, NULL);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
	HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	SetWindowText(hWnd, L"Eaçãoscreva aqui...");

	CHARRANGE m_crRE50W;
	m_crRE50W.cpMin = 1;
	m_crRE50W.cpMax = 5;
	SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&m_crRE50W);

	CHARFORMAT2 m_cfRE50W;
	m_cfRE50W.cbSize = sizeof(CHARFORMAT2);
	m_cfRE50W.dwMask = CFM_COLOR | CFM_BOLD | CFM_CHARSET;
	m_cfRE50W.crTextColor = RGB(255, 0, 0);
	m_cfRE50W.dwEffects = CFE_BOLD;
	m_cfRE50W.bCharSet = DEFAULT_CHARSET;
	SendMessage(hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&m_cfRE50W);

	m_crRE50W.cpMin = -1;
	m_crRE50W.cpMax = -1;
	SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&m_crRE50W);

	return this;
}