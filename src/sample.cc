#include <signal.hpp>

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

int fi( int i )
{
    auto ans =  i * i + i;
    std::cout << "\nnormal function fi -- " << ans;
    return ans;
}

struct fii
{
    int operator() (int i) const
    {
        auto ans = i * i * i;
        std::cout << "\nfunctor of struct fii -- " << ans;
        return ans;
    }

    int hay( int i ) const 
    {
        auto ans = i * i * i * i;
        std::cout << "\nmember ponter of struct fii --hay " << ans;
        return ans;
    }

    static int goo( int i )
    {
        auto ans = i * i * i * i * i;
        std::cout << "\nstatic member ponter of struct fii --goo " << ans;
        return ans;
    }

    private:
        int foo;
    public: 
        fii() : foo( -1 ) {}

        int f(int i) const 
        { 
            auto ans = foo*i; 
            std::cout << "\nmember pointer of struct fii --> f " << ans;
            return ans;
        }
};

int main()
{
    signal0x::signal<int, int> sig;

    std::cout << "\n[[normal test]]\n";
    //connect to normal function
    auto s0 = sig.connect( fi );
    //connect to lambda object
    auto s1 = sig.connect( [](int i) { auto ans = i+1; std::cout << "\nlambda object -- " << ans; return ans; } );
    //connect to function object 
    std::function<int(int)> fiii = []( int i ) { auto ans = i*i; std::cout << "\nfunction object fiii -- " << ans; return ans; };
    auto s2 = sig.connect( fiii );
    //connect to functor
    auto s3 = sig.connect( fii() );
    //connect to a class with operator()
    fii FII;
    auto s4 = sig.connect( FII );
    //connect to member pointer
    auto s5 = sig.connect( std::bind( &fii::hay, &FII, std::placeholders::_1 ) );
    //connect to member pointer
    auto s6 = sig.connect( std::bind( &fii::goo, std::placeholders::_1) );
    sig( -1 );

    std::cout << "\n\n[[collect result with a container]]\n";
    std::vector<int> arr;
    sig( std::back_inserter(arr), 6 );
    std::cout << "\n\n-->>the collected result:\n\n";
    std::copy( arr.begin(), arr.end(), std::ostream_iterator<int>(std::cout, "\n"));

    std::cout << "\n\n[[disconnect one by one]]\n";
    sig.disconnect( s0 );
    sig(0);
    sig.disconnect( s1 );
    sig(1);
    sig.disconnect( s2 );
    sig(2);
    sig.disconnect( s3 );
    sig(3);
    sig.disconnect( s4 );
    sig(4);
    sig.disconnect( s5 );
    sig(5);
    sig.disconnect( s6 );
    sig(6);

    std::cout << "\n\n[[connection with priority]]\n";
    sig.connect( [](int i) { std::cout << i << "\n"; return i; } );
    sig.connect( [](int i) { std::cout << i+10 << "\n"; return i; }, 10 );
    sig.connect( [](int i) { std::cout << i+20 << "\n"; return i; }, 20 );
    sig.connect( [](int i) { std::cout << i-10 << "\n"; return i; }, -10 );
    sig.connect( [](int i) { std::cout << i-20 << "\n"; return i; }, -20 );

    sig(7);

    //block the signal
    std::cout << "\n\n[[block the signal]]\n\n";
    sig.block();
    sig(8);

    //unblock the signal
    std::cout << "\n\n[[unblock the signal]]\n\n";
    sig.unblock();
    sig(9);


    std::cout << "\n\n[[disconnect all]]\n\n";
    sig.disconnect_all();
    sig(10);




    return 0;
}





