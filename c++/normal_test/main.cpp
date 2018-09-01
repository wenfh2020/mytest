/* wfh - 2018/09/01
 * c/c++ test code.
 * */
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

void Help() {
    const char* pInfo = "-virtual\n"\
                        "-pointer\n";
    printf("%s", pInfo);
}

int main(int args, char** argv) {
    //PrintArgs(args, argv);
    if (args != 2) {
        printf("please input arg!\n");
        return 0;
    }

    CTest oTest;
    std::string strArgv(argv[1]);

    if (strArgv.find("help") != std::string::npos) {
        Help();
    } else if (strArgv == "virtual") {
        oTest.TestVirtual();
    } else if (strArgv == "pointer") {
        oTest.TestPointer();
    } else {
        printf("invalid arg! please ./exe help for arg!\n");
    }

    return 0;
}
