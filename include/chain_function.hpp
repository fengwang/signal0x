#ifndef _CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS
#define _CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS

#include <functional>

namespace signal0x
{
    namespace signal0x_private
    {
    //
    //  an example:
    //              void g(int);
    //              struct h{ void operator(int); };
    //              struct i{ void f(int); };
    //              i i_;
    //
    //              //make a chain
    //              auto chain = chain_function(    //a lambda function
    //                                              [](int i){std::cout<<i;}, 
    //                                              //a normal function
    //                                              g, 
    //                                              //a functor
    //                                              h(), 
    //                                              //a pointer to a class method
    //                                              std::bind( &i::f, &i_, std::placeholders::_1 ) 
    //                                          );
    //              //call the chain function
    //              int n;
    //              chain(n); //this will call \lambda(n), g(n), (h())(n), i_(n) in turn
    //
    template< typename R, typename... Args >
    struct chain_function
    {
        typedef R return_type;
        typedef std::function<R(Args...)> function_type;
   
        template<typename F>
        const function_type
        operator()( const F& f_ ) const 
        { return f_; }

        template<typename F, typename... Fs>
        const function_type
        operator()( const F& f_, const Fs&... fs_ ) const
        {
            const function_type f = chain_function<R, Args...>()(f_);
            const function_type g = chain_function<R, Args...>()(fs_...);
            return [f, g](Args... args) -> return_type { f(args...); return g(args...); };
        }
    };
    }//namespace signal0x_private
}//namespace signal0x

#endif//_CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS

