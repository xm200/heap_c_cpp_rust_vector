#include <cstddef>
#include <iostream>
#include <memory>

/*
 * So it coninues. I am still implementing vector but in cpp
 * Now i will resize and move instead of reallocating
*/
class Vector {
private:
	std::size_t size, capacity;
	std::unique_ptr<double[]> elems;
	

	// Some resize logic
	void resize() {
		if (size != capacity) return;
		capacity <<= 1;
		// some cringe cpp actions. actually thats why i like c more than cpp
		auto new_elems = std::make_unique<double[]>(capacity);
		std::move(elems.get(), elems.get() + size, new_elems.get());
		elems = std::move(new_elems);
	}
public:
	explicit Vector() :
		size(0),
		capacity(1),
		elems(std::make_unique<double[]>(capacity))
	{}

	~Vector() = default;
	
	// deny copy
	Vector(const Vector&) = delete;
	Vector& operator=(const Vector&) = delete;
	
	// allow move
	Vector(Vector&&) = default;
	Vector& operator=(Vector&&) = default;

	void add_element(double val) {
		if (size == capacity) {
			resize();
		}
		elems[size++] = val;
	}

	void print() {
		for (std::size_t i = 0; i < size; i++)
			printf("[idx]: %ld\t[elem]: %lf", i, elems[i]);
	}
};


int main() {
	// logical step from previous version is to create vector
	// at the beginning of the program
	
	auto vec = std::unique_ptr<Vector>();
	char choise;
	
	auto print_menu = [] {
		std::cout << "e. Put element\n"
			  << "p. Print vector\n"
			  << "q. Quit";
	};

	while (true) {
		print_menu();
		std::cout << "> ";
		std::cin >> choise;

		switch (choise) {
			case 'e' : {
				double val;
				std::cin >> val;
				vec->add_element(val);
				std::cout << "Stored!\n";
				break;
			} case 'p' : {
				vec->print();
				break;
			} case 'q' : {
				std::cout << "Goodbye!\n";
				return 0;
			}
		}
	}
}

