#include "Timer_win32.h"

/////////////////////////////////////////////////////////////////////////////
/// Win32 timers
///
struct Win32Timer
{
  UINT_PTR handle;
  Fl_Timeout_Handler callback;
  void *data;
};

static Win32Timer* win32_timers;
static int win32_timer_alloc;
static int win32_timer_used;
static HWND s_TimerWnd;

static void realloc_timers()
{
  if (win32_timer_alloc == 0) {
    win32_timer_alloc = 8;
  }
  win32_timer_alloc *= 2;
  Win32Timer* new_timers = new Win32Timer[win32_timer_alloc];
  memset(new_timers, 0, sizeof(Win32Timer) * win32_timer_used);
  memcpy(new_timers, win32_timers, sizeof(Win32Timer) * win32_timer_used);
  Win32Timer* delete_me = win32_timers;
  win32_timers = new_timers;
  delete [] delete_me;
}


static void delete_timer(Win32Timer& t)
{
  KillTimer(s_TimerWnd, t.handle);
  memset(&t, 0, sizeof(Win32Timer));
}

static LRESULT CALLBACK s_TimerProc(HWND hwnd, UINT msg,
                                    WPARAM wParam, LPARAM lParam)
{
  switch (msg) {
  case WM_TIMER:
    {
      unsigned int id = (unsigned) (wParam - 1);
      if (id < (unsigned int)win32_timer_used && win32_timers[id].handle) {
        Fl_Timeout_Handler cb   = win32_timers[id].callback;
        void*              data = win32_timers[id].data;
        delete_timer(win32_timers[id]);
        if (cb) {
          (*cb)(data);
        }
      }
    }
    return 0;

  default:
    break;
  }

  return DefWindowProc(hwnd, msg, wParam, lParam);
}

void add_timeout(double time, Fl_Timeout_Handler cb, void* data, HINSTANCE *hinst)
{
  repeat_timeout(time, cb, data, hinst);
}

void repeat_timeout(double time, Fl_Timeout_Handler cb, void* data, HINSTANCE* hinst)
{
  int timer_id = -1;
  for (int i = 0;  i < win32_timer_used;  ++i) {
    if ( !win32_timers[i].handle ) {
      timer_id = i;
      break;
    }
  }
  if (timer_id == -1) {
    if (win32_timer_used == win32_timer_alloc) {
      realloc_timers();
    }
    timer_id = win32_timer_used++;
  }
  unsigned int elapsed = (unsigned int)(time * 1000);

  if ( !s_TimerWnd ) {
    const char* timer_class = "FLTimer";
    WNDCLASSEX wc;
    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof (wc);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = (WNDPROC)s_TimerProc;
    wc.hInstance = (HINSTANCE) hinst;
    wc.lpszClassName = timer_class;
    /*ATOM atom =*/ RegisterClassEx(&wc);
    // create a zero size window to handle timer events
    s_TimerWnd = CreateWindowEx(WS_EX_LEFT | WS_EX_TOOLWINDOW,
                                timer_class, "",
                                WS_POPUP,
                                0, 0, 0, 0,
                                NULL, NULL, *hinst, NULL);
    // just in case this OS won't let us create a 0x0 size window:
    if (!s_TimerWnd)
      s_TimerWnd = CreateWindowEx(WS_EX_LEFT | WS_EX_TOOLWINDOW,
				  timer_class, "",
				  WS_POPUP,
				  0, 0, 1, 1,
				  NULL, NULL, *hinst, NULL);
    ShowWindow(s_TimerWnd, SW_SHOWNOACTIVATE);
  }

  win32_timers[timer_id].callback = cb;
  win32_timers[timer_id].data     = data;

  win32_timers[timer_id].handle =
    SetTimer(s_TimerWnd, timer_id + 1, elapsed, NULL);
}

int has_timeout(Fl_Timeout_Handler cb, void* data)
{
  for (int i = 0;  i < win32_timer_used;  ++i) {
    Win32Timer& t = win32_timers[i];
    if (t.handle  &&  t.callback == cb  &&  t.data == data) {
      return 1;
    }
  }
  return 0;
}

void remove_timeout(Fl_Timeout_Handler cb, void* data)
{
  int i;
  for (i = 0;  i < win32_timer_used;  ++i) {
    Win32Timer& t = win32_timers[i];
    if (t.handle  &&  t.callback == cb  &&
      (t.data == data  ||  data == NULL)) {
      delete_timer(t);
    }
  }
}
