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
