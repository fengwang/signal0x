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
        const function_type
        operator()( const F& f_ ) const 
        { 
            return f_;
        }

        template<typename F, typename... Fs>
        const function_type
        operator()( const F& f_, const Fs&... fs_ ) const
        {
            const function_type f = chain_function<R, Args...>()(f_);
            const function_type g = chain_function<R, Args...>()(fs_...);
            return [f, g](Args... args) -> return_type { f(args...); return g(args...); };
            //const function_type ans = [f, g](Args... args) -> return_type { f(args...); return g(args...); };
            //return ans;
        }

    };

    template< typename R, typename... Args, typename... Fs >
    const typename chain_function<R, Args...>::funciton_type
    make_chain_function( const Fs&... fs )
    {
        return chain_function<R, Args...>()(fs...);
    }


}//namespace signal0x

#endif//_CHAIN_FUNCTION_HPP_INCLUDED_SOFIHJ498UFSDKLJHKJFSDLKJSFDNMSIOHJ4EOIUSFDKJHEIUHSFDKJHEIUHSDKFJHSAKJFCKJHDKJHSFDDS

