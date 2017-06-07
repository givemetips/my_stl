
namespace my_stl {

template <typename T>
class DefaultAllocator {
	
private:
	using value_type = T;
	using pointer = T*;

public:

	static_assert(sizeof(char) == 1, "TAllocator assumes that size of char == 1");

	pointer allocate(size_t n) const {
		const size_t charsCount = n * sizeof(value_type);

		return static_cast<T*>(
			static_cast<void*>(new char[charsCount])
			);
	
	}

	void deallocate(pointer p) {
		
		if (p) {
			delete[] static_cast<char*>( 
				static_cast<void*>(p)
			);
		}
	
	}

	template <typename... Args>
	void construct(T* p, Args&&... args) {
		
		if (p) {
			new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
		}
	
	}

	void destroy(T* p) {

		if (p) {
			p->~T();
		}
	
	}
};

}