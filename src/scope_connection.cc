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

int main()
{
    signal0x::signal<void> sig;
    sig.connect( f<0>() );
    std::cout << "\nsignal emmit out of scope.";
    sig();
    {
        signal0x::scope_connection<void> s( sig, f<1>() );
        std::cout << "\nsignal emmit in scope.";
        sig();
    }
    std::cout << "\nsignal emmit out of scope.";
    sig();

    return 0;
}







