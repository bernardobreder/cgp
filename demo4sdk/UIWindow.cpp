
#include "UI.h"

#include "resource.h"
#include <stdexcept>
#include <windowsx.h>
#include <algorithm>
#include <d3d9.h>
#include <d3dx9tex.h>

//#include <algorithm>
//#include <math.h>
//void abssort(float* x, unsigned int n) {
//	int aa = 5;
//	std::sort(x, x + n, [&aa](float a, float b) {
//		int bb = aa;
//		return std::abs(a) < std::abs(b);
//	});
//}
//
//#pragma comment(lib, "d2d1")

LPDIRECT3D9 pD3D = nullptr;

LPDIRECT3DDEVICE9 pd3dDevice = nullptr;

static size_t windowsOpened = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int count = 1;
	static UIMouseButton setCursorMouseButton;
	static bool setCursorMouseDown = false;

	UIWindow* win = (UIWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (message) {
	case WM_PAINT: {
		static HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, white);
		win->messagePaint();
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_CLOSE: {
		if (MessageBox(hWnd, L"Deseja  fechar?", L"App", MB_OKCANCEL) == IDOK)  {
			DestroyWindow(hWnd);
		}
		return 0;
	}
	case WM_DESTROY: {
		win->messageWindowClose();
		windowsOpened--;
		PostQuitMessage(0);
		break;
	}
	case WM_CREATE: {
		LPCREATESTRUCTW createStruct = (LPCREATESTRUCTW)lParam;
		win = (UIWindow*)createStruct->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)win);
		windowsOpened++;
		return 0;
	}
	case WM_SHOWWINDOW: {
		win->messageWindowOpen();
		return 0;
	}
	case WM_KEYDOWN: {
		win->messageKeyPress((int)wParam);
		return 0;
	}
	case WM_KEYUP: {
		win->messageKeyRelease((int)wParam);
		return 0;
	}
	case WM_SYSKEYDOWN: {
		break;
	}
	case WM_SYSKEYUP: {
		break;
	}
	case WM_CHAR: {
		int wmId, wmEvent;
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		break;
	}
	case WM_COMMAND: {
		int codeWnd = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		if (win->component) {
			UIComponent* child = win->component->findByCode(codeWnd);
			if (child) {
				child->messageMouseClick(0, 0, UIMouseButtonLeft);
			}
		}
		wchar_t msg[1024];
		wsprintf(msg, L"Notify[%d] %d %d", count++, codeWnd, wmEvent);
		win->setTitle(msg);
		break;
	}
	case WM_SYSCHAR: {
		break;
	}
					 //case WM_LBUTTONDOWN: {
					 //	int x = GET_X_LPARAM(lParam);
					 //	int y = GET_Y_LPARAM(lParam);
					 //	UIMouseButton button = wParam & MK_LBUTTON ? UIMouseButtonLeft : UIMouseButtonRight;
					 //	// if (GetKeyState(VK_MENU) & 0x8000);
					 //	win->messageMouseDown(x, y, button);
					 //	break;
					 //}
					 //case WM_LBUTTONUP: {
					 //	int x = GET_X_LPARAM(lParam);
					 //	int y = GET_Y_LPARAM(lParam);
					 //	UIMouseButton button = wParam & MK_LBUTTON ? UIMouseButtonLeft : UIMouseButtonRight;
					 //	// if (GetKeyState(VK_MENU) & 0x8000);
					 //	// if (GetKeyState(VK_RMENU) & 0x8000);

					 //	win->messageMouseUp(x, y, button);
					 //	break;
					 //}
					 //case WM_LBUTTONDBLCLK: {
					 //	int x = GET_X_LPARAM(lParam);
					 //	int y = GET_Y_LPARAM(lParam);
					 //	UIMouseButton button = wParam & MK_LBUTTON ? UIMouseButtonLeft : UIMouseButtonRight;
					 //	win->messageMouseDblClick(x, y, button);
					 //	break;
					 //}
					 //case WM_RBUTTONDOWN: {
					 //	int x = GET_X_LPARAM(lParam);
					 //	int y = GET_Y_LPARAM(lParam);
					 //	UIMouseButton button = UIMouseButtonRight;
					 //	win->messageMouseDown(x, y, button);
					 //	break;
					 //}
					 //case WM_RBUTTONUP: {
					 //	int x = GET_X_LPARAM(lParam);
					 //	int y = GET_Y_LPARAM(lParam);
					 //	UIMouseButton button = UIMouseButtonRight;
					 //	win->messageMouseUp(x, y, button);
					 //	break;
					 //}
	case WM_MOUSEWHEEL: {
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(hWnd, &point);
		int fwKeys = GET_KEYSTATE_WPARAM(wParam);
		bool ctrl = (fwKeys & MK_CONTROL) == MK_CONTROL;
		bool shift = (fwKeys & MK_SHIFT) == MK_SHIFT;
		int delta = -GET_WHEEL_DELTA_WPARAM(wParam);
		win->messageMouseWheel(point.x, point.y, 0.0f, (float)delta);
		break;
	}
	case WM_MOUSEHWHEEL: {
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(hWnd, &point);
		int fwKeys = GET_KEYSTATE_WPARAM(wParam);
		bool ctrl = (fwKeys & MK_CONTROL) == MK_CONTROL;
		bool shift = (fwKeys & MK_SHIFT) == MK_SHIFT;
		int delta = -GET_WHEEL_DELTA_WPARAM(wParam);
		win->messageMouseWheel(point.x, point.y, (float)delta, 0.0f);
		break;
	}
						 /*case WM_MOUSEACTIVATE: {
							 int x = GET_X_LPARAM(lParam);
							 int y = GET_Y_LPARAM(lParam);
							 break;
							 }
							 case WM_MOUSELEAVE: {
							 int x = GET_X_LPARAM(lParam);
							 int y = GET_Y_LPARAM(lParam);
							 break;
							 }*/
						 /*case WM_MOUSEMOVE: {
							 int x = GET_X_LPARAM(lParam);
							 int y = GET_Y_LPARAM(lParam);
							 win->messageMouseMove(x, y);
							 return 0;
							 }*/
	case WM_SETCURSOR: {
		int codeWnd = LOWORD(lParam);
		int wmEvent = HIWORD(lParam);
		int acodeWnd = LOWORD(wParam);
		int awmEvent = HIWORD(wParam);
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(hWnd, &point);
		if (point.x < 0 || point.y < 0) {
			break;
		}
		if (!setCursorMouseDown && LOBYTE(wmEvent) == MK_LBUTTON) {
			setCursorMouseButton = UIMouseButtonLeft;
			setCursorMouseDown = true;
			win->messageMouseDown(point.x, point.y, setCursorMouseButton);
		}
		else if (!setCursorMouseDown && LOBYTE(wmEvent) == 4) {
			setCursorMouseButton = UIMouseButtonRight;
			setCursorMouseDown = true;
			win->messageMouseDown(point.x, point.y, setCursorMouseButton);
		}
		else if (!setCursorMouseDown && LOBYTE(wmEvent) == 7) {
			setCursorMouseButton = UIMouseButtonCenter;
			setCursorMouseDown = true;
			win->messageMouseDown(point.x, point.y, setCursorMouseButton);
		}
		else if (setCursorMouseDown) {
			setCursorMouseDown = false;
			win->messageMouseUp(point.x, point.y, setCursorMouseButton);
		}
		else {
			win->messageMouseMove(point.x, point.y);
		}
		break;
	}
	case WM_SIZE: {
		RECT rect;
		if (GetWindowRect(hWnd, &rect)) {
			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
			win->width = width;
			win->height = height;
			win->messageWindowResize(width, height);
		}
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

UIWindow::UIWindow()
{
	static WNDCLASSEX wcex;
	if (!wcex.lpfnWndProc) {
		HMODULE hInstance = GetModuleHandle(NULL);
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_MYICON));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_DESKTOP + 1);
		wcex.lpszMenuName = 0;
		wcex.lpszClassName = L"GUI";
		RegisterClassEx(&wcex);
	}
	this->component = nullptr;
	this->x = 0;
	this->y = 0;
	this->width = 900;
	this->height = 600;
}

UIWindow::~UIWindow()
{
}

UIWindow* UIWindow::setPositionToCenter() {
	/*HWND hwndParent;
	RECT rect, rectP = { 0, 0, 0, 0 };
	int width, height;
	int screenwidth = GetSystemMetrics(SM_CXSCREEN);
	int screenheight = GetSystemMetrics(SM_CYSCREEN);


	GetWindowRect(hWnd, &rect);
	hwndParent = GetParent(hWnd);
	if (hwndParent) {
	GetWindowRect(hwndParent, &rectP);
	}
	else {
	rectP.right = screenwidth;
	rectP.bottom = screenheight;
	}

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	x = ((rectP.right - rectP.left) - width) / 2 + rectP.left;
	y = ((rectP.bottom - rectP.top) - height) / 2 + rectP.top;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + width > screenwidth)  x = screenwidth - width;
	if (y + height > screenheight) y = screenheight - height;

	MoveWindow(hWnd, x, y, width, height, FALSE);*/
	return this;
}

void UIWindow::setTitle(wchar_t* title)
{
	SetWindowText(hWnd, title);
}

UIWindow* UIWindow::setComponent(UIComponent* component)
{
	if (this->component) {
		delete this->component;
	}
	this->component = component;
	return this;
}

UIWindow* UIWindow::setVisible(bool flag)
{
	visible = flag;
	HWND hwndParent = NULL;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	RECT WindowRect = { 0, 0, width, height };
	AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE, dwExStyle);
	this->borderMargin = { abs(WindowRect.left), WindowRect.right - width, abs(WindowRect.top), WindowRect.bottom - height };
	WindowRect.right += abs(WindowRect.left);
	WindowRect.bottom += abs(WindowRect.top);

	int screenwidth = GetSystemMetrics(SM_CXSCREEN);
	int screenheight = GetSystemMetrics(SM_CYSCREEN);
	if (hwndParent) {
		GetWindowRect(hwndParent, &WindowRect);
	}
	else {
		WindowRect.right = screenwidth;
		WindowRect.bottom = screenheight;
	}
	x = ((WindowRect.right - WindowRect.left) - width) / 2 + WindowRect.left;
	y = ((WindowRect.bottom - WindowRect.top) - height) / 2 + WindowRect.top;
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + width > screenwidth)  x = screenwidth - width;
	if (y + height > screenheight) y = screenheight - height;

	if (!(hWnd = CreateWindowEx(dwExStyle, L"GUI", L"Titulo", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, x, y, width, height, hwndParent, NULL, 0, this))) {
		return this;
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);

	if (!(pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
		MessageBox(hWnd, L"Can not initialize DirectX", L"DirectX Error", MB_OK);
		DestroyWindow(hWnd);
		return this;
	}
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow = hWnd;
	// Create a default DirectX device
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice))) {
		pD3D->Release(); pD3D = nullptr;
		MessageBox(hWnd, L"Can not create a DirectX Device", L"DirectX Error", MB_OK);
		DestroyWindow(hWnd);
		return this;
	}
	if (component) {
		component->setParentHWnd(hWnd);
	}
	SetFocus(hWnd);
	UpdateWindow(hWnd);
	return this;
}

bool UIWindow::isVisiable()
{
	return visible;
}

void UIWindow::eventLoop()
{
	D3DXMATRIX mProjection;
	D3DXMatrixPerspectiveFovLH(&mProjection, D3DXToRadian(45.0f), 1.0f, 1.0f, 100.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &mProjection);

	wchar_t* filename = L"C:\\git\\cglangsdk\\test.bmp";
	D3DXIMAGE_INFO imageInfo;
	if (FAILED(D3DXGetImageInfoFromFile(filename, &imageInfo))) {

	}
	IDirect3DSurface9* surface;
	if (FAILED(pd3dDevice->CreateOffscreenPlainSurface(imageInfo.Width, imageInfo.Height, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &surface, NULL))) {
		surface->Release();
	}
	if (FAILED(D3DXLoadSurfaceFromFile(surface, NULL, NULL, filename, NULL, D3DX_DEFAULT, 0, NULL))) {
		surface->Release();
	}
	
	struct CUSTOMVERTEX
	{
		FLOAT x, y, z, rhw;
		DWORD color;
	};
	CUSTOMVERTEX g_Vertices[] =
	{
		{ 320.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(0, 255, 0, 0), },
		{ 250.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(0, 0, 255, 0), },
		{ 50.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_ARGB(0, 0, 0, 255), },
	};
	LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;
	if (FAILED(pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB, NULL))) {

	}
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0))) {

	}
	memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
	g_pVB->Unlock();
	D3DXMATRIX objMat, matRotate, finalMat;

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.0f),
		640.0f / 480.0f, 0.1f, 100.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIX matView;
	D3DXVECTOR3 cameraPosition, cameraLook;
	D3DXMatrixLookAtLH(&matView,
		&cameraPosition, 
		&cameraLook, 
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	int targetFrameRate = 60;
	DWORD target = (DWORD)(1000 / targetFrameRate);
	LARGE_INTEGER timerFrequency;
	QueryPerformanceFrequency(&timerFrequency);
	MSG msg;
	DWORD starting_point = GetTickCount();
	while (windowsOpened) {
		for (;;) {
			BOOL foundMessage;
			if (foundMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if ((GetTickCount() - starting_point) >= target) break;
			if (!foundMessage) Sleep(1);
		}
		starting_point = GetTickCount();
		
		// Update((float)(GetTickCount() - starting_point) / 1000.0f);

		static int angle = 0;
		D3DXMatrixIdentity(&objMat);
		D3DXMatrixRotationY(&matRotate, D3DXToRadian(45));
		D3DXMatrixMultiply(&finalMat, &objMat, &matRotate);
		pd3dDevice->SetTransform(D3DTS_WORLD, &finalMat);

		pd3dDevice->BeginScene();

		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
		pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		pd3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		pd3dDevice->EndScene();

		/*IDirect3DSurface9* backbuffer = nullptr;
		pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
		pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 0), 1.0f, 0);
		pd3dDevice->StretchRect(surface, NULL, backbuffer, NULL, D3DTEXF_NONE);*/
		pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
	if (surface) surface->Release();
	if (pd3dDevice) pd3dDevice->Release();
	if (pD3D) pD3D->Release();
}

void UIWindow::messagePaint()
{

}

void UIWindow::messageKeyPress(int charCode)
{

}

void UIWindow::messageKeyRelease(int charCode)
{

}

void UIWindow::messageMouseMove(int x, int y)
{

}

void UIWindow::messageMouseDown(int x, int y, UIMouseButton button)
{

}

void UIWindow::messageMouseUp(int x, int y, UIMouseButton button)
{

}

void UIWindow::messageMouseDblClick(int x, int y, UIMouseButton button)
{

}

void UIWindow::messageMouseWheel(int x, int y, float dx, float dy)
{

}

void UIWindow::messageWindowOpen()
{

}

void UIWindow::messageWindowResize(int width, int height)
{
	if (component) {
		width -= borderMargin.left + borderMargin.right;
		height -= borderMargin.top + borderMargin.bottom;
		component->setSize(width, height);
		component->layout();
	}
}

void UIWindow::messageWindowClose()
{

}