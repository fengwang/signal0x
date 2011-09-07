#ifndef _SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE
#define _SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE

#include <singleton.hpp>        // for signal0x::singleton 
#include <chain_function.hpp>   // for signal0x::chain_function
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
        typedef std::ptrdiff_t                                                  connection_type;
        typedef int                                                             priority_type;
        typedef signal                                                          self_type;
        typedef self_type                                                       publisher_type;
        typedef std::function<R( Args... )>                                     function_type;
        typedef function_type                                                   subscriber_type;
        typedef function_type                                                   slot_type;
        typedef std::map<connection_type, subscriber_type>                      associate_connection_subscriber_type;
        typedef std::map<priority_type, associate_connection_subscriber_type>   priority_connection_subscriber_type;
        typedef std::mutex                                                      mutex_type;
        typedef std::lock_guard<mutex_type>                                     lock_guard_type;

    private://lock to get a consistent snapshot of the source
        signal( const self_type& other, const lock_guard_type& ): pcst_blocked_( other.pcst_blocked_ ), pcst_( other.pcst_ ) {}
        signal( self_type&& other, const lock_guard_type& ): pcst_blocked_( std::move(other.pcst_blocked_) ), pcst_( std::move(other.pcst_) ) {}
    public:
        signal( const self_type& other ) : signal( other, lock_gurad_type(other.m_ ) ) {}
        signal( self_type&& other ) : signal( other, lock_gurad_type(other.m_ ) ) {}
        signal() {}
        
        self_type& 
        operator = ( const self_type& other )
        {   // 1)  lock both mutexes safely 
            std::lock( m_, other.m_ ); 
            // 2)  adopt the ownership into the std::lock_guard instances to ensure the locks are released safely at the end of the function.
            lock_guard_type l1( m_, std::adopt_lock );      
            lock_guard_type l2( other.m_, std::adopt_lock );
            pcst_blocked_ = other.pcst_blocked_;
            pcst_ = other.pcst_;
            return *this;
        }

        self_type& 
        operator = ( self_type&& other )
        {
            lock_guard_type l( m_ );      
            pcst_blocked_ = std::move(other.pcst_blocked_);
            pcst_ = std::move(other.pcst_);
            return *this;
        }

        template< typename... F >
        const connection_type
        connect( const priority_type w, const F&... f )
        { 
            auto ff =  signal0x_private::chain_function<R, Args...>()(f...);    //generate a function chain
            auto& c =  signal0x_private::singleton<connection_type>::instance();//a global id generator to do connection representation 
            lock_guard_type l( m_ );                                            
            (pcst_[w]).insert( std::make_pair( c, ff ) );                       //save connection id and chain function to the table
            return c++;                                                         //increase global id generator
        }

        template< typename... F >
        const connection_type
        connect(  const F&... f ) //if priority parameter not given, set it to the lowest
        { return connect( std::numeric_limits<priority_type>::max(), f... ); }

        void 
        disconnect( const connection_type& c )
        {
            lock_guard_type l( m_ );      
            for ( auto& i : pcst_ )//try move    //check cavity       //erase if empty      //break
                    if ( i.second.erase( c ) ) { if(!i.second.size()) pcst_.erase(i.first); return; }
            for ( auto& i : pcst_blocked_ ) 
                    if ( i.second.erase( c ) ) { if(!i.second.size()) pcst_blocked_.erase(i.first); return; }
        }

        void 
        disconnect_all()
        { 
            lock_guard_type l( m_ );      
            pcst_.clear(); 
            pcst_blocked_.clear(); 
        }

        void 
        operator()( Args... args ) const
        { 
            lock_guard_type l( m_ );      
            for ( auto const & i : pcst_ ) //invoking functions according to their priorities
                for ( auto const & j : i.second ) //if of same priority, invoking randomly
                    try { (j.second)( args... ); } //in case of bad function call, such like null ptr, just skip 
                    catch( std::bad_function_call& bfc ) {} 
        }

        void 
        emit( Args... args ) const
        { operator()(args...); }

        template<typename Output_Iterator>
        void
        operator() ( Output_Iterator o, Args... args ) const
        {
            lock_guard_type l( m_ );      
            for ( auto const & i : pcst_ )
                for ( auto const & j : i.second )
                    try { *o++ = (j.second)( args... ); }//redirect the return values to a stream
                    catch( std::bad_function_call& bfc ) {}
        }

        bool 
        block( const connection_type con )
        {
            lock_guard_type l( m_ );      
            for ( auto i : pcst_ )
                for ( auto j : i.second )
                {  
                    if ( j.first != con ) continue; 
                    pcst_blocked_[i.first].insert( j ); //copy the connection to the blocked group
                    pcst_[i.first].erase( con );        //erase the connection from the normal group
                    return true; 
                }
            return false;
        }

        bool 
        unblock( const connection_type con )
        {
            lock_guard_type l( m_ );      
            for ( auto i : pcst_blocked_ )
                for ( auto j : i.second )
                {  
                    if ( j.first != con ) continue;
                    pcst_[i.first].insert( j );         //copy the connection to the normal group
                    pcst_blocked_[i.first].erase( con );//erase the connection from the blocked group
                    return true; 
                }
            return false;
        }

        void 
        swap( self_type& other )
        {
            std::lock( m_, other.m_ ); 
            lock_guard_type l1( m_, std::adopt_lock );      
            lock_guard_type l2( other.m_, std::adopt_lock );
            std::swap( pcst_, other.pcst_ );                //swap the normal group members    
            std::swap( pcst_blocked_, other.pcst_blocked_ );//swap the blocked group members
        }

    private:
        priority_connection_subscriber_type                     pcst_blocked_; //the blocked group
        priority_connection_subscriber_type                     pcst_;         //the normal group 
        mutable mutex_type                                      m_;            //mutex 
    };//struct signal

    template< typename R, typename... Arg >
    void //swap two signals
    swap( signal<R,Arg...>& lhs, signal<R,Arg...>& rhs )
    { lhs.swap( rhs ); }

    template< typename R, typename... Arg >
    void //erase a connection from a signal
    disconnect( signal<R, Arg...>& sig, const connection_type& con )
    { sig.disconnect( con ); }

    template< typename R, typename... Arg >
    void //overload disconnection
    disconnect( const connection_type& con, signal<R, Arg...>& sig )
    { sig.disconnect( con ); }

    template<typename S, typename... F>
    connection_type //connect functions f to a signal s, with priority w
    connect( const typename S::priotity_type w, S& s, const F&... f )
    { return s.connect( w, f... ); }

    template<typename S, typename... F>
    connection_type //connect functions f to a signal s, with lowest priority
    connect(  S& s, const F&... f )
    { return connect( std::numeric_limits<typename S::priority_type>::max(), s, f... ); }

    template< typename R, typename... Args >
    struct scope_connection
    {
        typedef scope_connection                        self_type;
        typedef signal<R, Args...>                      signal_type;
        typedef typename signal_type::connection_type   connection_type;
        typedef typename signal_type::priority_type     priority_type;
        typedef typename signal_type::function_type     function_type;

        template<typename... F>
        scope_connection( const priority_type w, signal_type& sig,  const F&... f ) : sig_(sig)
        { con_ = sig_.connect( w, f...); }

        template<typename... F>
        scope_connection( signal_type& sig,  const F&... f ) : sig_(sig)
        { con_ = sig_.connect( std::numeric_limits<priority_type>::max(), f...); }

        const connection_type connection() const { return con_; } 

        ~scope_connection() { sig_.disconnect(con_); }          //disconnect on leaving scope

        scope_connection( const self_type& ) = delete;          //no copy ctor
        self_type& operator = ( const self_type& ) = delete;    //no assignment operator

        private:
        signal_type& sig_;    
        connection_type con_;  
    };

}//namespace signal0x

#endif//_SIGNAL_HPP_INCLUDED_SOIU498ASDFKLSFDLKJXCVKJDSLKFJASLKJ438721SDP230SFAJHEIOUFDJLKSAFLKSAFOIU3498YSFDUIWEIUOWOIUE

