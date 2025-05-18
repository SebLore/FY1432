#include <iostream>
#include <stdexcept>

#include "Application.h"

static constexpr auto g_windowTitle = "Billiards Simulation";
static constexpr int g_windowWidth = 1280;
static constexpr int g_windowHeight = 720;

int main(int argc, char** argv)
{
	try
	{
		Application app(g_windowWidth, g_windowHeight, g_windowTitle);
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "FATAL ERROR: " << e.what() << "\n";
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "FATAL ERROR: An unknown exception occurred." << "\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}