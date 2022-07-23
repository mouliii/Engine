#include "Cherry.h"
#include "Cherry/Logic/ImGuiLayer.h"
#include "ExampleLayer.h"



int main()
{

	CherryEngine::init();
	

	ExampleLayer* example_layer = new ExampleLayer();
	CherryEngine::push_layer(example_layer);

	ImGuiLayer* imguil = new ImGuiLayer();
	CherryEngine::push_layer(imguil);

	//std::cout << CherryEngine::get_layer("main") << " " << CherryEngine::get_layer("secondary") << "\n";
	

	CherryEngine::run();
	CherryEngine::destroy();

}


 