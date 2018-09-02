#include "class.h"
#include<unistd.h>
#include<sys/types.h>
#include <stdlib.h>
void CTest::TestVirtual() {
    printf("test virtual function!\n");
    printf("sizeof empty class %lu\n", sizeof(CEmpty));
    printf("sizeof virtual class, base =  %lu, child = %lu\n",
           sizeof(CBase), sizeof(CChild2));

    CChild2* pChild2 = new CChild2;
    CBase* pBase = (CBase*)pChild2;
    CChild* pChild = (CChild*)pBase;
    pChild2->Func();
    pChild2->Func3();
    pChild->Func2();
    pChild->Func3();
    pBase->Func();
    pBase->Func2();
    pBase->FuncA();
    pChild2->FuncA();
    delete (CChild*)pChild;
}

void CTest::TestPointer() {
    printf("test pointer!\n");

    //char* pp = "1234"; //warnning
    char szInfo[10] = {"123456789"};
    //can not change content.
    const char* p = szInfo;
    char const* p2 = p;

    //can not change address.
    char* const p3 = szInfo;

    p++;
    p2++;
    //p3++; //build failed

    //p[1] = 't'; //build failed
    //p2[1] = 't'; //build failed
    p3[1] = 't';
}

void CTest::TestFork() {
    printf("test fork!\n");

    printf("begin fork\n");
    pid_t iPid = fork(); //fork return twice, so..
    if (0 == iPid) {
        //child
        printf("fork child! pid = %d\n", getpid());
    } else if (iPid > 0) {
        //parent
        printf("fork parent! pid = %d\n", getpid());
    } else {
        printf("fork failed!\n");
        return;
    }
    printf("after fork\n");
}

void CTest::TestListReverse() {
    printf("test list reverse!\n");

    TList oList;
    TListNode* pNodeTemp = NULL;
    TListNode* pNodeTemp2 = NULL;

    //add items
    for (int i = 0; i < 10; i++) {
        pNodeTemp = (TListNode*)malloc(sizeof(TListNode));
        pNodeTemp->iData = i;
        pNodeTemp->pNext = NULL;
        oList.AddTail(pNodeTemp);
    }

    //print items
    oList.PrintList();

    //reverse
    oList.Reverse();

    //print items
    oList.PrintList();

    //free data.
    pNodeTemp = oList.GetHead();
    while (pNodeTemp != NULL) {
        pNodeTemp2 = pNodeTemp->pNext;
        free(pNodeTemp);
        pNodeTemp = pNodeTemp2;
    }
}
