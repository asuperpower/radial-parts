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
 *  A neater way of calling std::cout <<
 */
void printl(std::string str)
{
    std::cout << str << std::endl;
}

/*
 * Function:  handle_input
 * --------------------
 *
 *  Handle one line of input from the console
 */
std::string handle_input()
{
    std::string ret;
    std::cin >> ret;
    return ret;
}
