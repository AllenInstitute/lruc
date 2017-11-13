#include <algorithm>
#include <cassert>
#include <list>
#include <sstream>

#include <tr1/unordered_map>

namespace lruc
{
	template < class Key, class T >
	class lru_cache
	{
		typedef std::pair<Key, T>								data_value_type;
		typedef std::list<data_value_type>						data_container;
		typedef typename data_container::iterator				data_iterator;

		typedef std::tr1::unordered_map<Key, data_iterator>		key_container;
		typedef typename key_container::const_iterator			key_iterator;


		size_t						_max_size;
		mutable data_container		_data;
		key_container				_keys;

		mutable size_t				_hit_count;
		mutable size_t				_miss_count;
		size_t						_insert_count;
		size_t						_evict_count;

	public:
		typedef typename data_container::const_iterator			data_const_iterator;

		explicit lru_cache(size_t max_size)
			: _max_size(max_size),
			_hit_count(0),
			_miss_count(0),
			_insert_count(0),
			_evict_count(0)
		{ assert(max_size); }


		void insert(const Key& key, const T& value)
		{
			const key_iterator key_iter = _keys.find(key);

			if (key_iter != _keys.end())
			{
				key_iter->second->second = value;

				_data.splice(_data.begin(), _data, key_iter->second);
			}
			else if (_data.size() == _max_size)
			{
				const data_value_type& value_iter = _data.back();

				_keys.erase(value_iter.first);
				_data.pop_back();

				++_evict_count;
			}

			_data.push_front(data_value_type(key, value));

			_keys[key] = _data.begin();

			++_insert_count;
		}


		data_const_iterator find(const Key& key) const
		{
			const key_iterator key_iter = _keys.find(key);

			if (key_iter == _keys.end())
			{
				++_miss_count;
				return end();
			}

			_data.splice(_data.begin(), _data, key_iter->second);

			++_hit_count;

			return key_iter->second;
		}


		inline bool contains(const Key& key) const { return _keys.count(key); }


		void erase(const Key& key)
		{
			const key_iterator& key_iter = _keys.find(key);

			if (key_iter == _keys.end())
				return;

			_data.erase(key_iter->second);
			_keys.erase(key_iter);

			++_evict_count;
		}


		void clear()
		{
			const size_t old_size = size();

			_keys.clear();
			_data.clear();

			_evict_count += old_size;
		}


		inline size_t hit_count()		const { return _hit_count; }
		inline size_t miss_count()		const { return _miss_count; }
		inline size_t insert_count()	const { return _insert_count; }
		inline size_t evict_count()		const { return _evict_count; }


		inline size_t max_size() const { return _max_size; }
		inline size_t size()	 const { return _keys.size(); }
		inline bool empty()		 const { return _keys.empty(); }


		inline data_const_iterator begin() const { return _data.begin(); }
		inline data_const_iterator end()   const { return _data.end(); }


		std::string to_string() const
		{
			std::stringstream lru_cache_string;

			lru_cache_string << "lru_cache: { address: " << this << ", max_size: " << max_size() <<
				", size: " << size() << ", hit_count: " << hit_count() << ", miss_count: " << miss_count() <<
					", insert_count: " << insert_count() << ", evict_count: " << evict_count() << " }\n";

			return lru_cache_string.str();
		}

	private:
		lru_cache(const lru_cache&);
		lru_cache& operator=(const lru_cache&);
	};
}
