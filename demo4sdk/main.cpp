
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "UI.h"

#include <ShObjIdl.h>

class MyWindow : public UIWindow
{

	virtual void messagePaint() {
		wchar_t msg[1024];
		wsprintf(msg, L"Paint");
		setTitle(msg);
	}

	virtual void messageKeyPress(int charCode) {
		wchar_t msg[1024];
		wsprintf(msg, L"Key Press %d", charCode);
		setTitle(msg);
	}

	virtual void messageKeyRelease(int charCode) {
		wchar_t msg[1024];
		wsprintf(msg, L"Key Release %d", charCode);
		setTitle(msg);
	}

	virtual void messageMouseDown(int x, int y, UIMouseButton button) {
		wchar_t msg[1024];
		wsprintf(msg, L"Mouse Down %d %d %d", x, y, button);
		setTitle(msg);
		//SetCapture(hWnd);
	}

	virtual void messageMouseUp(int x, int y, UIMouseButton button) {
		wchar_t msg[1024];
		wsprintf(msg, L"Mouse Up %d %d %d", x, y, button);
		setTitle(msg);
	}

	virtual void messageMouseMove(int x, int y) {
		wchar_t msg[1024];
		wsprintf(msg, L"Mouse Move %d %d", x, y);
		setTitle(msg);
	}

	void messageMouseWheel(int x, int y, float dx, float dy)
	{
		wchar_t msg[1024];
		wsprintf(msg, L"Mouse Wheel %d %d %d %d", x, y, (int)dx, (int)dy);
		setTitle(msg);
	}

	virtual void messageWindowOpen() {
		wchar_t msg[1024];
		wsprintf(msg, L"Window Open");
		setTitle(msg);
	}

	virtual void messageWindowResize(int width, int height) {
		UIWindow::messageWindowResize(width, height);
		wchar_t msg[1024];
		wsprintf(msg, L"Window Resize %d %d", width, height);
		setTitle(msg);
	}

	virtual void messageWindowClose() {
		wchar_t msg[1024];
		wsprintf(msg, L"Window Close");
		setTitle(msg);
	}

};

int read_file() {
	HANDLE hFile = CreateFile(L"C:\\Projects\\Win32Project3\\Win32Project3\\UIWindow.cpp", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) return 1;
	DWORD length;
	if ((length = GetFileSize(hFile, 0)) == 0) return 1;
	char* buffer;
	if ((buffer = new char[length]) == 0) return 1;
	ULONG numread;
	ReadFile(hFile, buffer, length, &numread, 0);
	ULONG i = 0;
	ULONG linestart = 0;
	ULONG* linebuffer;
	if ((linebuffer = new ULONG[length]) == 0) return 1;
	ULONG numlines = 0;
	for (i = 0; i < length;) {
		if (buffer[i++] == '\r') {
			if (buffer[i] == '\n') i++;
			linebuffer[numlines++] = linestart;
			linestart = i;
		}
	}
	linebuffer[numlines] = length;
	CloseHandle(hFile);
	return 0;
}


size_t getline(char* buffer, ULONG* linebuffer, ULONG lineno, char *buf, size_t len)
{
	char *lineptr;
	size_t linelen;
	lineptr = buffer + linebuffer[lineno];
	linelen = linebuffer[lineno + 1] - linebuffer[lineno];
	linelen = min(len, linelen);
	memcpy(buf, lineptr, linelen);
	return linelen;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	MyWindow win1;
	//MyWindow win2;
	if (false && SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
		IFileOpenDialog* pFileOpen;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pFileOpen))) {
			if (SUCCEEDED(pFileOpen->Show(NULL))) {
				IShellItem*  pItem;
				if (SUCCEEDED(pFileOpen->GetResult(&pItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
						MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	/*win1.setComponent((new UIBorderLayout())->setCenterComponent(new UIButton(L"Center")));
	win1.setComponent((new UIBorderLayout())->setCenterComponent(new UIButton(L"Center"))->setLeftComponent(new UIButton(L"Left"))->setRightComponent(new UIButton(L"Right"))->setTopComponent(new UIButton(L"Top")));
	win1.setComponent((new UIButton(L"Abrir"))->setPosition(0, 0)->setSize(500, 400));
	win1.setComponent((new UIBorderLayout())->setCenterComponent(new UIButton(L"Center"))->setLeftComponent(new UIButton(L"Left")));
	win1.setComponent((new UIBorderLayout())->setCenterComponent(new UIButton(L"Center"))->setLeftComponent(new UIButton(L"Left"))->setRightComponent(new UIButton(L"Right")));
	win1.setComponent((new UIButton(L"Abrir")));
	win1.setComponent((new UITextField()));
	win1.setComponent((new UIBorderLayout())->setCenterComponent(new UIButton(L"Center"))->setLeftComponent(new UILabel(L"Left"))->setRightComponent(new UILabel(L"Right"))->setTopComponent(new UILabel(L"Top"))->setBottomComponent(new UILabel(L"Bottom")));
	win1.setComponent((new UIBorderLayout())->setCenterComponent(new UIButton(L"Center"))->setLeftComponent(new UIButton(L"Left"))->setRightComponent(new UIButton(L"Right"))->setTopComponent(new UIButton(L"Top"))->setBottomComponent(new UIButton(L"Bottom")));
	win1.setComponent((new UIButton(L"Abrir")));*/
	win1.setComponent((new UIBorderLayout())->setCenterComponent((new UIBorderLayout())->setCenterComponent(new UIRichText())->setLeftComponent(new UIButton(L"Left"))->setRightComponent(new UIButton(L"Right"))->setTopComponent(new UIButton(L"Top"))->setBottomComponent(new UIButton(L"Bottom")))->setLeftComponent(new UIButton(L"Left"))->setRightComponent(new UIButton(L"Right"))->setTopComponent(new UIButton(L"Top"))->setBottomComponent(new UIButton(L"Bottom")));
	win1.setVisible(true);
	//win2.setVisible(true);
	win1.eventLoop();

	return 0;
}
