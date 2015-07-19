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

//COMMON ERROR CODES:
/*
0xA0: Comma found in command
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

void flagwrite(std::string command)
{
    //format:
    //flagwrite [letter] [name] [description]
    //example:
    //flagwrite q Quantity Specifies the quantity to be added. A required flag.

    //I'VE GOT A BETTER WAY TO DO THIS!!!
    //I BEGIN BY SPLITTING IT BY SPACE
    std::vector<std::string> args = split(command, ' ');
    std::string newflag;

    //error checking - commas are very not allowed as it stuffs up the file IO.
    for (unsigned int a = 1; a <= args.size(); a++)
    {
        for (unsigned int b = 0; b <= args[a].length(); b++)
        {
            std::string word = args[a];
            if (word[b] == ',')
            {
                printl("Commas are not allowed in the command.", ERR);
                exit(0xA0);
            }
        }
    }

    //comma seperate the values manually, then add the rest to the string in the for loop.
    newflag = args[1] + "," + args[2] + ",";

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

void adder(std::string command)
{
    //removes add from the 'command', then adds it to 'newpart'
    //original string:
    //  add resistor 256 -v 20k -t 1% -p 1/2W

    //needs to check class file to ensure all flags are specified. If there is an error, it should state any missing flag(s)
    classes.open("classes.csv", std::ios::out | std::ios::app);
    std::string classfile;
    while(getline(classes, classfile));
    classes.close();
    std::vector<std::string> args = split(command, ' ');

    //split the csv getline
    std::vector<std::string> classidx = split(classfile, 0);
    //split the command by space so we can compare the following:
    /* the class file should look like this:
    resistor qvtp
    n-chn qcvpb //b is beta value, c is code
    capacitor qvtp
    */

    //ERROR CHECKING TO MAKE SURE THAT THE CLASS IS A MATCH
    //WHERE THE COMMAND IS:
    //add [class] [location] [-q flag] [other flags]
    bool match = false;

    std::string classcommand;

    for (unsigned int i = 0; i <= classidx.size(); i++)
    {
        std::string tempclassati = classidx[i];
        std::string classmatcher;
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

            for(unsigned int l = tempclassati.length(); tempclassati[l] != ' '; l--)
            {
               int sizebeforespace = 0;
               for (i = 0; i <= tempclassati.length(); i++)
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

            match = true;
            break;
        }
    }
    if (!match)
    {
        printl("No such class " + args[1] + " found in database.", ERR);
        exit(0xF1);
    }

    //END ERROR CHECKING FOR CLASS MATCH

    //Now we check the flags...
    //for the command
    /*add resistor 256 -v 20k -t 1% -p 1/2W*/
    //the flags start at the fourth command argument, therefore we set 'flagstart' to 4.
    const unsigned int flagstart = 4;
    for (unsigned int i = flagstart; i <= args.size(); i += 2)
    //THE REASON i increments by 2
    //in this, we are checking to ensur that the first part of the command (args)'s flag is - to ensure it is a flag
    {
        bool issue = false;
        std::string temparg = args[i];
        if (temparg[0] != '-')
        {
            issue = true;
        }
        if(issue)
        {
            printl("found non-flag command argument where there should be a flag command argument.", ERR);
            return;
        }

        //searches through flags
        bool found = false;
        for (unsigned int h = 0; h <= classcommand.length(); h++)
        {
            if (temparg[1] == classcommand[h])
            {
                found = true;
                break;
            }
        }

        if(!found)
        {
            printl("found non-flag command argument where there should be a flag command argument: " , ERR);
            return;
        }


    }


    parts.open("parts.csv", std::ios::out | std::ios::app);
    for (unsigned int i = 0; i <= args.size(); i++)
    {
        parts << args[i];
        parts << ',';
    }
    parts << std::endl;
    parts.close();



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
    //where -s is sort by [property]
    //must lookup class file to see what flags need to be displayed

    //as usual we begin by removing the view command at the start so it's just 'resistors -s quantity'

    //then we check if resistors is a class match
}

//This function adds a new class and stores it in a file called classfile.csv
void classhandler(std::string command)
{
    //NOTE: need to add checking to ensure is valid flag
    //example: if the input is 'class resistor qtc
    std::vector<std::string> args = split(command, ' ');

    //error checking
    if (args.size() > 3)//if it exists - if more than 3 arguments
    {
        printl("\tBad Command\n\tReason:\tToo Many Arguments\n\tCommand:\n\t\t" + command, ERR);
        return;
    }

    classes.open("classes.csv", std::ios::out | std::ios::app);

    classes << args[1] << ',' << args[2] << std::endl;

    classes.close();
}

void commandhandler(std::string command)
{
    bool match = false;
    std::string commands[] = {"help", "add", "flagwrite", "flagdelete", "class", "view", "change", "cls", "clear", "exit"};

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
