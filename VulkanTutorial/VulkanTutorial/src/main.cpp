#include "Renderer.h"

#include <stdexcept>
#include <iostream>

int main()
{
	Application::Renderer app;

	try
	{
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}