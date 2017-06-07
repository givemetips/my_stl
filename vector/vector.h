#pragma once

#include <cstdlib>
#include "allocator.h"

namespace my_stl {


template <class T, class TAllocator = DefaultAllocator<T> > 
class vector {
	private:
		using value_type = T;
		using size_type = size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = T*;
		using const_iterator = const iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>; 

		TAllocator allocator;

		T* data_;

		size_type current_size_;

		size_type capacity_;

		const int MULTIPLY_CONST = 2;
		
		/**
		 * Swap content of vectors.
		 * @param other vector to be swapped
		 */
		void swap(vector& other) noexcept {
			std::swap(data_, other.data_);
			std::swap(current_size_, other.current_size_);
			std::swap(capacity_, other.capacity_);
		}

		/**
		 * Increase capacity of the vector;
		 */
		void grow() {

			if (capacity()) {

				reserve(capacity() * MULTIPLY_CONST);

			}	else {
	
				reserve(1);

			}
		}

		/**
		 * Operator new overloading for constructing at specific memory position. 
		 * @param  size Size of element.
		 * @param  mem  Pointer to a memory.
		 * @return      Pointer to a memory where element will be constructed.
		 */
		void* operator new (size_t size, void* mem) {
			return mem;
		}

	public:

		/**
		 * Default constructor
		 */
		vector() : 
			data_(nullptr), 
			current_size_(0), 
			capacity_(0)
			{} ;
		
		/**
		 * Construct vector of count elements.
		 * @param  count Number of elements to construct;
		 * @param  value Values to be filled.
		 */
		explicit vector(size_type count, const T& value = T() ) : 
			data_(new T[count]),
			current_size_(count),
			capacity_(count) {
		
				for (size_type i = 0; i < count; ++i) {
					data_[i] = value;
				}

		}

		/**
		 * Construct vector out of another vector.
		 */
		vector(const vector& other) :
			data_(new T[other.size()]),  
			current_size_(other.size()), 
			capacity_(other.size()) {

				for (size_t i = 0; i < other.size(); ++i) {
					data_[i] = other.data_[i];
				}

		}

		/**
		 * Move constructor.
		 */
		vector(vector&& other) : 
			data_(other.data_), 
			current_size_(other.current_size_), 
			capacity_(other.capacity_) 
			{	
				other.data_ = nullptr;
				other.current_size_ = 0;
				other.capacity_ = 0;
			};

		/**
		 * Destructor.
		 */
		~vector() {
			for (size_t i = 0; i < size(); ++i) {
				allocator.destroy(data_ + i);
			}

			allocator.deallocate(data_);

			current_size_ = 0;
			capacity_ = 0;
		}

		/**
		 * Operator = overloading.
		 */
		vector& operator=(const vector& other) {

			if (&other == this) return *this;
			
			~vector();
			new (this) vector(other);
		
		};

		/**
		 * Move semantics for operator = overloading.
		 */
		vector& operator=(vector&& other) {
			
			if (this != &other) {

				vector tmp;

				std::move(tmp.data_, other.data_);
				std::move(tmp.capacity_, other.capacity_);
				std::move(tmp.current_size_, other.current_size_);

				swap(tmp);
			}

			return *this;

		} 
		
		/**
		 * Reserve memory for new_capacity elements. Does nothing if new_capacity is less than old capacity,
		 * otherwise reserves memory for new_capacity elements.
		 * @param new_capacity New capacity of the vector.
		 */
		void reserve(const size_type& new_capacity) {

			if (new_capacity <= capacity()) {
				return;
			}

			T* proxy_data = allocator.allocate(new_capacity);

			size_t pos = 0;
			
			try {
				
				while (pos < size()) {
					allocator.construct(&proxy_data[pos], data_[pos]);
					++pos;
				}	
			
			} catch (...) {

				for (int i = static_cast<int>(pos) - 1; i >= 0; --i) {
					allocator.destroy(proxy_data + i);
				}
				
				allocator.deallocate(proxy_data);

				throw;
			}

			vector proxy_vector;

			swap(proxy_vector);

			data_ = proxy_data;
			current_size_ = proxy_vector.size();
			capacity_ = new_capacity;
		}

		/**
		 * Assign count elements to a specific value. If count > capacity(); New memory will be allocated
		 * @param count New capacity and size of the vector.
		 * @param value Value to be filled.
		 */
		void assign(size_type count, const T& value) {

			if (count <= capacity()) {

				for (size_t pos = 0; pos < count; ++pos) {
					data_[pos] = value;
				}

			} else {

				reserve(count);
	
				for (size_t pos = 0; pos < count; ++pos) {
					data_[pos] = value;
				}				
			
			}

			current_size_ = count;

		}

		/**
		 * Safe realization of operator[]. Will throw std::out_of_range() exception if pos is invalid.
		 * @param  pos Index of the element to be accessed.
		 * @return     Reference to the elemet.
		 */
		reference at(size_type pos) {
			if (pos <= current_size_) return data_[pos];
			else {
				throw std::out_of_range("Index is out of range");
			}
		}

		/**
		 * Safe realization of operator[]. Will throw std::out_of_range() exception if pos is invalid.
		 * @param  pos Index of the element to be accessed.
		 * @return     Const reference to the elemet.
		 */
		const_reference at(size_type pos) const {
			if (pos <= current_size_) return data_[pos];
			else {
				throw std::out_of_range("Index is out of range");
			}
		}

		/**
		 * Operator[] overloading. Does not throw any exception if pos is invalid.
		 * @param  pos Index of the element to be accessed.
		 * @return     Reference to the elemet.
		 */
		reference operator[](size_type pos) {
			return data_[pos];
		}

		/**
		 * Operator[] overloading. Does not throw any exception if pos is invalid.
		 * @param  pos Index of the element to be accessed.
		 * @return     Const reference to the elemet.
		 */
		const_reference operator[](size_type pos) const {
			return data_[pos];
		}

		/**
		 * Accessing first element of the vector.
		 * @return Reference to the first element of the vector.
		 */
		reference front() {
			if (data_ && current_size_) return data_[0];
			else throw std::out_of_range("Can't access front element of empty vector");
		}

		/**
		 * Accessing first element of the vector.
		 * @return Const reference to the first element of the vector.
		 */
		const_reference front() const {
			if (data_ && current_size_) return data_[0];
			else throw std::out_of_range("Can't access front element of empty vector");
		}

		/**
		 * Accessing last element of the vector.
		 * @return Reference to the last element of the vector.
		 */
		reference back() {
			if (data_ && current_size_) return data_[current_size_ - 1];
			else throw std::out_of_range("Can't access back element of empty vector");
		}

		/**
		 * Accessing last element of the vector.
		 * @return Const reference to the last element of the vector.
		 */
		const_reference back() const {
			if (data_ && current_size_) return data_[current_size_ - 1];
			else throw std::out_of_range("Can't access back element of empty vector");
		}

		/**
		 * Check whether vector is empty.
		 * @return True if size == 0, Else otherwise.
		 */
		bool empty() const {
			return current_size_ == 0;
		}

		/**
		 * Get current size of the vector.
		 * @return Size of the vector.
		 */
		size_type size() const {
			return current_size_;
		}

		/**
		 * Get current capacity of the vector.
		 * @return Capacity of the vector.
		 */
		size_type capacity() const {
			return capacity_;
		}

		/**
		 * Shrinks vector's capacity to it's size.
		 */
		void shrink_to_fit() {

			if (capacity() == size()) return;

			T* proxy_data = allocator.allocate(size());

			size_t pos = 0;
			
			try {

				while (pos < size()) {
					allocator.construct(proxy_data[pos], data_[pos]);
				}

			} catch (...) {

				for (int i = static_cast<int>(pos) - 1; i >= 0; --i) {
					allocator.destroy(proxy_data[pos]);
				}
				
				allocator.deallocate(proxy_data);

				throw;

			}

			vector proxy_vector;

			swap(proxy_vector);

			data_ = proxy_data;
			current_size_ = proxy_vector.size();
			capacity_ = proxy_vector.capacity();
		}

		/**
		 * Clear vector's content. Capacity remains unchanged.
		 */
		void clear() {

			assign(capacity(), T());
			current_size_ = 0;
		
		}

		/**
		 * Append value to the end of vector.
		 * @param value Value to be appended.
		 */
		void push_back(const T& value) {

			if (size() == capacity()) {
				grow();
			}

			data_[size()] = value;
			++current_size_;

		}

		/**
		 * Construct object with value at the end of vector.
		 * @param value Value to be constructed.
		 */
		template<typename... U>
		void emplace_back(U&&... value) {

			if (size() == capacity()) {
				grow();
			}

			allocator.construct(data_[size()], std::forward<U>(value)...);
			++current_size_;

		}

		/**
		 * Remove last element of the vector. Throws std::out_of_range exception if vector is empty. 
		 */
		void pop_back() {
			
			if (size()) {
				--current_size_;
			} else {
				throw std::out_of_range("Cant pop last element of empty vector");
			}

		}

		/**
		 * Set size and capacity of the vector to count. Fill it with given value.
		 * @param count New size and capacity of the vector.
		 * @param value Fill vector's content with given value.
		 */
		void resize(size_type count, const value_type& value = T()) {
			
			for (size_t pos = 0; pos < count; ++pos) {
				allocator.destroy(data_ + pos);
			}
			
			allocator.deallocate(data_);

			data_ = allocator.allocate(count);
			
			for (size_t pos = 0; pos < count; ++pos) {
				allocator.construct(data_ + pos, value);
			}

			current_size_ = count;
			capacity_ = count;
			
		}	

		/**
		 * Pointer to the first element.
		 * @return Pointer to the first element.
		 */
		iterator begin() {
			return data_;
		}

		/**
		 * Pointer to the first element/
		 * @return Const pointer.
		 */
		const_iterator begin() const {
			return data_;
		}

		/**
		 * Pointer to the first element/
		 * @return Const pointer.
		 */
		const_iterator cbegin() const {
			return data_;
		}

		/**
		 * Pointer to the next to last element.
		 * @return Pointer to the next to last element.
		 */
		iterator end() {
			return (data_ + size());
		}

		/**
		 * Pointer to the next to last element.
		 * @return Const pointer to the next to last element.
		 */
		const_iterator end() const {
			return (data_ + size());
		}

		/**
		 * Pointer to the next to last element.
		 * @return Const pointer to the next to last element.
		 */
		const_iterator cend() const {
			return (data_ + size());
		}

		reverse_iterator rend() {
			return reverse_iterator(data_);
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(data_);
		}

		const_reverse_iterator crend() const {
			return const_reverse_iterator(data_);
		}

		reverse_iterator rbegin() {
			return reverse_iterator(data_ + size());
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(data_ + size());
		}

		const_reverse_iterator crbegin() const {
			return const_reverse_iterator(data_ + size());
		}
};


}//namespace ended