# lruc
C++03 fast and lightweight LRU in-memory cache implementation.

# Build Requirements
* C++03 compiler with [TR1](https://en.wikipedia.org/wiki/C%2B%2B_Technical_Report_1) support (for using [std::tr1::unordered_map<>](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2003/n1456.html))
* [CMake](https://cmake.org/) >= 2.8

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
<b>lruc::lru_cache::begin</b>
<b>lruc::lru_cache::end</b>
<b>lruc::lru_cache::to_string</b>
</pre>
Constant.

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
