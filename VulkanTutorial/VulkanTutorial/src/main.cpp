#include "TriangleApp.h"

#include <stdexcept>
#include <iostream>

int main()
{
	system("call compileShaders.bat");
	Application::TriangleApp app;

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