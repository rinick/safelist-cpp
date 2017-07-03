#pragma once

#include <list>

template<class T>
class SafeIterator;

template<class T>
class SafeList;

template<class T>
class SafeList_Base
	: public std::list<T> {
	friend class SafeIterator<T>;
protected:
	SafeIterator<T> * safeIter = nullptr;

};

template<class T>
class SafeIterator {
	friend class SafeList<T>;
	typedef typename std::list<T>::iterator iterator;
	typedef typename std::list<T>::reference reference;

	iterator iterCurrent;
	iterator iterNext;
	iterator iterEnd;
	bool isLast;

	SafeList_Base<T> * pList;

public:
	SafeIterator(SafeList_Base<T> & list) :
		pList(&list),
		iterCurrent(list.begin()),
		iterNext(list.begin()),
		iterEnd(list.end()),
		isLast(false)
	{
		if (list.safeIter) {
			throw "cocurrent iteration";
		}
		if (list.empty()) {
			pList = nullptr;
		}
		else {
			iterEnd--;
			iterNext++;
			list.safeIter = this;
		}
	}

	SafeIterator(SafeIterator& original) :
		pList(original.pList),
		iterCurrent(original.iterCurrent),
		iterNext(original.iterNext),
		iterEnd(original.iterEnd),
		isLast(original.isLast) {
		std::cout << "cp\n";
		if (pList) {
			original.done();
			pList->safeIter = this;
		}
	}

	reference operator*() const {
		return *iterCurrent;
	}

	SafeIterator& operator++()
	{
		if (pList) {
			if (isLast) {
				done();
			}
			else {
				iterCurrent = iterNext;
				if (iterNext == iterEnd) {
					isLast = true;
				}
				else {
					iterNext++;
				}
			}
		}
		return (*this);
	}

	void done() {
		pList->safeIter = nullptr;
		pList = nullptr;
	}

	iterator & erase() {
		pList->erase(iterCurrent);
		return iterCurrent;
	}

	bool hasMore() const {
		// pList will be set to null when the iteration end
		return pList;
	}

	~SafeIterator() {
		if (pList) {
			pList->safeIter = nullptr;
		}
	}
};

template<class T>
class SafeList
	: public SafeList_Base<T> {
	typedef typename std::list<T>::iterator iterator;
public:
	SafeIterator<T> it() {
		return SafeIterator<T>(*this);
	}


	void erase(iterator it) {
		if (safeIter && !safeIter->isLast) {
			if (safeIter->pList && safeIter->iterNext == it) {
				if (safeIter->iterNext == safeIter->iterEnd) {
					safeIter->done();
					SafeList_Base::erase(it);
					return;
				}
				else {
					safeIter->iterNext++;
				}
			}
			if (safeIter->iterEnd == it) {
				if (safeIter->iterNext == safeIter->iterEnd) {
					safeIter->done();
					SafeList_Base::erase(it);
					return;
				}
				else {
					safeIter->iterEnd--;
				}
			}
		}
		SafeList_Base::erase(it);
	}
};
