#ifndef _CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS
#define _CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS

#include <functional>

namespace signal0x
{

    template< typename R, typename... Args >
    struct chain_function
    {
        typedef R return_type;
        typedef std::function<R(Args...)> function_type;
   
        template<typename F>
        function_type&&
        operator()( const F& f_ ) const 
        { 
            function_type ans( f_ );
            return std::move(ans);
        }

        template<typename F, typename... Fs>
        function_type&&
        operator()( const F& f_, const Fs&... fs_ ) const
        {
            function_type&& f = chain_function<R, Args...>()(f_);
            function_type&& g = chain_function<R, Args...>()(fs_...);
            function_type ans = [&f, &g](Args... args) -> return_type { f(args...); return g(args...); };
            return std::move(ans);
        }

    };

    template< typename R, typename... Args, typename... Fs >
    typename chain_function<R, Args...>::funciton_type&&
    make_chain_function( const Fs&... fs )
    {
        return std::move(chain_function<R, Args...>()(fs...));
    }


}//namespace signal0x

#endif//_CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS

