#ifndef _SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE
#define _SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE

#include <singleton.hpp>// for signal0x::singleton 

#include <cstddef>      // for std::ptrdiff_t
#include <functional>   // for std::function
#include <utility>      // for std::forward, std::move
#include <algorithm>    // for std::swap
#include <map>          // for std::map
#include <utility>      // for std::make_pair
#include <limits>       // for std::numeric_limits<T>::max()
#include <mutex>        // for std::mutex etc.

namespace signal0x
{

    typedef std::ptrdiff_t                                  connection_type;

    template< typename R, typename... Args >
    struct signal
    {
        typedef std::ptrdiff_t                                  connection_type;
        typedef int                                             priority_type;
        typedef signal                                          self_type;
        typedef self_type                                       publisher_type;
        typedef std::function<R( Args... )>                     function_type;
        typedef function_type                                   subscriber_type;
        typedef function_type                                   slot_type;
        typedef std::map<connection_type, subscriber_type>      associate_connection_subscriber_type;
        typedef std::map<priority_type, associate_connection_subscriber_type> 
                                                                priority_connection_subscriber_type;
        typedef std::mutex                                      mutex_type;
        typedef std::lock_guard<mutex_type>                     lock_guard_type;
    private:
        bool                                                    is_blocked_;
        priority_connection_subscriber_type                     pcst_;
        mutable mutex_type                                      m_;

    public:
        signal() : is_blocked_(false) {}
        signal( const self_type& other ) : is_blocked_( other.is_blocked_ ), 
                                           pcst_( other.pcst_ ) {}
        signal( self_type&& other ) : is_blocked_( std::move(other.is_blocked_) ), 
                                      pcst_( std::move(other.pcst_) ) {}
        // 1)  lock both mutexes safely 
        // 2)  adopt the ownership into the std::lock_guard instances  
        //     to ensure the locks are released safely at the end of the function.
        self_type& 
        operator = ( const self_type& other )
        {   // 1)
            std::lock( m_, other.m_); 
            // 2)
            lock_guard_type l1( m_, std::adopt_lock );      
            lock_guard_type l2( other.m_, std::adopt_lock );
            is_blocked_ = other.is_blocked_;
            pcst_ = other.pcst_;
            return *this;
        }
        self_type& 
        operator = ( self_type&& other )
        {
            is_blocked_ = std::move(other.is_blocked_);
            pcst_ = std::move(other.pcst_);
            return *this;
        }

        template< typename F >
        const connection_type
        connect(F&& f, const priority_type w = std::numeric_limits<priority_type>::max() )
        {
            auto&c =  singleton<connection_type>::instance();
            auto const cc = c++;
            lock_guard_type l( m_ );      
            (pcst_[w]).insert( std::make_pair( cc, (function_type)(std::forward<F>(f)) ) );
            return cc;
        }

        template< typename F >
        const connection_type
        connect(const F& f, const priority_type w = std::numeric_limits<priority_type>::max() )
        {
            auto&c =  singleton<connection_type>::instance();
            auto const cc = c++;
            lock_guard_type l( m_ );      
            (pcst_[w]).insert( std::make_pair( cc, (function_type)(f) ) );
            return cc;
        }

        template< typename F >
        const connection_type
        connect( const priority_type w, F f )
        { return connect( f, w ); }


        template< typename... F >
        const connection_type
        connect( F... f )
        {
        
        }



        void 
        disconnect( const connection_type& c )
        {
            lock_guard_type l( m_ );      
            for ( auto& i : pcst_ )
                    i.second.erase( c );
        }

        void 
        disconnect_all()
        { 
            lock_guard_type l( m_ );      
            pcst_.clear(); 
        }

        void 
        operator()( Args... args ) const
        { 
            lock_guard_type l( m_ );      
            if ( is_blocked_ ) return;
            for ( auto const & i : pcst_ )
                for ( auto const & j : i.second )
                    (j.second)( args... );
        }

        template<typename Output_Iterator>
        Output_Iterator
        operator() ( Output_Iterator o, Args... args ) const
        {
            lock_guard_type l( m_ );      
            if ( is_blocked_ ) return o;
            for ( auto const & i : pcst_ )
                for ( auto const & j : i.second )
                    *o++ = (j.second)( args... );
            return o;
        }

        void 
        block() 
        { 
            lock_guard_type l( m_ );      
            is_blocked_ = true; 
        }

        void 
        unblock()
        { 
            lock_guard_type l( m_ );      
            is_blocked_ = false; 
        }

        bool 
        is_blocked() const 
        { 
            lock_guard_type l( m_ );      
            return is_blocked_; 
        }

        void 
        swap( self_type& other )
        {
            std::lock( m_, other.m_); 
            lock_guard_type l1( m_, std::adopt_lock );      
            lock_guard_type l2( other.m_, std::adopt_lock );
            std::swap( pcst_, other.pcst_ );
            std::swap( is_blocked_, other.is_blocked_ );
        }

    };//struct signal

    template< typename R, typename... Arg >
    void 
    swap( signal<R,Arg...>& lhs, signal<R,Arg...>& rhs )
    { lhs.swap( rhs ); }

    template< typename R, typename... Arg >
    void 
    disconnect( signal<R, Arg...>& sig, const connection_type& con )
    { sig.disconnect( con ); }

    template< typename R, typename... Arg >
    void 
    disconnect( const connection_type& con, signal<R, Arg...>& sig )
    { sig.disconnect( con ); }

    template< typename R, typename... Arg, typename F >
    connection_type
    connect( signal<R, Arg...>& sig, F f, 
             const typename signal<R, Arg...>::priority_type w = std::numeric_limits<typename signal<R, Arg...>::priority_type>::max() )
    { return sig.connect( f, w ); }

    template< typename R, typename... Arg, typename F >
    connection_type
    connect( F f, signal<R, Arg...>& sig, 
             const typename signal<R, Arg...>::priority_type w = std::numeric_limits<typename signal<R, Arg...>::priority_type>::max() )
    { return sig.connect( f, w ); }


    template< typename R, typename... Args >
    struct scope_connection
    {
        typedef scope_connection                        self_type;
        typedef signal<R, Args...>                      signal_type;
        typedef typename signal_type::connection_type   connection_type;
        typedef typename signal_type::priority_type     priority_type;
        typedef typename signal_type::function_type     function_type;

        template< typename F >
        scope_connection( signal_type& sig, F&& f, 
                          const priority_type w = std::numeric_limits<priority_type>::max() ) : sig_(sig)
        { con_ = sig_.connect(std::forward<function_type>(f), w); }

        const connection_type
        connection() const 
        { return con_; }

        ~scope_connection()
        { sig_.disconnect(con_); }

        scope_connection( const self_type& ) = delete;
        self_type& operator = ( const self_type& ) = delete;

        private:
        signal_type& sig_;
        connection_type con_;
    };

}//namespace signal0x

#endif//_SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE

