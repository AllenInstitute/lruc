/* Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License. */


#include <algorithm>
#include <cassert>
#include <list>
#include <sstream>
#include <unordered_map>

namespace lruc
{
	template < class Key, class T >
	class lru_cache
	{
		typedef std::pair<Key, T>								data_value_type;
		typedef std::list<data_value_type>						data_container;
		typedef typename data_container::iterator				data_iterator;

		typedef std::unordered_map<Key, data_iterator>			key_container;
		typedef typename key_container::const_iterator			key_iterator;


		size_t						_max_size;
		mutable data_container		_data;
		key_container				_keys;

		mutable size_t				_hit_count;
		mutable size_t				_miss_count;
		size_t						_insert_count;
		size_t						_evict_count;

	public:
		typedef typename data_container::const_iterator			const_data_iterator;

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
			else
			{
				if (_data.size() == _max_size)
				{
					const data_value_type& value_iter = _data.back();

					_keys.erase(value_iter.first);
					_data.pop_back();

					++_evict_count;
				}

				_data.push_front(data_value_type(key, value));

				_keys[key] = _data.begin();
			}

			++_insert_count;
		}


		const_data_iterator find(const Key& key) const
		{
			const key_iterator key_iter = _keys.find(key);

			if (key_iter == _keys.end())
			{
				++_miss_count;
				return cend();
			}

			_data.splice(_data.begin(), _data, key_iter->second);

			++_hit_count;

			return key_iter->second;
		}


		bool contains(const Key& key) const { return _keys.count(key); }


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


		size_t hit_count()		const { return _hit_count; }
		size_t miss_count()		const { return _miss_count; }
		size_t insert_count()	const { return _insert_count; }
		size_t evict_count()	const { return _evict_count; }


		size_t max_size()		const { return _max_size; }
		size_t size()			const { return _keys.size(); }
		bool empty()			const { return _keys.empty(); }


		const_data_iterator cbegin() const { return _data.cbegin(); }
		const_data_iterator cend()   const { return _data.cend(); }


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
