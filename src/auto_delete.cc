#include <signal.hpp>
#include <iostream>
#include <iterator>
#include <functional>

using namespace signal0x;


struct hello
{
    hello() : n( -1 ) {}
    int n;
    void operator()() const
    {
        std::cout << n << " Hello ";
    }
};

void world()
{
    std::cout << " World! ";
}


int main()
{
    signal<void> s;

    hello* h = new hello;

    s.connect( *h, world );

    (*h).n = 2;

    std::cout << "\nwith ptrs\n";
    s();

    delete h; h = 0;

    std::cout << "\nwithout ptrs\n";
    s();

    return 0;
}




