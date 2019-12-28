#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
    HWND edit = FindWindowA("GxWindowClass","Ä§ÊÞÊÀ½ç");
    //HWND edit = FindWindowExA(h,NULL,"Edit",NULL);
    if(edit)
    {
        cout << "find!\n";
        

        SendMessageA(edit,WM_CHAR,'w',NULL);
        SendMessageA(edit,WM_CHAR,'W',NULL);
        SendMessageA(edit,WM_CHAR,' ',NULL);
    }
    system("pause");
    return 0;
}