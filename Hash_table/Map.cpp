#pragma once

#include <iostream>
#include <stdexcept> 
#define W sizeof(size_t) * 8
#define L 11
#define A 0.7

using namespace std;	

template<typename K, typename T>
struct Pair {
	K key;
	T value;
	Pair<K, T>* next;

	Pair<K, T>(const K& key, const T& value) : key(key), value(value), next(nullptr) {};
};

template<typename K, typename T>
class Map {
private:
	size_t _size;
	Pair<K, T>** _buskets;

	 size_t hash(const K& key) const {
		//The multiply-shift method
		size_t tmp = key * A;
		return ((tmp >> L) | (tmp << W - L)) % _size;
	}
	void copy(const Map<K, T>& other) {
		_size = other._size;
		_buskets = new Pair<K, T>*[_size];
		for (size_t i(0); i < _size; ++i) {
			_buskets[i] = nullptr;
		}
		for (size_t i(0); i < _size; ++i) {
			Pair<K, T>* tmp = other._buskets[i];
			while (tmp) {
				insert(tmp->key, tmp->value);
				tmp = tmp->next;
			}
		}
	}

public:
	/*Map<K, T>() {
		_size = 10;
		_buskets = new Pair<K, T>* [_size];
		for (size_t i(0); i < _size; ++i) {
			_buskets[i] = nullptr;
		}
	};*/
	Map<K, T>(size_t size=10) {
		if(!size) throw std::invalid_argument("Size is zero");
		_size = size;
		_buskets = new Pair<K, T>* [_size];
		for (size_t i(0); i < size; ++i) {
			_buskets[i] = nullptr;
		}
	}
	Map<K, T>(size_t size, unsigned seed) {
		_size = size;
		_buskets = new Pair<K, T>* [_size];
		for (size_t i(0); i < size; ++i) {
			_buskets[i] = nullptr;
		}

		srand(seed);
		size_t count = 0;
		while (count != 256) {
			unsigned x = unsigned(rand() % 256);
			if (!this->contains(x)) {
				this->insert(count, x);
				++count;
			}
		}
	}
	Map<K, T>(const Map<K, T>& other) {
		copy(other);
	}
	~Map<K, T>() {
		clear();
		delete[] _buskets;
	}

	Map<K, T>& operator=(const Map<K, T>& other) {
		if (&other == this) {
			return *this;
		}
		clear();
		delete[] _buskets;
		
		copy(other);
		return *this;
	}

	void clear() {
		Pair<K, T>* tmp = nullptr;
		for (size_t i(0); i < _size; ++i) {
			tmp = _buskets[i];
			while (tmp) {
				auto tmp_next = tmp->next;
				delete tmp;
				tmp = tmp_next;
			}
		}
	}

	void insert(const K& key, const T& value) const {
		size_t index = hash(key);
		auto tmp = _buskets[index];
		if (!tmp) {
			_buskets[index] = new Pair<K, T>(key, value);
			return;
		}
		while (tmp) {
			if (tmp->key == key) {
				throw std::invalid_argument("This key already in map");
				return;
			}
			if (tmp->next) {
				tmp = tmp->next;
			}
			else {
				tmp->next = new Pair<K, T>(key, value);
				return;
			}
		}
	}
	bool insert_or_assign(const K& key, const T& value) const {
		size_t index = hash(key);
		auto tmp = _buskets[index];
		if (!tmp) {
			_buskets[index] = new Pair<K, T>(key, value);
			return true;
		}
		while (tmp) {
			if (tmp->key == key) {
				tmp->value = value;
				return false;
			}
			if (tmp->next) {
				tmp = tmp->next;
			}
			else {
				tmp->next = new Pair<K, T>(key, value);
				return true;
			}
		}
	}

	bool contains(const T& value) const {
		for (size_t index(0); index < _size; ++index) {
			auto tmp = _buskets[index];
			while (tmp) {
				if (tmp->value == value) {
					return true;
				}
				tmp = tmp->next;
			}
		}
		return false;
	}

	T* search(const K& key) { // std::optional<std::reference_wrapper<T>>
		size_t index = hash(key);
		auto tmp = _buskets[index];
		if (tmp == nullptr) {
			return nullptr;
		}
		while (tmp) {
			if (tmp->key == key) {
				return &tmp->value;
			}
			tmp = tmp->next;
		}
		return nullptr;
	}

	const T* search(const K& key) const {
		return const_cast<Map*>(this)->search(key);
	}

	bool erase(const K& key) {
		size_t index = hash(key);
		auto tmp = _buskets[index];
		if (tmp == nullptr) {
			return false;
		}
		if (tmp->key == key) {
			_buskets[index] = tmp->next;
			delete tmp;
			return true;
		}
		Pair<K, T>* prev = nullptr;
		while (tmp) {
			if (tmp->key == key) {
				prev->next = tmp->next;
				delete tmp;
				return true;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		return false;
	}

	int count(const K& key) const {
		size_t index = hash(key);
		auto tmp = _buskets[index];
		while (tmp) {
			if (tmp->key == key) {
				return 1;
			}
			tmp = tmp->next;
		}
		return 0;
	}

	void print() const {
		Pair<K, T>* tmp = nullptr;
		for (size_t i(0); i < _size; ++i) {
			tmp = _buskets[i];
			while (tmp) {
				std::cout << tmp->key << " : " << tmp->value << " ";
				tmp = tmp->next;
			}
			std::cout<<endl;
		}
	}
};

char pearson_hash(const Map<char, char>& map, const std::string& str) {
	char hash = 0;
	for (char c : str) {
		hash = *map.search((hash ^ (unsigned)c));
	}
	return hash;
}

bool is_equal(const std::string& left, const std::string& right) {
	const static auto map = Map<char, char>(16, 1655);
	if (pearson_hash(map, left) == pearson_hash(map, right)) {
		return true;
	}
	else {
		return false;
	}
}

