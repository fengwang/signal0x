#include <signal.hpp>
#include <iostream>
#include <iterator>
#include <functional>

using namespace signal0x;


struct hello
{
    void operator()() const
    {
        std::cout << " Hello ";
    }
};

void world()
{
    std::cout << " World! ";
}

void print_1( int i )
{
    std::cout << "\t"<< i;
}

void double_1( int i )
{
    std::cout << "\t" << i+i;
}


double d1( double d )
{ std::cout << "\nd1 called" << std::endl; return d; }

double d2( double d )
{ std::cout << "\nd2 called" << std::endl; return d+d; }

double d3( double d )
{ std::cout << "\nd3 called" << std::endl;  return d1(d)+d2(d); }

struct d4 
{
    double do_d4( double d ) const 
    {
        std::cout << "\nd4 called" << std::endl;
        return d3(d) + d1(d);
    }
};

int main()
{
    signal<void> s;
    hello h;

    connection_type c1 = s.connect( hello() );
    connection_type c2 = s.connect( h, 1 );
    connection_type c3 = s.connect( world, 2);

    s();
    std::cout << "\n\n";

    s.disconnect( c1 );
    s();
    std::cout << "\n\n";

    s.disconnect( c2 );

    s();
    std::cout << "\n\n";

    s.disconnect( c3 );


    signal<void,int> s1;
    s1.connect( print_1 );
    s1.connect( double_1 );
    s1(10);
    std::cout << "\n";


    signal<double, double> dd;
    dd.connect( d1 );
    dd.connect( d2 );
    dd.connect( d3 );
    d4 d4_;
    dd.connect( std::bind(&d4::do_d4, &d4_, std::placeholders::_1) );

    dd( std::ostream_iterator<double>(std::cout,"\t"), 1.0 );
    //dd.emit( std::ostream_iterator<double>(std::cout,"\t"), 1.0 );

    dd.disconnect_all();
    std::cout << "\n[[disconnecting all]]\n" << std::endl;
    dd( 1.0);

    auto con = dd.connect( d1, d2, d3, std::bind(&d4::do_d4, &d4_, std::placeholders::_1), [](double d){ std::cout << "\nlamda fun called" << std::endl; return d; } );
    std::cout << "\n[[chain function as slot]]\n" << std::endl;
    dd( 2.0);

    dd.disconnect( con );
    std::cout << "\n[[chain function disconnect]]\n";
    dd(3.0);


    return 0;
}




