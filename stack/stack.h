/**
 * @file stack.h
 * Implements a stack class
 * @author Givemetips, 2017
 */


#pragma once

#include <fstream>
#include <string>
#include <deque>

/**
 * Macro for check state of stack
 * @return [description]
 */

namespace my_stl {
/**
 *  stack class
 */
template <class T, class Container = std::deque<T> > class stack {
private:
	/**
	 * container of elements
	 */
	Container container_;

public:
	
	/**
	 * constructor
	 */
	stack() {
		container_.clear();
	}

	/**
	 * get the top element of stack
	 * @return last element in stack
	 */
	T top() const {
		if (empty()) {
			
			T item;
			return item;
		} else {

			return container_.back();
		}
	}

	/**
	 * pop last element out of stack
	 */
	void pop() {
		if (container_.size() != 0) container_.pop_back();
	}

	/**
	 * push element in stack
	 * @param elem element to push
	 */
	void push(const T& elem) {
		container_.push_back(elem);
	}

	/**
	 * check if stack is empty
	 * @return True if size == 0, False otherwise
	 */
	bool empty() const {
		return container_.empty();
	}

	/**
	 * return current size of stack
	 * @return size_t 
	 */
	std::size_t size() const {
		return container_.size(0);
	}

};

}//stck namespace ended