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

bool running = true;

std::string commands[] = {"help", "exit"};

//Select what we want to do
void select(std::string input)
{
    if(input == "exit")
    {
        running = false;
    }
    else
    {
        printl("Unknown Command!");
    }
}

int main()
{
    //Main loop
    printl("Welcome to the parts picker!!");
    while(running)
        select(handle_input());

    return 0;
}

