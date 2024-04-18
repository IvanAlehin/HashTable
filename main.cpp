
#include <iostream>
#include "Hash_table/Map.cpp"

using namespace std;

int main() {
	Map<int, int> map;
	map.insert(1, 123124);
	map.insert(21, 2);
	map.insert(3, 3);
	map.insert(6, 4);
	map.print();
	cout << endl<<endl;

	map.insert_or_assign(31, 5);
	map.insert_or_assign(1, 6);

	map.print();
	cout << endl << endl;

	map.erase(6);

	map.print();
	cout << endl << endl;

    cout << is_equal("world", "world");
    return 0;

}
