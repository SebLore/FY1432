#include "Application.h" // Your main application class
#include <iostream>
#include <stdexcept>


int main(int argc, char** argv)
{
	try
	{
		Application app(1280, 720, "Billiards Simulation");
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "FATAL ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "FATAL ERROR: An unknown exception occurred." << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}