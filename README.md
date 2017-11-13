# lruc
C++03 fast and lightweight LRU in-memory cache implementation.

# Build Requirements
* Compiler with [C++11](https://en.wikipedia.org/wiki/C%2B%2B11) standart support
* [CMake](https://cmake.org/) >= 3.1

# Complexity
<pre>
<b>lruc::lru_cache::insert</b>
<b>lruc::lru_cache::find</b>
<b>lruc::lru_cache::contains</b>
<b>lruc::lru_cache::erase</b>
</pre>
Amortized constant on average, worst case linear in the size of the cache.

<pre>
<b>lruc::lru_cache::clear</b>
</pre>
Linear in the size of the cache, i.e., the number of elements.

<pre>
<b>lruc::lru_cache::empty</b>
<b>lruc::lru_cache::size</b>
<b>lruc::lru_cache::max_size</b>
<b>lruc::lru_cache::hit_count</b>
<b>lruc::lru_cache::miss_count</b>
<b>lruc::lru_cache::insert_count</b>
<b>lruc::lru_cache::evict_count</b>
<b>lruc::lru_cache::hit_count</b>
<b>lruc::lru_cache::cbegin</b>
<b>lruc::lru_cache::cend</b>
<b>lruc::lru_cache::to_string</b>
</pre>
Constant.

# Usage
```cpp
using namespace lruc;

lru_cache<size_t, std::string> cache(42);

cache.insert(1, "Trent");
cache.insert(2, "Reznor");

const lru_cache<size_t, std::string>::const_data_iterator value_iter = cache.find(1);

if (cache.contains(2))
  std::cout << cache.to_string();
```
