#!/usr/bin/python
import os
import time
import sys

if(len(sys.argv) == 2):
    if(sys.argv[1] == "command"):
        print("\n***test ligne de command:***\n")
        print("1. test ligne -b:")
        if os.system(r"./minicc -b") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")

        print("2. test ligne -h:")
        if os.system(r"./minicc -h") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")

        print("3. test ligne file.c:")
        if os.system(r"./minicc main.c") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")

        print("4. test ligne file.c -o file.s:")
        if os.system(r"./minicc main.c -o main.s") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")

        print("5. test ligne -s file.c:")
        if os.system(r"./minicc -s main.c") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")

        print("6. test ligne -o file.s -v file.c:")
        if os.system(r"./minicc -o main.s -v main.c") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")

        print("7. test ligne -o file.s -t 0 file.c -r 6:")
        if os.system(r"./minicc -o main.s -t 0 main.c -r 6") == 0:
                print("Execution Successful")
        else:
                print("Execution FAILED")
        print("---------------------\n")
#------------------------------------------------------------------------
elif(len(sys.argv) == 3):
    if((sys.argv[1] == "verif") & (sys.argv[2] == "ok")):
        print("\n***test Verif:***")
        print("test OK of Verif\n")
        for i in range(1, 4):
            command = "./minicc ./Tests/Verif/OK/" + str(i) + ".c"
            if os.system(command) == 0:
                print("example {0}: Execution Successful".format(i))
            else:
                print("exemple {0}: Execution FAILED".format(i))
            print("------------------\n")
    elif((sys.argv[1] == "verif") & (sys.argv[2] == "ko")):
        print("\n***test Verif:***")
        print("test KO of Verif\n")
        for i in range(1, 4):
            command = "./minicc ./Tests/Verif/KO/" + str(i) + ".c"
            if os.system(command) == 0:
                print("example {0}: Execution Successful".format(i))
            else:
                print("exemple {0}: Execution FAILED".format(i))
            print("------------------\n")

#------------------------------------------------------------------------
    elif((sys.argv[1] == "syntaxe") & (sys.argv[2] == "ok")):
        print("\n***test Syntaxe:***")
        print("test OK of syntaxe\n")
        for i in range(1, 4):
            command = "./minicc ./Tests/Syntaxe/OK/" + str(i) + ".c"
            if os.system(command) == 0:
                print("example {0}: Execution Successful".format(i))
            else:
                print("exemple {0}: Execution FAILED".format(i))
            print("------------------\n")
    elif((sys.argv[1] == "syntaxe") & (sys.argv[2] == "ko")):
        print("\n***test Syntaxe:***")
        print("test KO of syntaxe\n")
        for i in range(1, 4):
            command = "./minicc ./Tests/Syntaxe/KO/" + str(i) + ".c"
            if os.system(command) == 0:
                print("example {0}: Execution Successful".format(i))
            else:
                print("exemple {0}: Execution FAILED".format(i))
            print("------------------\n")
            
#------------------------------------------------------------------------
    elif((sys.argv[1] == "gencode") & (sys.argv[2] == "ok")):
        print("\n***test Gencode:***")
        print("test OK of Gencode\n")
        for i in range(1, 4):
            command = "./minicc ./Tests/Gencode/OK/" + str(i) + ".c"
            if os.system(command) == 0:
                os.system("java -jar ../tools/Mars_4_2.jar out.s")
                print("example {0}: Execution Successful".format(i))
            else:
                print("exemple {0}: Execution FAILED".format(i))
            print("------------------\n")
    elif((sys.argv[1] == "gencode") & (sys.argv[2] == "ko")):
        print("\n***test Gencode:***")
        print("test KO of Gencode\n")
        for i in range(1, 4):
            command = "./minicc ./Tests/Gencode/KO/" + str(i) + ".c"
            if os.system(command) == 0:
                print("example {0}: Execution Successful".format(i))
            else:
                print("exemple {0}: Execution FAILED".format(i))
            print("------------------\n")