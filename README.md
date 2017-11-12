# lruc
C++03 LRU cache implementation

# Build Requirements
* C++03 compiler with [TR1](https://en.wikipedia.org/wiki/C%2B%2B_Technical_Report_1) support (for using [std::tr1::unordered_map<>](http://www.cplusplus.com/reference/unordered_map/unordered_map/))
* [CMake](https://cmake.org/) >= 2.8

# Usage
```cpp
using namespace lruc;

lru_cache<size_t, std::string> cache(42);

cache.insert(1, "Trent");
cache.insert(2, "Reznor");

const lru_cache<size_t, std::string>::data_const_iterator value_iter = cache.find(1);

if (cache.contains(2))
  std::cout << cache.to_string();
```
