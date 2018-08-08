#include "class.h"

void CClass::TestVirtual() {
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
    delete pChild;
}
