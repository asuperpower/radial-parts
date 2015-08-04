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

std::fstream parts("parts.csv");
std::fstream classes("classes.csv");
std::fstream flags("flags.csv");

//BUGS FOR ME TO LOOK FOR IN FUTURE:
/*
a) sometimes I think that /n = 0x0 which of course it isn't, sometimes I split at 0x0 instead of 0xA
b) sometimes I do something weird in a for loop, if args[i].size() is found somewhere, it should be args.size
*/

//I should add from the start that while using the descriptor 'class' to describe the different types of components (resistors, capacitors etc.),
//it's a meaningful way to describe it, so I will keep it that way for now.

//COMMON ERROR CODES:
/*
0xA0: Comma found in command
0xA1: General Error with Command
0xA2: Input is NULL!!!
0xF0: Classfile not configured!!!
*/

//Select what we want to do - example function, not used in the program... probably.
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

/*std::vector<std::string> flagread()
{
    std::string flagfile;
    flags.open("flags.csv", std::ios::out | std::ios::app | std::ios::binary);
    while(std::getline(flags, flagfile));
    flags.close();
    std::vector<std::string> flagread = split(flagfile, '\n');
    return flagread;
}*/

bool commaexists(std::string command)
{
    for (unsigned int i = 0; i <= command.length(); i++)
    {
        if (command[i] == ',')
        {
            return true;
        }
    }
    return false;
}

void flagwrite(std::string command)
{
    /*+===================================================================
    Command Input Looks Like:
    flagwrite [letter] [name] [description]

    Example of this:
    flagwrite q Quantity Specifies the quantity to be added. A required flag.

    File Output Looks Like:
    q,Quantity,Quantity Specifies the quantity to be added. A required flag.
    NOTE:
    Notice how the description isn't comma separated. Special tricks below!
    ===================================================================+*/

    if(commaexists(command))
    {
        printl("USING COMMAS IS AN ILLEGAL OPERATION. COMMAND: '" + command + "' TERMINATED.", ERR);
        return;
    }

    std::vector<std::string> args = split(command, ' ');

    //comma seperate the values manually, then add the rest to the string in the for loop.
    std::string shortflag = args[1] + "," + args[2] + ",";
    std::string newflag;

    for (unsigned int i = 3; i <= args.size(); i++)
    {
        //adds the part of the space seperated string back to the string, adding the space back in the process.
        newflag += args[i];
        newflag += ' ';
    }

    flags.open("flags.csv", std::ios::out | std::ios::app);
    flags << newflag << std::endl;
    flags.close();
}

void flagprocessor(std::string &tempclassati, std::string &classcommand)
{
    /*
    Essentially what this function does is check the classcommand input by the user against the flag file,
    and because
    */
    for(unsigned int l = tempclassati.length(); tempclassati[l] != ' '; l--)
            {
               int sizebeforespace = 0;
               for (unsigned int i = 0; i <= tempclassati.length(); i++)
               {
                   if (tempclassati[i] == ' ')
                   {
                       sizebeforespace++;
                       break;
                   }
                   else
                   {
                       sizebeforespace++;
                   }
               }
               classcommand[l-sizebeforespace] = tempclassati[l];
            }
    return;
}

bool classmatcher(std::string &command, std::string &classcommand, std::string &tempclassati, std::vector<std::string> &args, std::vector<std::string> &classidx)
{
    //NOTE::THIS FUNCTION IS CAUSING A SEGFAULT
    //needs to check class file to ensure all flags are specified. If there is an error, it should state any missing flag(s)
    classes.open("classes.csv", std::ios::out | std::ios::app);
    std::string classfile;
    while(getline(classes, classfile));
    classes.close();

    if (classfile == "")
    {
        printl("Classfile has not been configured!", ERR);
        exit(0xF0);
    }

    //split the command by space
    args = split(command, ' ');
    //split the csv by each newline
    classidx = split(classfile, 0xA);
    //split the command by space so we can compare the following:
    /* the class file should look like this:
    resistor qvtp
    n-chn qcvpb //b is beta value, c is code
    capacitor qvtp
    */

    //ERROR CHECKING TO MAKE SURE THAT THE CLASS IS A MATCH
    //WHERE THE COMMAND IS:
    //add [class] [location] [-q flag] [other flags]
    //std::string classcommand /<-/WAS HERE

    for (unsigned int i = 0; i <= classidx.size(); i++)
    {
        std::string tempclassati = classidx[i];//was//std::string tempclassati = classidx[i];
        std::string classmatcher;//was//std::string classmatcher;
        for (unsigned int j = 0; tempclassati[j] != ' '; j++)//removes everything after the space from tempclassati and puts it in classmatcher
        {
            classmatcher[j] = tempclassati[j];
        }

        if (classmatcher == args[1]) //args[1] is [classname]
        {
            //we found a match!
            //before the break the loop, we better take that class line (classidx[i]), which looks something like 'resistor qvtp'
            //we are taking tempclassati and processing that
            //two questions: a) why take that and b) why are we processing that?
            //a) we don't use classmatcher as that only takes 'resistor' and not 'qvtp', which are the accepted flags, which is the part we want.
            //b) we need the flags for later and it's nice to just have the string "qvtp" so we can put it in a for loop and compare it later
            //THIS PROCESS IS NOW DONE IN THE FUNCTION CALLED IN THE NEXT LINE
            flagprocessor(tempclassati, classcommand);
            return true;
        }
    }
    return false;
}

void adder(std::string command)
{
    /*+===================================================================
    Command Input Looks Like:
    add resistor 256 -q 10 -v 20k -t 1% -p 1/2W

    File Output Looks Like:
    resistor,256,-q10,-v20,-t1%,-p1/2W,\n
    ===================================================================+*/

    if(commaexists(command))
    {
        printl("USING COMMAS IS AN ILLEGAL OPERATION. COMMAND: '" + command + "' TERMINATED.", ERR);
        return;
    }

    /*+==========
    These are created in THIS function,
    then they are ALL passed into the function
        bool classmatcher(...)
    by reference

    and then the following strings
        tempclassati
        classcommand
    are passed by into the function
        void flagprocessor(...)
    by reference
    ==========+*/
    std::string classcommand;
    std::string tempclassati;
    std::vector<std::string> args;
    std::vector<std::string> classidx;

    if (!classmatcher(command, classcommand, tempclassati, args, classidx))
    {
        printl("Error with command: '" + command + "' !!!", ERR);
        exit(0xA1);
    }

    parts.open("parts.csv", std::ios::out | std::ios::app);
    //does the first 2 non-flag ones normally
    for (unsigned int i = 1; i < 3; i++)
    {
        parts << args[i];
        parts << ',';
    }
    //args[3] is the first flag, '-q'
    //therefore, if the command input follows convention
    for (unsigned int i = 3; i < args.size(); i++)
    {

    }
    parts << std::endl;
    parts.close();
}

std::string getFlags(std::string commandclass)
{
    classes.open("classes.csv", std::ios::out | std::ios::app);
    std::string classfile;
    while(getline(classes, classfile));
    classes.close();

    std::vector<std::string> args = split(classfile, 0xA);  //Splits at the newline character

    for (unsigned int i = 0; i <= args.size(); i++)
    {
        std::string classstring = args[i];
        //std::vector<std::string> classline = (classstring, ',');
        unsigned int commaPoint = 0;
        for (unsigned int k = 0; k <= classstring.length(); k++)
        {
            if (classstring[k] == ',')
            {
                commaPoint = k;
            }
        }
        if (!commaPoint)//this should never happen
        {
            printl("error in std::string getFlags(std::string commandclass) - commaPoint = 0 - no commas found in file (file corrupt?)", ERR);
            exit (0xB0);
        }

        std::string className;
        std::string classFlags;

        for (unsigned int z = 0; z <= classstring.length(); z++)
        {
            std::string classtring = args[i];//why does this need to be here? IDK HALP
            if (z < commaPoint)
            {
                className[z] = classtring[z];
            }
            if (z > commaPoint)
            {
                classFlags[z] = classstring[z];
            }
        }
        if (className == classfile)
        {
            return classFlags;
        }
    }
    return "0000";
}

void viewer(std::string command)
{
    /*+===================================================================
    Command Input Looks Like:
    view resistors -s quantity
    where -s is sort by [property]
    ===================================================================+*/

    std::vector<std::string> args = split(command, ' ');
    const std::string commandclass = args[1];//the *command* specifies the class such as resistor, n-chn etc.

    std::string flag = getFlags(commandclass);//gets the flags for the class - for example, the class resistor would be something like 'qvtp' (quantity, value etc.) (-q -v -t -p)
    //the function getFlags returns "0000" if there is NOT a flag match, so we check...
    if (flag == "0000")
    {
        printl("No class known as '" + commandclass + "' found!!!", ERR);
        return;
    }

    //Now we look at each flag file and find the corresponding name of the flag letter. For example, we look up the flag letter 'q' and find the name 'Quantity'

    return;
}

//This function adds a new class and stores it in a file called classfile.csv
void classhandler(std::string command)
{
    /*+===================================================================
    Command Input Looks Like:
    class resistor qtvp

    File Output Looks Like:
    resistor,qtvp
    ===================================================================+*/
    if (command == "")
    {
        printl("Command is NULL - Something has gone wrong!!!", ERR);
        exit(0xA2);
    }

    std::vector<std::string> args = split(command, ' ');

    //error checking
    if (args.size() != 3)//if it exists - if more than 3 arguments
    {
        if (args.size() <3)
        {
            printl("\tBad Command\n\tReason:\tNot Enough Arguments!!!\n\tCommand:\n\t\t" + command, ERR);
            return;
        }
        else
        {
            printl("\tBad Command\n\tReason:\tToo Many Arguments!!!\n\tCommand:\n\t\t" + command, ERR);
            return;
        }

    }

    classes.open("classes.csv", std::ios::out | std::ios::app);
    classes << args[1] << ',' << args[2] << std::endl;
    classes.close();
}

void commandhandler(std::string command)
{
    bool match = false;
    const std::string commands[] = {"help", "commands", "add", "flagwrite", "flagdelete", "class", "classinfo", "view", "change", "cls", "clear", "exit"};

    std::cout << commands[0];
    while(1);


    //Split our string into chunks according to where a space is!
    std::vector<std::string> args = split(command, ' ');

    //Check for valid commands
    //match = validcmdcheck(); -- turn this into function later..........
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
            if(args[1] == "commands")
            {
                std::string commanderino = "General Commands: ";
                for (unsigned int i = 0; i < command.size(); i++)
                {
                    commanderino += (command[i] + ",");
                }
                printl(commanderino, INFO);
                return;
            }

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
                return;
            }

            if(args[1] == "flagwrite")
            {
                printl("Syntax:\t flagwrite [flag letter] [flag name] [flag description]\n\n\tExample:\n\t\tflagwrite q Quantity Specifies the quantity to be added. A required flag.", INFO);//ADD MORE INFO LATERRRRR
                return;
            }

            //flag help ---------------------

            //manual flag help - takes precedence as always has better automation.
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
            if(args[1] == "-p")
            {
                printl("\tMax Power of the Component, as a Percentage.\n\tExample:\n\t\tadd resistor -v 20k -p 1/2W", INFO);
                return;
            }
            if(args[1] == "-c")
                printl("\tComponent Code.\n\tExample:\n\t\tadd n-chnFET -q 3 -c 510", INFO);
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

        if (args[COMMAND] == "commands")
        {
            /*std::string commanderino = "General Commands: ";
            strcat(commanderino, commands);
            printl(commanderino, INFO);*/
            return;
        }

        if (args[COMMAND] == "flag")
        {
            printl("THIS FEATURE HASN'T BEEN ADDED YET!", INFO);
            return;
        }

        if (args[COMMAND] == "add")
        {
            adder(command);
            return;
        }

        if (args[COMMAND] == "flagwrite")
        {
            flagwrite(command);
            return;
        }

        if (args[COMMAND] == "class")
        {
            if(args.size() == 1)
            {
                //lists all of the classes

                //opens class file, imports into string
                //seperates into vector, splits with \n
                //prints each string until it finds a space, then prints each induvidual letter preceded by a space and a dash (-)

                //example:
                /*file is
                resistor qvtp
                n-chn qcvpb //b is beta value, c is code
                capacitor qvtp
                */
                /*output is:
                class:
                    resistor
                flags:
                    -q -v -t -p

                class:
                    n-chn qcvpb
                flags:
                    -q -c -v -p -b

                class:
                    capacitor
                flags:
                    -q -v -t -p
                */
            }
            else
            {
                classhandler(command);
            }
            return;
        }

        if(args[COMMAND] == "exit" || args[COMMAND] == "quit")
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
