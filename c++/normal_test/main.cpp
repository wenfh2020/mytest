/* wfh - 2018/09/01
 * c/c++ test code.
 */
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "class.h"

#define HELP_INFO "-virtual\n-pointer\n-fork\n-list\n-align\n"

void Help();
void PrintArgs(int iArgs, char** ppArgv);

int main(int iArgs, char** ppArgv) {
    //PrintArgs(iArgs, ppArgv);
    if (iArgs != 2) {
        printf("please input arg!\n");
        return 1;
    }

    CTest oTest;
    std::string strArgv(ppArgv[1]);

    if (strArgv.find("help") != std::string::npos) {
        Help();
    } else if (strArgv == "virtual") {
        oTest.TestVirtual();
    } else if (strArgv == "pointer") {
        oTest.TestPointer();
    } else if (strArgv == "fork") {
        oTest.TestFork();
    } else if (strArgv == "list") {
        oTest.TestListReverse();
    } else if (strArgv == "align") {
        oTest.TestByteAlign();
    } else {
        printf("invalid arg! please ./exe help for arg!\n");
        return 2;
    }

    return 0;
}

void PrintArgs(int iArgs, char** ppArgv) {
    printf("arg count = %d\n", iArgs);
    for (int i = 0; i < iArgs; i++) {
        printf("arg value[%d] = %s\n", i, ppArgv[i]);
    }
}

void Help() {
    printf("%s", HELP_INFO);
}
