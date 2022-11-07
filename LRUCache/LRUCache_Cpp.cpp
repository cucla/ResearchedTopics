#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <unordered_set>
#include <bitset>



template <typename Key, typename Val>
class LRUCache
{
public:
    LRUCache( int capacity = 100 ) : capacity{ capacity }
    {}

    Val get( Key key )
    {
        if( LOOKUP.count( key ) == 0 ) return -1;

        CACHE.splice( CACHE.begin(), CACHE, LOOKUP[key] );
        return CACHE.front().first;
    }

    void put( Key key, Val value )
    {
        if( LOOKUP.count( key ) > 0 )
        {
            CACHE.splice( CACHE.begin(), CACHE, LOOKUP[key] );
            CACHE.front().first = value;
        }
        else
        {
            if( CACHE.size() == capacity )
            {
                LOOKUP.erase( CACHE.back().second );
                CACHE.pop_back();
            }

            CACHE.emplace_front( value, key );
            LOOKUP[key] = CACHE.begin();
        }
    }

    int getSize()
    {
        return CACHE.size();
    }

    void clear()
    {
        CACHE.clear();
        LOOKUP.clear();
    }

private:
    int capacity;
    std::list<std::pair<Val, Key>> CACHE;
    std::unordered_map<Key, typename std::list<std::pair<Val, Key>>::iterator> LOOKUP;
};


template <typename Key, typename Val>
class _LruCacheFunctionWrapper
{
    struct CacheInfo
    {
        int hits;
        int misses;
        int maxsize;
        int currsize;
    };

public:
    _LruCacheFunctionWrapper( std::function<Val( Key )> func, int maxSize ) 
        : _wrapped{ func }
        , _cache{ maxSize }
        , _hits{ 0 }
        , _misses{ 0 }
        , _maxsize{ maxSize }
    {
    }

    template<typename... Args>
    Val operator()( Args... args )
    {
        auto res = _cache.get( args... );

        if( res == -1 )
        {
            ++_misses;
            res = _wrapped( args... );
            _cache.put( args..., res );
        }
        else
            ++_hits;

        return res;
    }

    CacheInfo getCacheInfo()
    {
        return CacheInfo{ _hits, _misses, _maxsize, _cache.getSize() };
    }

    void clearCache()
    {
        _cache.clear();
        _hits = 0;
        _misses = 0;
    }

private:
    std::function<Val( Key )> _wrapped;
    LRUCache<Key, Val> _cache;
    int _hits;
    int _misses;
    int _maxsize;
};


long long fib( int n )
{
    if( n < 2 )
        return n;
    return fib( n - 1 ) + fib( n - 2 );
}


int main()
{
    for ( auto i = 0; i < 16; ++i )
        std::cout << fib(i) << " ";

    LRUCache<int, std::string> cache(100);
    cache.put( 4, "44");
    cache.put( 5, "55");
    cache.put( 7, "77" );


    _LruCacheFunctionWrapper<int, long long> wrapper( &fib, 50 );

    for( auto i = 0; i < 16; ++i )
        std::cout << wrapper( i ) << " ";



    std::cout << "\n";
}
