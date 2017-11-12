#include "lruc/lru_cache.hpp"

#include "gtest/gtest.h"

using namespace lruc;

TEST(LruCacheTest, Init)
{
	lru_cache<int, int> cache(42);

	ASSERT_EQ(cache.max_size(), 42);
	ASSERT_EQ(cache.size(), 0);
	ASSERT_EQ(cache.hit_count(), 0);
	ASSERT_EQ(cache.miss_count(), 0);
	ASSERT_EQ(cache.insert_count(), 0);
	ASSERT_EQ(cache.evict_count(), 0);

	EXPECT_TRUE(cache.empty());
}

TEST(LruCacheTest, Insert)
{
	lru_cache<int, int> cache(1);

	cache.insert(1, 3);

	const lru_cache<int, int>::data_const_iterator value_iter = cache.find(1);

	EXPECT_TRUE(cache.contains(1));

	ASSERT_NE(value_iter, cache.end());
	ASSERT_EQ(value_iter->second, 3);

	ASSERT_EQ(cache.size(), 1);
	ASSERT_EQ(cache.insert_count(), 1);
	ASSERT_EQ(cache.hit_count(), 1);
}

TEST(LruCacheTest, Clear)
{
	lru_cache<int, int> cache(3);

	cache.insert(1, 1);
	cache.insert(2, 2);
	cache.insert(3, 3);

	cache.clear();

	EXPECT_TRUE(cache.empty());
	EXPECT_FALSE(cache.size());

	ASSERT_EQ(cache.evict_count(), 3);
}

TEST(LruCacheTest, Erase)
{
	lru_cache<int, int> cache(3);

	cache.insert(1, 1);
	cache.insert(2, 2);
	cache.insert(3, 3);

	cache.erase(2);

	EXPECT_FALSE(cache.contains(2));

	ASSERT_EQ(cache.find(2), cache.end());
	ASSERT_EQ(cache.miss_count(), 1);
	ASSERT_EQ(cache.evict_count(), 1);
}

TEST(LruCacheTest, Size)
{
	lru_cache<int, int> cache(3);

	cache.insert(1, 1);
	cache.insert(2, 2);

	ASSERT_EQ(cache.size(), 2);

	cache.erase(2);

	ASSERT_EQ(cache.size(), 1);

	cache.clear();

	EXPECT_FALSE(cache.size());
}

TEST(LruCacheTest, Contains)
{
	lru_cache<int, int> cache(2);

	cache.insert(1, 1);

	EXPECT_TRUE(cache.contains(1));
	EXPECT_FALSE(cache.contains(7));
}

TEST(LruCacheTest, Find)
{
	lru_cache<int, int> cache(1);

	cache.insert(1, 3);

	const lru_cache<int, int>::data_const_iterator value_iter = cache.find(1);

	EXPECT_TRUE(cache.contains(1));
	EXPECT_FALSE(cache.contains(2));

	ASSERT_EQ(cache.find(2), cache.end());
	ASSERT_NE(value_iter, cache.end());

	ASSERT_EQ(value_iter->first, 1);
	ASSERT_EQ(value_iter->second, 3);

	ASSERT_EQ(cache.miss_count(), 1);
	ASSERT_EQ(cache.hit_count(), 1);
}

TEST(LruCacheTest, Evict)
{
	lru_cache<int, int> cache(2);
	lru_cache<int, int>::data_const_iterator value_iter = cache.end();

	cache.insert(1, 1);
	cache.insert(2, 2);

	value_iter = cache.find(1);
	ASSERT_NE(value_iter, cache.end());
	ASSERT_EQ(value_iter->second, 1);

	cache.insert(3, 3);
	EXPECT_FALSE(cache.contains(2));
	ASSERT_EQ(cache.find(2), cache.end());

	cache.insert(4, 4);
	EXPECT_FALSE(cache.contains(1));
	ASSERT_EQ(cache.find(1), cache.end());

	value_iter = cache.find(3);
	ASSERT_NE(value_iter, cache.end());
	ASSERT_EQ(value_iter->second, 3);

	value_iter = cache.find(4);
	ASSERT_NE(value_iter, cache.end());
	ASSERT_EQ(value_iter->second, 4);

	ASSERT_EQ(cache.evict_count(), 2);
}

TEST(LruCacheTest, ToString)
{
	lru_cache<int, int> cache(2);

	cache.insert(1, 1);
	cache.insert(2, 2);
	cache.insert(3, 3);

	cache.find(2);
	cache.find(3);

	std::stringstream lru_cache_string;
	lru_cache_string << "lru_cache: { address: " << &cache << ", max_size: " << cache.max_size() <<
		", size: " << cache.size() << ", hit_count: " << cache.hit_count() << ", miss_count: " << cache.miss_count() <<
			", insert_count: " << cache.insert_count() << ", evict_count: " << cache.evict_count() << " }\n";

	ASSERT_EQ(cache.to_string(), lru_cache_string.str());
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
