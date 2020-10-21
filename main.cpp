#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

int main()
{
	int i;
	for (i = 0; i < 16; i++)
	{
		std::cout << "*";
	}
	std::cout << "" << std::endl;
	for (i = 0; i < 6; i++)
	{
		std::cout << "*              *" << std::endl;
	}
	for (i = 0; i < 16; i++)
	{
		std::cout << "*";
	} 
	std::cout << "" << std::endl;
	
	return 0;
}
