/*+===================================================================
  File:        stdfunc.cpp

  Summary:     Implementation of stdfunc.cpp

  Author:      Jesse Buhagiar     [Quaker]    2-July-2015

  Rev History:
  2/7/2015: Intial Revision

----------------------------------------------------------------------
  Copyright 2015-2016 Radial Technologies, Pty Ltd
===================================================================+*/

#include <stdfunc.h>


/*
 * Function:  printl
 * --------------------
 *
 *  Print one line of text with a carriage return.
 */
void printl(std::string str, int status)
{
    //Temporary status string
    std::string stat;

    //Check the status and change colour accordingly.
    if(status == OK)
    {
        setcol(OK_COL);
        std::cout << "[OK] ";
    }
    else if(status == WARN)
    {
        setcol(WARN_COL);
        std::cout << "[WARN] ";
    }
    else if(status == ERR)
    {
        setcol(ERR_COL);
        std::cout << "[ERR] ";
    }
    else if(status == INFO)
    {
        setcol(INFO_COL);
        std::cout << "[INFO]";
    }

    setcol(0x07);
    std::cout << str << std::endl;
}

/*
 * Function:  handle_input
 * --------------------
 *
 *  Handle one line of input from the console.
 */
std::string handle_input()
{
    std::string ret;
    std::getline(std::cin, ret);
    return ret;
}

/*
 * Function:  split
 * --------------------
 *
 *  Split a string according to a token
 *  Taken from: http://www.cplusplus.com/articles/2wA0RXSz/
 */
std::vector<std::string> split(const std::string& input, const char& delim)
{
    std::string buff{""};
    std::vector<std::string>s;

    for(auto i:input)
    {
        if(i != delim)
            buff+=i;
        else
            if(i == delim && buff != "")
        {
            s.push_back(buff);
            buff = "";
        }
    }

    if(buff != "")
        s.push_back(buff);

    return s;
}

/*
 * Function:  setcol
 * --------------------
 *
 *  Set the colour of the (windows specific) console
 */
#ifdef _WIN32
void setcol(int colour)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colour);
}
#endif // _WIN32

/*
 * Function:  cls
 * --------------------
 *
 *  Clear the screen. Thanks Microsoft!
 *  https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022%28v=vs.85%29.aspx
 */
#ifdef _WIN32

void cls()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }
#endif // _WIN32
