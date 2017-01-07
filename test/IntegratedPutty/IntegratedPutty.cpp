#include <Windows.h>

struct PINFO {
  HWND main_window = 0;
  HWND putty_window = 0;
  HANDLE hProcess = nullptr;
  bool putty_run = false;
} process_info;

// test ssh server
wchar_t user[] = L"guest";
wchar_t password[] = L"guest";
wchar_t ip[] = L"192.168.0.5";

#define WM_USER_CLOSE_SESSION WM_USER + 1
#define CLOSE_OK 0
#define CLOSE_FATAL 1

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR nCmdLine, int nCmdShow)
{
  const wchar_t CLASS_NAME[] = L"WindowClass";
  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.lpszClassName = CLASS_NAME;
  wc.hInstance = hInstance;
  RegisterClass(&wc);

  process_info.main_window = CreateWindowEx(0, CLASS_NAME, L"Integrated Putty", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

  if (process_info.main_window == 0)
    return 0;

  ShowWindow(process_info.main_window, nCmdShow);
  nCmdShow = 1;

  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

void resizePutty()
{
  RECT rcClient;
  GetClientRect(process_info.main_window, &rcClient);
  int caption_h = GetSystemMetrics(SM_CYCAPTION);
  int border_h = GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
  int borrder_w = GetSystemMetrics(SM_CXSIZEFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
  MoveWindow(process_info.putty_window, -borrder_w, -(caption_h + borrder_w), rcClient.right + borrder_w * 2, rcClient.bottom + caption_h + border_h * 2, true);
}

void execPutty()
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  wchar_t czCommandLine[200];
  wsprintf(czCommandLine, L"putty.exe -load \"Default Settings\" -ssh -2 -l %s -pw %s -P 22 %s -hwndparent %lu", user, password, ip, process_info.main_window);

  CreateProcess(NULL, czCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

  process_info.hProcess = pi.hProcess;
  process_info.putty_run = true;
  WaitForInputIdle(process_info.hProcess, 2000);
  process_info.putty_window = FindWindowEx(process_info.main_window, 0, nullptr, nullptr);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_DESTROY:
      // kill putty
      TerminateProcess(process_info.hProcess, 0);
      PostQuitMessage(0);
      return 0;

    case WM_CREATE:
      return 0;

    case WM_ACTIVATEAPP:
      if (!process_info.putty_run) {
        execPutty();
        resizePutty();
      }
      break;
    case WM_SIZE:
      resizePutty();
      break;
    case WM_USER_CLOSE_SESSION:
      if (lParam == CLOSE_OK) {
        SetWindowText(process_info.main_window, L"current session closed");
      } else {
        SetWindowText(process_info.main_window, L"current session fatal closed");
      }
      break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}