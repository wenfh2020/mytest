#ifndef _CLASS_
#define _CLASS_

#include <stdio.h>

class CEmpty {
};

class CBase {
public:
    CBase() {
        printf("base()\n");
    }
    virtual ~CBase() {
        printf("~base()\n");
    }
    virtual void Func() {
        printf("base func!\n");
    }
    virtual void Func2() {
        printf("base func2!\n");
    }
    void FuncA() {
        printf("base func a\n");
    }
};

class CBase2 {
public:
    CBase2() {
        printf("base2()\n");
    }
    virtual ~CBase2() {
        printf("~base2()\n");
    }
    virtual void Func() {
        printf("base2 func\n");
    }
};

class CChild : public CBase, public CBase2 {
public:
    CChild() {
        printf("child()\n");
    }
    virtual ~CChild() {
        printf("~child()\n");
    }
    virtual void Func2() {
        printf("child func2\n");
    }
    virtual void Func3() {
        printf("child func3\n");
    }
};

class CChild2 : public CChild {
public:
    CChild2() {
        printf("child2()\n");
    }
    virtual ~CChild2() {
        printf("~child2()\n");
    }
    virtual void Func() {//delete virtual key, it work!
        printf("child2 func\n");
    }
    virtual void Func3() {
        printf("child2 fun3\n");
    }
    void FuncA() {
        printf("child2 func a\n");
    }
};

class CClass {
public:
    CClass() {}
    virtual ~CClass() {}

public:
    void TestVirtual();
};

#endif
