#pragma once

#include <iostream>
#include <stdexcept> 

using namespace std;	

template<typename K, typename T>
struct Pair {
	K key;
	T value;
	Pair<K, T>* next;

	Pair<K, T>(const K& key, const T& value) : key(key), value(value), next(nullptr) {};
};





