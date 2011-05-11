#include <signal.hpp>

#include <iostream>
#include <cstddef>

template< std::size_t N >
struct f
{
    void operator()() const 
    {
        std::cout << "\nfunctor f<" << N << "> called.";
    }
};

void g1()
{
    std::cout << "\nfunction g1 called.";
}

int main()
{
    signal0x::signal<void> sig;

    std::cout << "\n\n!!Chain function connection!!\n";
    f<7> f7;
    auto c = sig.connect( f<0>(), f<1>(), f<2>(), f<3>(), f<4>(), f<5>(), f<6>(), f7, g1 );
    sig();
    
    sig.disconnect(c);
    std::cout << "\n\n!!disconnecting\n\n";
    sig();

    sig.connect( f<10>(), f<11>(), f<22>(), f<33>(), f<45>(), f<65>(), f<96>() );
    std::cout << "\n\nchain connection again.\n";
    sig();

    return 0;
}







