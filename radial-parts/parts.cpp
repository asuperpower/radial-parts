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
    const std::string help = "help";//looks for this match in a command - ie: if help is typed then help is given
    bool match = false;
    //looks for match in help and if the first four characters of the command input by the user is "help", then match is true and the if statement is executed.
    for(unsigned int i = 0; i < strlen(help); i++)
    {
        if help[i] = command[i]
        {
            match = true;
        }
        else
        {
            match = false;
            break;
        }
    }
    if (match)//executed if the start of the users command is "help"
    {
        if (!command[4])//essentially, if only help is typed, does the help script
        {
            printl("\tCommands:\n\t--------------------\n\t'add'\n\t\tAdds a part to the database. You are required to specify the location, quantity.\n\tExample:\n\t\tadd resistor 356 -s 20k -v 5 -t 1% -p 5\n\t--------------------\n\t'view'\n\t\tView the database. Add no modifiers to view the entire database.\n\tExample:\n\t\tview resistors -s quantity \n\t--------------------\n\tTo create a new class, such as resistor or mosfet, type 'class' followed by the class name. The class tutorial will help you set up your class.\n\t--------------------\n\tTo get help about a specific flag, type help, and then the flag.", OK);
        }

        //outputting error command if something different happens - if it doesn't follow 'help -[letter]'
        if (command[4] != ' ')
        {
            printl("Unknown Command!",ERR);
            return;
        }
        if (command[5] != '-')
        {
            printl("Unknown Command!",ERR);
            return;
        }
        if (command[7])
        {
            printl("Unknown Command!",ERR);
            return;
        }

        //so in all other instances from here on, it is assumed that help from a flag is requested - for example: 'help -q' or 'help -v'.
        //If this isn't the case, the unknown command error will be output.

        if (command[6] == 'v')
        {
            printl("\tThis flag specifies the value of the component.\n\tExample:\n\t\tadd resistor 256 -v 20k\n\t\tThis example adds a resistor to the database in location 2, drawer 5, section 6 with the value 20k, determined by the flag.",OK);
            return;
        }
        if (command[6] == 't')
        {
            printl("\tTolerance of the component, as a percentage.\n\tExample:\n\t\tadd resistor -v 20k -t 1%");
            return;
        }
        if (command[6] == 's')
        {
            printl("\tSorts by a flag, or sorts by \n\tUsage:\tfor command 'view'\n\tDefault:\tAlphabetically sorted.");//needs to be smart enough to sort 20k to be more than 2M
            return;
        }
        if (command[6] == 'n')
        {
            printl("\tCreates a new flag. The tutorial associated with the creation guides you through the process.");
            return;
        }
    }
    else
    {
        printl("Unknown Command!",ERR)
        return;
    }

}

int main()
{
    //Main loop
    printl("Welcome to the parts picker!!\nFor Help, enter 'help'.", OK);
    std::string command;
    command = handle_input();

    commandhandler(command);

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
        select(handle_input());
    }

    return 0;
}
