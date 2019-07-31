#include <iostream>
#include <string>

// first construct，last destruct
// class copy B(const B& o)

class A {
   public:
    A() { std::cout << this << " A::A " << std::endl; }
    A(char* p) {
        m_strData = p;
        std::cout << this << " A::A " << m_strData.c_str() << std::endl;
    }

    A(const char* p) {
        m_strData = p;
        std::cout << this << " A::A " << m_strData.c_str() << std::endl;
    }
    ~A() { std::cout << this << " A::~A " << m_strData.c_str() << std::endl; }

   public:
    const char* GetData() { return m_strData.c_str(); }

   private:
    std::string m_strData;
};

class B {
   public:
    B() { std::cout << this << " B::B " << std::endl; }
    B(const B& o) {
        m_strData = o.m_strData;
        std::cout << this << " B::B(&) " << std::endl;
    }
    B(char* p) {
        m_strData = p;
        std::cout << this << " B::B " << m_strData.c_str() << std::endl;
    }
    B(const char* p) {
        m_strData = p;
        std::cout << this << " B::B " << m_strData.c_str() << std::endl;
    }
    ~B() { std::cout << this << " B::~B " << m_strData.c_str() << std::endl; }

   public:
    const char* GetData() { return m_strData.c_str(); }

   private:
    std::string m_strData;
};

void test(B b) {}

int main() {
    B b("b");
    std::cout << "---" << std::endl;
    test(b);
    return 0;
}