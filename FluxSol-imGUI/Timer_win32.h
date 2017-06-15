#ifndef _TIMER_WIN32_H_
#define _TIMER_WIN32_H_
/////////////////////////////////////////////////////////////////////////////
/// Win32 timers
///
/** Signature of some timeout callback functions passed as parameters */
typedef void (*Fl_Timeout_Handler)(void *data);

#include <windows.h>
static LRESULT CALLBACK s_TimerProc(HWND hwnd, UINT msg,
                                    WPARAM wParam, LPARAM lParam);

void add_timeout(double time, Fl_Timeout_Handler cb, void* data, HINSTANCE *hinst);

void repeat_timeout(double time, Fl_Timeout_Handler cb, void* data, HINSTANCE *hinst);

int has_timeout(Fl_Timeout_Handler cb, void* data);

void remove_timeout(Fl_Timeout_Handler cb, void* data);


#endif
extern  HINSTANCE fl_display;
//extern Window fl_window;
extern HDC fl_gc;
extern MSG fl_msg;
//extern HDC fl_GetDC(Window);
extern HDC fl_makeDC(HBITMAP);
