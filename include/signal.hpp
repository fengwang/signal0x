#ifndef _SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE
#define _SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE

#include <singleton.hpp>// for signal0x::singleton 

#include <cstddef>      // for std::ptrdiff_t
#include <functional>   // for std::function
#include <utility>      // for std::forward, std::move
#include <algorithm>    // for std::swap
#include <map>          // for std::map
#include <utility>      // for std::pair
#include <cstdint>      // for std::int_least64_t
#include <limits>       // for std::numeric_limits<T>::max()

namespace signal0x
{

    typedef std::ptrdiff_t                                  connection_type;

    template< typename R, typename... Args >
    struct signal
    {
        typedef std::ptrdiff_t                                  connection_type;
        typedef std::int_least64_t                              priority_type;
        typedef signal                                          self_type;
        typedef self_type                                       publisher_type;
        typedef std::function<R( Args... )>                     function_type;
        typedef function_type                                   subscriber_type;
        typedef function_type                                   slot_type;
        typedef std::map<connection_type, subscriber_type>      associate_connection_subscriber_type;
        typedef std::map<priority_type, associate_connection_subscriber_type> 
                                                                priority_connection_subscriber_type;
    private:
        bool                                                    is_blocked_;
        priority_connection_subscriber_type                     pcst_;

    public:
        signal() : is_blocked_(false) {}
        signal( const self_type& other ) : is_blocked_( other.is_blocked_ ), 
                                           pcst_( other.pcst_ ) {}
        signal( self_type&& other ) : is_blocked_( std::move(other.is_blocked_) ), 
                                      pcst_( std::move(other.pcst_) ) {}
        self_type& 
        operator = ( const self_type& other )
        {
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
        connect(const F& f, const priority_type w = std::numeric_limits<priority_type>::max() )
        {
            auto&c =  singleton<connection_type>::instance();
            (pcst_[w]).insert( std::make_pair( c, (function_type)(f) ) );
            return c++;
        }

        template< typename F >
        const connection_type
        connect(F&& f, const priority_type w = std::numeric_limits<priority_type>::max() )
        {
            auto&c =  singleton<connection_type>::instance();
            (pcst_[w]).insert( std::make_pair( c, (function_type)(std::forward<F>(f)) ) );
            return c++;
        }

        template< typename F >
        const connection_type
        connect(const priority_type w, F f)
        { return connect( f, w ); }

        void 
        disconnect( const connection_type& c )
        {
            for ( auto& i : pcst_ )
                    i.second.erase( c );
        }

        void 
        disconnect_all()
        { pcst_.clear(); }

        void 
        operator()( Args... args ) const
        { 
            if ( is_blocked_ ) return;
            for ( auto const & i : pcst_ )
                for ( auto const & j : i.second )
                    (j.second)( args... );
        }

        template<typename Output_Iterator>
        void
        emit( Output_Iterator o, Args... args ) const
        {
            if ( is_blocked_ ) return;
            for ( auto const & i : pcst_ )
                for ( auto const & j : i.second )
                    *o++ = (j.second)( args... );
        }

        void 
        block() 
        { is_blocked_ = true; }

        void 
        unblock()
        { is_blocked_ = false; }

        bool 
        is_blocked() const 
        { return is_blocked_; }

        void 
        swap( self_type& other )
        {
            std::swap( pcst_, other.pcst_ );
            std::swap( is_blocked_, other.is_blocked_ );
        }

    };//struct signal

    template< typename R, typename... Arg >
    void 
    swap( signal<R,Arg...>& lhs, signal<R,Arg...>& rhs )
    { lhs.swap( rhs ); }

}//namespace signal0x

#endif//_SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE

