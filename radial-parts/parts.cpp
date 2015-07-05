/*+===================================================================
  File:        parts.cpp

  Summary:     Main Program file

  Author:      Antony Ashton     [asuperpower]    31-June-2015

  Rev History:
  31/6/2015: Intial Revision

----------------------------------------------------------------------
  Copyright 2015-2016 Radial Technologies, Pty Ltd
===================================================================+*/
#include <stdfunc.h>
#include <baseio.h>

bool running = true;

std::string commands[] = {"help", "exit"};
std::fstream parts;

//Select what we want to do
void select(std::string input)
{
    std::cout << input;
    if(input == "exit")
    {
        running = false;
        return;
    }
    if(input == "add part")
    {
        printl("Here!!", OK);
        return;
    }
    else
    {
        printl("Unknown Command!", ERR);
        return;
    }
}

int main()
{
    //Main loop
    printl("Welcome to the parts picker!!", OK);

    //Check if our parts file actually exists!
    if(!parts.good())
    {
        printl("Parts file does not exist! Generating now...", WARN);
        parts.open("parts.csv", std::ios::out | std::ios::app);
        parts << "resistor,0";
        //Write some stuff here!
        parts.close();
        printl("Done!", OK);
    }

    while(running)
    {
        std::cout << "Command>";
        select(handle_input());
    }

    return 0;
}

