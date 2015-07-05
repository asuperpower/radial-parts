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
    if(status == OK)
    {
        std::cout << "[OK] ";
    }
    else if(status == WARN)
    {
        std::cout << "[WARN] ";
    }
    else if(status == ERR)
    {
        std::cout << "[ERR] ";
    }

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
