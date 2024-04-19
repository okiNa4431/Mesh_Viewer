#include <filesystem>
#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    while (true)
    {
        POINT currentMousePos;
        GetCursorPos(&currentMousePos);
        ScreenToClient(GetActiveWindow(), &currentMousePos);
        cout << currentMousePos.x << " " << currentMousePos.y << endl;
    }
    return 0;
}