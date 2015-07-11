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

std::fstream parts;

//Select what we want to do
void select(std::string input)
{
    //Crude way of splitting strings
    std::string::size_type pos = input.find_first_of(','); //Find positions where a ',' is located
    std::string command = input.substr(0, pos); //Command data

    if(command == "exit")
    {
        running = false;
        return; //Return so we don't get any stray unknown commands. Basically break us out at this point!
    }
    if(command == "add part")
    {
        std::string data = input.substr(pos + 1, input.length());

        //Write the data to the csv file
        parts.open("parts.csv", std::ios::out | std::ios::app);
        parts << data << ",0" << std::endl;
        parts.close();
        printl("Done adding part!", OK);
        return;
    }
    else
    {
        printl("Unknown Command!", ERR);
        return;
    }
}

void commandhandler(std::string command)
{
    bool match = false;
    std::string commands[] = {"help", "exit"};

    //Get position of each space in the entire string of input
    std::string::size_type pos = command.find_first_of(' '); //Find positions where a ',' is located

    //Extract the command part of the data
    std::string comm = command.substr(0, pos);

    //Check for valid commands
    for(unsigned int i = 0; i < sizeof(commands) / sizeof(*commands); i++) //Divide the sizeof the array (in characters) by the size of a character pointer
    {
        if(comm == commands[i])
        {
            match = true;
            break;
        }
    }

    if(match)
    {
        if(comm == "help")
        {

        }
    }
    else
    {
        printl("Unknown Command: " + command, ERR);
    }
}

int main()
{
    //Main loop
    printl("Welcome to the parts picker!!\nFor Help, enter 'help'.", OK);

    //Check if our parts file actually exists!
    if(!parts.good())
    {
        printl("Parts file does not exist! Generating now...", WARN);
        parts.open("parts.csv", std::ios::out | std::ios::app);

        parts.close();
        printl("Done!", OK);
    }

    while(running)
    {
        std::cout << "Command>";
        commandhandler(handle_input());
    }

    std::string command;
    command = handle_input();


    return 0;
}
