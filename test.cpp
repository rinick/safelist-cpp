
#include <iostream>

#include <vector>
#include <cassert>
#include "safelist.h"

void test1();
void test2();

int main()
{

	test1();
	test2();

	std::cout << "all tests passed";
	std::cin.ignore();

	return 0;
}

/**
 * add and remove elements during iteration
 */
void test1() {

	SafeList<int> list;

	std::vector<std::list<int>::iterator> nodes;

	for (int i = 0; i < 10; ++i) {
		list.push_back(i);
		nodes.push_back(--list.end()); // store the iterator of current number
	}

	std::vector<size_t> removeOrder = { 0, 3, 2, 4, 7, 1, 9 };
	std::vector<int> expectedValues = { 0, 1, 2, 4, 5, 6, 8 };
	size_t expectedIdx = 0;
	for (auto it = list.it(); it.hasMore(); ++it) {
		assert(*it == expectedValues[expectedIdx]);
		list.erase(nodes[removeOrder[expectedIdx]]);
		list.push_back(expectedIdx + 10);
		expectedIdx++;
	}

	expectedIdx = 0;
	expectedValues = { 5, 6, 8, 10, 11, 12, 13, 14, 15, 16 };
	for (auto it = list.it(); it.hasMore(); ++it) {
		assert(*it == expectedValues[expectedIdx]);
		expectedIdx++;
	};

};

/**
 * add and remove elements during iteration in a different order
 */
void test2() {

	SafeList<int> list;

	std::vector<std::list<int>::iterator> nodes;

	for (int i = 0; i < 10; ++i) {
		list.push_back(i);
		nodes.push_back(--list.end()); // store the iterator of current number
	}

	std::vector<size_t> removeOrder = { 0, 3, 2, 4, 7, 9, 8 };
	std::vector<int> expectedValues = { 0, 1, 2, 4, 5, 6, 8 };
	size_t expectedIdx = 0;
	for (auto it = list.it(); it.hasMore(); ++it) {
		assert(*it == expectedValues[expectedIdx]);
		list.erase(nodes[removeOrder[expectedIdx]]);
		list.push_back(expectedIdx + 10);
		expectedIdx++;
	}

	expectedIdx = 0;
	expectedValues = { 1, 5, 6, 10, 11, 12, 13, 14, 15, 16 };
	for (auto it = list.it(); it.hasMore(); ++it) {
		assert(*it == expectedValues[expectedIdx]);
		expectedIdx++;
	};

};

