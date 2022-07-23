#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"
#include "Application.h"



int main()
{

	CherryEngine::init();
	

	Application* application = new Application();	
	CherryEngine::add_layer(application, "main");

	ImGuiLayer* imguil = new ImGuiLayer();
	CherryEngine::add_layer(imguil, "imguil");

	//std::cout << CherryEngine::get_layer("main") << " " << CherryEngine::get_layer("secondary") << "\n";
	

	CherryEngine::run();
	CherryEngine::destroy();

}


 