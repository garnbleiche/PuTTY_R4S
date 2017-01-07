# PuTTY R4S
A modified version of PuTTY for integration into Remote4Support

## New command-line arguments
* **hwndparent**: the handle of the parent window

If **hwndparent** is set, the PuTTY window becomes a child of the specified window. Modal windows errors do not appear, the error is written to the terminal window. In the case of closing the connection to the server message will be sent to the parent window
```c
#define WM_USER_CLOSE_SESSION WM_USER + 1
#define CLOSE_OK 0
#define CLOSE_FATAL 1
....
SendMessage(hwnd_parent, WM_USER_CLOSE_SESSION, 0, CLOSE_FATAL);
....
```

Links
========================

* PuTTY main page: [http://www.chiark.greenend.org.uk/~sgtatham/putty/](http://www.chiark.greenend.org.uk/~sgtatham/putty/)
