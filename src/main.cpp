#include "vulkan/application.h"

#include <cstdlib>
#include <stdexcept>
#include <iostream>

int main() {
	eng::Application app;
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;	
}