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
std::fstream flags;

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

std::string flagread(std::string command)
{
    //input command example: flagread
    return "kqly";
}

void flagwrite(std::string command)
{
    //format:
    //flagwrite [letter] [name] [description]
    //example:
    //flagwrite q Quantity Specifies the quantity to be added. A required flag.

    std::string newflag[3];

    //ok, so we need to add a new flag in the file from the command 'flagwrite [letter] [name] [description]'
    //we can't seperate by spaces just yet as the description is allowed to have spaces.
    //this file has to be handled different to others, they have to be comma seperated rather than space seperated.
    //however the command can't be comma seperated, this confuses the user

    //so first thing we do is split the string in two
    //so it's 'flagwrite [letter] [name]' or 'flagwrite q Quantity ' and 'Specifies the quantity to be added. A required flag.'
    //(notice the the space is in the first string)
    //to split the string, we need to find where - and it's always 1 after the 3rd space. In the example the second string would start at command[21].

    //after we have two strings (lets call them string1 and string2)

    //we first remove 'flagwrite ' from the first one (including the space) and add it to a new string, lets call it string 1-1
    //next we take them, we turn the spaces in string 1-1 to commas and we rejoin them and write them to the file

    //-algorithm -----start ---------
    //find the third space so we know where to split the string
    unsigned int s = 0; //s stands for space! we stop when this is equal to 3!
    unsigned int i = 0;
    do
    {
        if (command[i] == ' ')
        {
            s++;
        }
        i++;
    }
    while (s < 3);
    //i is now where the new string should start.
    std::string string1[i-1];
    std::string string2[sizeof(command)-(i-1)];

    for (unsigned int k = 0; k < i; k++)//k < c means it stops before it reaches the space.
    {
        string1[k] = command[k];
    }
    for (unsigned int k = i; k < sizeof(command); k++)
    {
        string2[k] = command[k];
    }

    //turns all of the spaces in string1 into commas
    for (unsigned int j = 0; j < sizeof(string1); j++)
    {
        if (string1[j] == " ")
        {
            string1[j] = ",";
        }
    }

    flags.open("flags.csv", std::ios::out | std::ios::app);
    //adds new line to file.
    std::fstream classes("flags.csv", std::ios_base::app | std::ios_base::out);//http://stackoverflow.com/questions/10071137/appending-a-new-line-in-a-filelog-file-in-c
    flags << newflag << std::endl;
    flags.close();
}

void adder(std::string command)
{
    //removes add from the 'command', then adds it to 'newpart'
    //original string:
    //  add resistor 256 -v 20k -t 1% -p 1/2W

    //looks at amount of spaces in the command (and subtracts one from it because the first space is ignored as it is after add which is removed)
    //essentially it takes 'add resistor 256 -v 20k -t 1% -p 1/2W', counts the spaces and subtracts one to see how many elements the array newpart will need.
    //so while the above will have 8 spaces, 'resistor 256 -v 20k -t 1% -p 1/2W' which will be stored in the new array will only have 7.

    unsigned int z = 0;
    for (unsigned int i = 0; i < sizeof(command); i++)
    {
        if (command[i] == ' ')
        {
            z++;
        }
    }
    z -= 1; //one is removed from z, see above comment.

    if(z == 0)
    {
        printl("Something went wrong! (Amount of spaces in string returned zero)", ERR);
    }

    std::string newpart[z];

    std::vector<std::string> args = split(command, ' ');
    int idx = 1;
    do
    {
        newpart[idx-1] = args[idx];
    }
    while (args[idx] != "\0");

    //needs to check class file to ensure all flags are specified. If there is an error, it should state any missing flag(s)
    parts.open("parts.csv", std::ios::out | std::ios::app);
    //adds new line to file.
    std::fstream classes("parts.csv", std::ios_base::app | std::ios_base::out);//http://stackoverflow.com/questions/10071137/appending-a-new-line-in-a-filelog-file-in-c
    classes << newpart << std::endl;
    classes.close();



    /*bool missing = true;

    if (missing)
    {
        printl("Missing flags: ", ERR)
    }*/
}

void viewer(std::string command)
{
    //example view command:
    //view resistors -s quantity
    //where -s is sort by []
    //must lookup class file to see what flags need to be displayed
}

//This function adds a new class and stores it in a file called classfile.csv
void classhandler(std::string command)
{
    //this ignores the first part of the command as it is class and does not need to be added to the file
    //so instead of adding 'class resistor svt' , it adds 'resistor svt'
    std::string classToBeAdded[2];

    std::vector<std::string> args = split(command, ' ');
    int idx = 1;
    do
    {
        classToBeAdded[idx-1] = args[idx];
    }
    while (args[idx] != "\0");
    classes.open("classes.csv", std::ios::out | std::ios::app);
    //adds new line to file.
    std::fstream classes("classes.csv", std::ios_base::app | std::ios_base::out);//http://stackoverflow.com/questions/10071137/appending-a-new-line-in-a-filelog-file-in-c
    classes << classToBeAdded << std::endl;
    classes.close();
}

void commandhandler(std::string command)
{
    bool match = false;
    std::string commands[] = {"help", "add", "flagwrite", "flagdelete", "class", "view", "change", "exit"};

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

            if(args[1] == "change")
            {
                printl("designed to change a flag of an item.", INFO);//ADD MORE INFO LATERRRRR
            }


            //flag help ---------------------
            if(args[1] == "-q")
            {
                printl("Specifies the quantity to be added. A required flag.", INFO);//thinking about adding something that makes it default to 1 if no flag is added... that is for another day though.
            }
            if(args[1] == "-v")
            {
                printl("\tThis flag specifies the value of the component.\n\tExample:\n\t\tadd resistor 256 -v 20k\n\t\tThis example adds a resistor to the database in location 2, drawer 5, section 6 with the value 20k, determined by the flag.",INFO);
                return;
            }
            if(args[1] == "-t")
            {
                printl("\tTolerance of the component, as a percentage.\n\tExample:\n\t\tadd resistor -v 20k -t 1%", INFO);
                return;
            }
            if(args[1] == "-s")
            {
                printl("\tSorts by a flag, or sorts by \n\tUsage:\tfor command 'view'\n\tDefault:\tAlphabetically sorted.", INFO);//needs to be smart enough to sort 20k to be more than 2M
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

        if (args[COMMAND] == "flag")
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
    if(!flags.good())
    {
        printl("Flags file does not exist! Generating now...", WARN);
        flags.open("flags.csv", std::ios::out | std::ios::app);

        flags.close();
        printl("Done!", OK);
    }

    while(running)
    {
        std::cout << "Command>";
        commandhandler(handle_input());
    }


    return 0;
}
