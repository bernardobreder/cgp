
#include "UI.h"


UIComponent::UIComponent()
{
	static int staticCodeWnd = 256;
	this->codeWnd = staticCodeWnd++;
	this->x = 0;
	this->y = 0;
	this->width = 1;
	this->height = 1;
	this->hWnd = nullptr;
}

UIComponent::~UIComponent()
{
	DestroyWindow(hWnd);
	hWnd = nullptr;
}

UIComponent* UIComponent::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	if (hWnd) {
		SetWindowPos(hWnd, NULL, x, y, width, height, SWP_DRAWFRAME);
	}
	return this;
}

UIComponent* UIComponent::setSize(int width, int height)
{
	this->width = width;
	this->height = height;
	if (hWnd) {
		SetWindowPos(hWnd, NULL, x, y, width, height, SWP_DRAWFRAME);
	}
	return this;
}

UIComponent* UIComponent::setRect(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	if (hWnd) {
		SetWindowPos(hWnd, NULL, x, y, width, height, SWP_DRAWFRAME);
	}
	return this;
}

UIComponent* UIComponent::setParentHWnd(HWND hParentWnd)
{
	return this;
}

UIWindow* UIComponent::getWindow() 
{
	HWND auxHwnd, winHwnd = hWnd;
	while (auxHwnd = GetParent(winHwnd)) winHwnd = auxHwnd;
	return (UIWindow*)GetWindowLongPtr(winHwnd, GWLP_USERDATA);
}

UIComponent* UIComponent::layout()
{
	return this;
}

void UIComponent::messageMouseClick(int x, int y, UIMouseButton button)
{

}

UIComponent* UIComponent::findByCode(int codeWnd)
{
	return this->codeWnd == codeWnd ? this : nullptr;
}

UIComponent* UIComponent::findByPoint(int x, int y)
{
	return this;
}