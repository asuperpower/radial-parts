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

#define COMMAND 0

bool running = true;

std::fstream parts;
std::fstream classes;

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

void adder(std::string command)
{

}

void viewer(std::string command)
{
    //example view command:
    //view resistors -s quantity
    //where -s is sort by
}

//This function adds a new class and stores it in a file called classfile.csv
void classhandler(std::string command)
{

    std::vector<std::string> args = split(command, ' ');

    //this ignores the first part of the command as it is class and does not need to be added to the file
    //so instead of adding 'class resistor svt' , it adds 'resistor svt'
    std::string classToBeAdded[2];
    int idx = 1;
    do
    {
        classToBeAdded[idx-1] = command[idx];
    }
    while (args[idx] != "\0");
    classes.open("classes.csv", std::ios::out | std::ios::app);
    //adds new line to file.
    std::fstream classes("classes.csv", std::ios_base::app | std::ios_base::out);//http://stackoverflow.com/questions/10071137/appending-a-new-line-in-a-filelog-file-in-c
    classes << classToBeAdded;
    classes.close();
}

void commandhandler(std::string command)
{
    bool match = false;
    std::string commands[] = {"help", "add", "flag", "class", "view", "cls", "exit"};

    //Split our string into chunks according to where a space is!
    std::vector<std::string> args = split(command, ' ');

    //Check for valid commands
    for(unsigned int i = 0; i < sizeof(commands) / sizeof(*commands); i++) //Divide the sizeof the array (in characters) by the size of a character pointer
    {
        if(args[COMMAND] == commands[i])
        {
            match = true;
            break;
        }
    }

    if(match)
    {
        if(args[COMMAND] == "cls" || args[COMMAND] == "clear")
        {
            cls();
        }
        if(args[COMMAND] == "help")
        {
            //misc help ---------------------
            if(args[1] == "class")
            {
                std::string classLine1 = "A class is a group of components. An example of a class would be a resistor, or n-channel mosfet.";
                std::string classLine2 = "For a class, the information you should include is the name of the flag and the flags to be used.";
                std::string classLine3 = "The information is saved in the csv file 'classes.csv'";
                std::string classLine4 = "";//"NOTE: At the moment, the order of the flags that you input to be associated with the class is the same order they appear in when viewed.";
                std::string classLine5 = "Example:";
                std::string classExample1 = "class nchn-mosfet vs";
                std::string classNote1 = "'vs' in the example above refers to the flags that are associated with the class.";
                std::string classLine6 = "You can also delete classes. To do this type delete class followed by the class name.";
                std::string classExample2 = "delete class nchn-mosfet.";
                std::string classNote2 = "You can also view all classes by typing 'classview'";
                printl("\t" + classLine1 + "\n\t" + classLine2 + "\n\t" + classLine3 + "\n\t" + classLine4 + "\n\t" + classLine5 + "\n\t\t" + classExample1 + "\n\t" + classNote1 + "\n\t" + classLine6 + "\n\t\t" + classExample2 + "\n\t" + classNote2, INFO);
                return;
            }


            //flag help ---------------------
            if(args[1] == "-v")
            {
                printl("\tThis flag specifies the value of the component.\n\tExample:\n\t\tadd resistor 256 -v 20k\n\t\tThis example adds a resistor to the database in location 2, drawer 5, section 6 with the value 20k, determined by the flag.",OK);
                return;
            }
            if(args[1] == "-t")
            {
                printl("\tTolerance of the component, as a percentage.\n\tExample:\n\t\tadd resistor -v 20k -t 1%", OK);
                return;
            }
            if(args[1] == "-s")
            {
                printl("\tSorts by a flag, or sorts by \n\tUsage:\tfor command 'view'\n\tDefault:\tAlphabetically sorted.", OK);//needs to be smart enough to sort 20k to be more than 2M
                return;
            }

            //PUT ALL HELP COMMANDS BEFORE HERE.
            //By this time, if none of the other paths were taken and there is still a flag input, it is an invalid help command.
            //First we check for an invalid flag help command, and then check for an invalid word after help that isn't part of the string 'commands'.
            if (args[1][0] == '-')
            {
                printl("\tINVALID FLAG: " + command + "\n\tPerhaps it hasn't been added yet? - type help flags for more.", ERR);
                return;
            }

            //SOMETIMES this comes back as true when only 'help' is typed - code commented out as it is not reliable.
            /*if ((args[1][0] > 0x41 && args[1][0] < 0x5A) || (args[1][0] > 0x61 && args[1][0] < 0x7A))
            {
                printl("\tINVALID HELP COMMAND: " + command, ERR);
                return;
            }*/
            else
            {
                printl("\tCommands:\n\t--------------------\n\t'add'\n\t\tAdds a part to the database. You are required to specify the location, quantity.\n\tExample:\n\t\tadd resistor 356 -s 20k -v 5 -t 1% -p 5\n\n\t--------------------\n\t'view'\n\t\tView the database. Add no modifiers to view the entire database.\n\tExample:\n\t\tview resistors -s quantity \n\n\t--------------------\n\tFor help on adding a new class, type 'help class'.\n\n\t--------------------\n\tTo get help about a specific flag, type help, and then the flag.\n\tExample:\n\t\thelp -s", INFO);
                return;
            }
        }

        if (args[COMMAND] == "flags")
        {
            printl("THIS FEATURE HASN'T BEEN ADDED YET!", INFO);
            return;
        }

        if (args[COMMAND] == "class")
        {
            classhandler(command);
            return;
        }

        if(args[COMMAND] == "exit")
        {
            exit(0x00); //Exit with status code 0
            return;
        }
    }
    else
    {
        printl("Unknown Command: " + command, ERR);
        return;
    }
}

int main()
{
    //Main loop
    printl("Welcome to the parts picker!!\nFor Help, enter 'help'.", OK);

    //Check if the required files exist.
    if(!parts.good())
    {
        printl("Parts file does not exist! Generating now...", WARN);
        parts.open("parts.csv", std::ios::out | std::ios::app);

        parts.close();
        printl("Done!", OK);
    }
    if(!classes.good())
    {
        printl("Class file does not exist! Generating now...", WARN);
        classes.open("classes.csv", std::ios::out | std::ios::app);

        classes.close();
        printl("Done!", OK);
    }

    while(running)
    {
        std::cout << "Command>";
        commandhandler(handle_input());
    }


    return 0;
}
