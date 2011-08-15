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

    (*h).n = 2;

    s.connect( *h, world );

    std::cout << "\nwith ptrs\n";
    s();

    delete h; h = 0;

    std::cout << "\nwithout ptrs\n";
    s();

    return 0;
}




