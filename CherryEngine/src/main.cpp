#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"
#include "Application.h"



int main()
{

	CherryEngine::init();
	

	Application* application = new Application();	
	CherryEngine::push_application(application);

	ImGuiLayer* imguil = new ImGuiLayer();
	CherryEngine::push_application(imguil);

	//std::cout << CherryEngine::get_layer("main") << " " << CherryEngine::get_layer("secondary") << "\n";
	

	CherryEngine::run();
	CherryEngine::destroy();

}


 