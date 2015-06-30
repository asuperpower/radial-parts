#include <iostream>
#include <cstdlib>
#include "include/stdfunc.h"

int main()
{
    enum returnvalues{view, edit, exit};
    int menu = mainmenu();
    if (menu == exit)
    {
        return 0;
    }
    return 0;
}

int mainmenu()
{
    enum returnvalues{view, edit, exit};
    int x = 0;
    system("cls");
    std::cout << "1.\tView Parts\n2.\tEdit Parts\n3.\tExit\n";
    std::cin >> x;
    if (x == 3)
    {
        //close any open files here?
        return exit;
    }
}
