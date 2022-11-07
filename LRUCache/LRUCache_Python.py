# PYTHON

import functools
import timeit
from collections import OrderedDict

def fib(n):
    if n < 2:
        return n
    return fib(n-1) + fib(n-2)
    
[fib(n) for n in range(16)]

setup_code = "from __main__ import fib"
times = timeit.repeat(setup=setup_code, stmt='[fib(n) for n in range(16)]', repeat=10, number=10)

print(f"Minimum execution time: {min(times)}")

@functools.lru_cache(maxsize=100)
def fib2(n):
    if n < 2:
        return n
    return fib(n-1) + fib(n-2)
    
setup_code = "from __main__ import fib2"
times = timeit.repeat(setup=setup_code, stmt='[fib2(n) for n in range(16)]', repeat=10, number=10)

print(f"Minimum execution time: {min(times)}")

# https://jellis18.github.io/post/2021-11-25-lru-cache/

class LruCache:
    def __init__(self, capacity):
        self.capacity = capacity
        self.__cache = OrderedDict()

    def get(self, key):
        if key not in self.__cache:
            return None
        self.__cache.move_to_end(key)
        return self.__cache[key]

    def insert(self, key, value):
        if len(self.__cache) == self.capacity:
            self.__cache.popitem(last = False)
        self.__cache[key] = value
        self.__cache.move_to_end(key)

    def __len__(self):
        return len(self.__cache)

    def clear(self):
        self.__cache.clear()
        
        
        
class CacheInfo:
    hits: int
    misses: int
    maxsize: int
    currsize: int
    
    
        
class _LruCacheFunctionWrapper:
    def __init__(self, func, maxSize):
        self.__wrapped__ = func
        self.__cache = LruCache(maxSize)
        self.__hits = 0
        self.__misses = 0
        self.__maxsize = maxSize
    
    def __call__(self, *args, **kwargs):
        call_args = args + tuple(kwargs.items())

        ret = self.__cache.get(call_args)

        if ret is None:
            self.__misses += 1
            ret = self.__wrapped__(*args, **kwargs)
            self.__cache.insert(call_args, ret)
        else:
            self.__hits += 1

        return ret
    
    def cache_info(self) -> CacheInfo:
        return CacheInfo(
            hits=self.__hits,
            misses=self.__misses,
            currsize=len(self.__cache),
            maxsize=self.__maxsize,
        )

    def cache_clear(self) -> None:
        self.__cache.clear()
        self.__hits = 0
        self.__misses = 0
        
        
wrapped = _LruCacheFunctionWrapper(fib, 4)
x = wrapped(3)
x



