/*+===================================================================
  File:        parts.cpp

  Summary:     Main Program file

  Author:      Antony Ashton     [asuperpower]    31-June-2015

  Rev History:
  31/6/2015: Intial Revision

----------------------------------------------------------------------
  Copyright 2015-2016 Radial Technologies, Pty Ltd
===================================================================+*/
#include <iostream>
#include <cstdlib>
#include <stdfunc.h>


int main()
{
    //Main loop
    while(handle_input() != "exit")
        printl(handle_input());

    return 0;
}

