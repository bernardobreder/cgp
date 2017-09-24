#pragma once

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include <Windows.h>

class UIComponent;
class UIWindow;
class UITextField;

struct UISize {

	int x;

	int y;

	int width;

	int height;

};

struct UIEdge {

	int left;

	int right;

	int top;

	int bottom;

};

enum UIMouseButton {
	UIMouseButtonLeft, UIMouseButtonRight, UIMouseButtonCenter
};

class UIWindow
{

protected:

	HWND hWnd;

	bool visible;
	
	int x;

	int y;

	int width;

	int height;

	struct UIEdge borderMargin;

	UIComponent* component;

public:
	UIWindow();

	virtual ~UIWindow();

	virtual void setTitle(wchar_t* title);

	virtual UIWindow* setComponent(UIComponent* component);

	virtual void messagePaint();

	virtual void messageKeyPress(int charCode);

	virtual void messageKeyRelease(int charCode);

	virtual void messageMouseMove(int x, int y);

	virtual void messageMouseDown(int x, int y, UIMouseButton button);

	virtual void messageMouseWheel(int x, int y, float dx, float dy);

	virtual void messageMouseUp(int x, int y, UIMouseButton button);

	virtual void messageMouseDblClick(int x, int y, UIMouseButton button);

	virtual void messageWindowOpen();

	virtual void messageWindowResize(int width, int height);

	virtual void messageWindowClose();

	virtual UIWindow* setVisible(bool flag);

	virtual bool isVisiable();

	virtual void eventLoop();

	virtual UIWindow* setPositionToCenter();

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class UIComponent
{

protected:

public:

	HWND hWnd;

	int codeWnd;

	int x;

	int y;

	int width;

	int height;

	UIComponent();

	virtual ~UIComponent();

	virtual UIComponent* setPosition(int x, int y);

	virtual UIComponent* setSize(int width, int height);

	virtual UIComponent* setRect(int x, int y, int width, int height);

	virtual UIComponent* setParentHWnd(HWND hParentWnd);

	virtual UIWindow* getWindow();

	virtual UIComponent* layout();

	virtual void messageMouseClick(int x, int y, UIMouseButton button);

	virtual UIComponent* findByPoint(int x, int y);

	virtual UIComponent* findByCode(int codeWnd);

};

class UILabel : public UIComponent
{

	wchar_t* text;

public:

	UILabel(wchar_t* text);

	virtual ~UILabel();

	virtual UIComponent* setParentHWnd(HWND hParentWnd);

	virtual UILabel* setText(const wchar_t* text);

};

class UIButton : public UIComponent
{

	wchar_t* text;

public:

	UIButton(wchar_t* text);

	virtual ~UIButton();

	virtual UIComponent* setParentHWnd(HWND hParentWnd);

	virtual UIButton* setText(const wchar_t* text);

};

class UITextField : public UIComponent
{

public:

	UITextField();

	virtual ~UITextField();

	virtual UIComponent* setParentHWnd(HWND hParentWnd);

};

class UIRichText : public UIComponent
{

public:

	UIRichText();

	virtual ~UIRichText();

	virtual UIComponent* setParentHWnd(HWND hParentWnd);

};

class UILayout : public UIComponent
{

public:

	UILayout();

	virtual ~UILayout();

};

class UIBorderLayout : public UILayout
{

	UIComponent* center;

	UIComponent* left;

	UIComponent* right;

	UIComponent* top;

	UIComponent* bottom;

	int margin;

public:

	UIBorderLayout();

	virtual ~UIBorderLayout();

	virtual UIComponent* setParentHWnd(HWND hParentWnd);

	virtual UIBorderLayout* setCenterComponent(UIComponent* component);

	virtual UIBorderLayout* setLeftComponent(UIComponent* component);

	virtual UIBorderLayout* setRightComponent(UIComponent* component);

	virtual UIBorderLayout* setTopComponent(UIComponent* component);

	virtual UIBorderLayout* setBottomComponent(UIComponent* component);

	virtual UIBorderLayout* setMargin(int margin);

	virtual int getMargin();

	virtual UIComponent* layout();

	virtual void messageMouseClick(int x, int y, UIMouseButton button);

	virtual UIComponent* findByPoint(int x, int y);

	virtual UIComponent* findByCode(int codeWnd);

};

