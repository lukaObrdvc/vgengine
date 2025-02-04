#include "Windows.h"

int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
{

    int i = 5;
    
    int j = _Generic(1.0, int : 2, double : 3);
    
    MessageBoxA(
                0,
                "Hello Sailor",
                "Message box",
                MB_OK
                );
    
    return 0;

}
