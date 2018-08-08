#include <stdio.h>
#include <iostream>
#include <string.h>
#include "class.h"

void PrintArgs(int iArgs, char** ppArgv) {
    printf("arg count = %d\n", iArgs);
    for (int i = 0; i < iArgs; i++) {
        printf("arg value[%d] = %s\n", i, ppArgv[i]);
    }
}

void help() {
    const char* pInfo = "a - \n"\
                        "b - \n";
    printf("%s", pInfo);
}

int main(int args, char** argv) {
    //PrintArgs(args, argv);
    if (args < 2 || args > 3) {
        return 0;
    }

    std::string strArgv1(argv[1]), strArgv2;
    if (2 == args) {
        if (strArgv1 == "help") {
            help();
            return 0;
        }
        return 0;
    }

    strArgv2 = argv[2];
    if (strArgv1 == "a") {
    } else if (strArgv1 == "b") {
        CClass oClass;
        if (strArgv2 == "construct") {
            oClass.TestVirtual();
        }
    }
    return 0;
}
