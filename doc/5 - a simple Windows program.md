# A Simple Windows Program #

To write a Windows program in C or C++, we must install MS Windows Software Development Kit (SDK) (which includes necessary **headers**, **libraries**, **compiler** and **linker**).

## Application entry point ##
Every Windows program includes an entry-point function named `wWinMain` (or `WinMain`).

```c++
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    return 0;
};
```

- `hInstance` - the handle to an instance. OS uses it to identify the application when it's loaded in memory.
- `hPrevInstance` - no meaning, is now always zero.
- `pCmdLine` - contains the CLI arguments as a Unicode string.
- `nCmdShow` - a flag that indicates whether the main application window is minimized, maximized, or shown normally.


A brief lifetime of a Windows Application:

- When the program starts, `wWinMain` registers some information about the behavior of the application window. One of the most important items is the address of a function, named `WindowPorc`.

- The program creates the window and receives a handle that uniquely identifies the window.

- The program enters a while `loop`. The program remains in this loop until the user closes the window and exits the application.

    > Note, the program doesn't explicitly call the `WindowPorc` function. Windows communicates with the program by passing it a series of messages. Each time the program calls the `DispatchMessage` function, it indirectly causes Windows to invoke the `WindowProc` function, once for each message.

## Create a window ##

A window class is a data structure which defines a set of behaviors that several windows might have in common. Every window must be associated with a window class.

Windows classes are registered with the system at run time. To register a new window class, fill in a `WNDCLASS` structure.

```c++
// Register the window class.
const wchar_t CLASS_NAME[]  = L"Sample Window Class";

WNDCLASS wc = { };

wc.lpfnWndProc   = WindowProc;
wc.hInstance     = hInstance;
wc.lpszClassName = CLASS_NAME;

RegisterClass(&wc);
```

- `lpfnWndProc` - a pointer to WindowPorc
- `hInstance` - the handle to the application instance
- `lpszClassName` - a string that identifies the window class.

Next, create a new instance of a window.

```c++
HWND hwnd = CreateWindowEx(
    0,                              // Optional window styles.
    CLASS_NAME,                     // Window class
    L"Learn to Program Windows",    // Window text
    WS_OVERLAPPEDWINDOW,            // Window style

    // Size and position
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    NULL,       // Parent window    
    NULL,       // Menu
    hInstance,  // Instance handle
    NULL        // Additional application data
    );

if (hwnd == NULL)
{
    return 0;
}

ShowWindow(hwnd, nCmdShow); // make the window visible

```
CreateWindowEx returns to a handle to the new window, or zero if the function fails.


## Windows messages ##

A GUI application must respond to events from the user and the operating system.

- **Events from the user** - include all the ways that someone can interact with your program: mouse clicks, key strokes, and so on.
- **Events from the operating system** - include anything "outside" of the program that can affect how the program behaves: plug in a new hardware device, or Windows enters a lower-power state.

These events can occur at any time while the program is running, in almost any order. To solve this problem, Windows uses a message-passing model. The operating system communicates with your application window by passing messages to it.

For each thread that creates a window, the operating system creates a queue for window message. You can pull a message from the queue by calling the `GetMessage` function. This function removes the first message from the head of the queue. If the queue is empty, the function blocks until another message is queued.

```c++
    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
```

The `TranslateMessage` function is related to keyboard input. It translates keystrokes (key down, key up) into characters. 

The `DispatchMessage` function tells the operating system to call the window procedure of the window that is the target of the message.

## Window procedure ##

The window procedure has the following signature:
```c++
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
```

- `hwnd` - a handle to the window.
- `uMsg` - the message code.
- `wParam/lParam` - contain additional data that pertains to the message.
- `LRESULT` - an integer value that your program returns to Windows.

A typical window procedure is simply a large switch statement that switches on the message code.

```c++
switch (uMsg)
{
    case WM_SIZE: // Handle window resizing

    // etc
}
```

A typical window procedure handles dozens of messages, so it can grow quite long. One way to make your code more modular is to put the logic for handling each message in a separate function.

If you don't handle a particular message in your window procedure, pass the message parameters directly to the `DefWindowProc` function. This function performs the default action for the message, which varies by message type.

```c++
return DefWindowProc(hwnd, uMsg, wParam, lParam);
```

> Reference: https://learn.microsoft.com/en-us/windows/win32/learnwin32/your-first-windows-program