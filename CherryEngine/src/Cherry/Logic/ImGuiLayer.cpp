#include "ImGuiLayer.h"


void ImGuiLayer::on_init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	Window* window = CherryEngine::get_render_window();

	ImGui_ImplGlfw_InitForOpenGL(window->_get_glfw_window(), true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
}

void ImGuiLayer::on_game_tick(TimeStep)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	static bool show_demo_window = true;
	static bool show_another_window = false;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (show_demo_window)
		ImGui::ShowDemoWindow();

	{


		SandBoxBase* main = CherryEngine::get_layer("main");
		Manager* manager = main->get_manager();
		const auto& entities = manager->get_entities();

		ImGui::Begin("entities");

		ImGui::OpenPopupOnItemClick("settings", ImGuiPopupFlags_MouseButtonRight);

		if (ImGui::BeginPopup("settings"))
		{
			static char name[64] = "entity_name";
			char buf[128];
			sprintf_s(buf, "entity: %s###Button", name); // ### operator override ID ignoring the preceding label
			ImGui::Button(buf);
			if (ImGui::BeginPopupContextItem())
			{
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::SameLine(); ImGui::Text("(<-- right click here)");

			if (ImGui::Button("add", { 30, 30 }))
			{
				manager->add_entity(std::string(name));
			}


			ImGui::EndPopup();
		}




		for (size_t i = 0; i < entities.size(); i++)
		{

			Entity* e = entities[i].get();
			const std::string& name = e->get_name();
			uint64_t entity_id = e->get_id();

			ImGui::PushID(i);

			bool entity_list = ImGui::CollapsingHeader(name.c_str());
			
			if (delete_item_on_right_click())
			{
				e->destroy();
				continue;
			}

			if (entity_list)
			{
				

				const ComponentArray* const components = e->get_all_components();
				ImGui::Indent(10.f);
				for (size_t j = 0; j < components->size(); j++)
				{
					
					Component* c = (*components)[j];
					if (!c)
						break;

					const std::string& component_name = component_type_to_string(c->c_type) + "##" + std::to_string(entity_id);

					ImGui::PushID(j+10);

					bool component_list = ImGui::CollapsingHeader(component_name.c_str());
					// DELETION OF COMPONENTS TODODO
				//if (delete_item_on_right_click())
				//{
				//	c->destroy();
				//	continue;
				//}

					if (component_list)
					{

						switch (c->c_type)
						{

						case ComponentType::ShapeComponent:
						{

							ShapeComponent* sc = static_cast<ShapeComponent*>(c);
							Polygon* shape = sc->get_shape();

							vec2f translation = shape->get_translation();
							float tr[2] = { translation.x, translation.y };
							ImGui::DragFloat2("translation", tr, 10.f, -INFINITY, INFINITY, "%f", 1.f);
							shape->set_translation({ tr[0], tr[1] });

							float rotation = shape->get_rotation();
							ImGui::DragFloat("rotation", &rotation, .05f, -3.141f, 3.141f, "%f", 1.f);
							shape->set_rotation(rotation);

							float scale = shape->get_scale();
							ImGui::DragFloat("scale", &scale, .1f, 0, INFINITY, "%f", 1.f);
							shape->set_scale(scale);
							std::vector<vec2f> local_points;
							shape->get_local_points(local_points);

							for (size_t w = 0; w < local_points.size(); w++)
							{
								
								float pt[2] = { local_points[w].x, local_points[w].y};
								std::string drag_float_name = "point" + std::to_string(w);
								ImGui::DragFloat2(drag_float_name.c_str(), pt, 0.5f, -INFINITY, INFINITY, "%f", 1.f);
								local_points[w] = {pt[0], pt[1]};
							}
							

							if (ImGui::Button("add point"))
							{

								local_points.push_back(vec2f(0.f, 0.f));

							}
							shape->set_local_points(local_points);

							break;
						}
						case ComponentType::CollisionComponent:
						{
							
							CollisionComponent* cc = static_cast<CollisionComponent*>(c);

							const std::bitset<COLLISION_MASK_LAYER_COUNT>& collision_mask = cc->get_collision_mask();
							ImGui::Text("collision mask");

							for (size_t j = 1; j <= COLLISION_MASK_LAYER_COUNT; j++)
							{
								bool b = collision_mask[j - 1];

								ImGui::Checkbox(std::to_string(j).c_str(), &b);

								cc->set_collision_mask_bit(j - 1, b);

								if (j == COLLISION_MASK_LAYER_COUNT || j == COLLISION_MASK_LAYER_COUNT / 2)
									continue;
								ImGui::SameLine();

							}

							break;

						}

						case ComponentType::DisplayComponent:
						{

							DisplayComponent* dc = static_cast<DisplayComponent*>(c);

							float col[4] = { dc->color.x, dc->color.y, dc->color.z, dc->color.w };

							ImGui::ColorPicker4("color", col);
							//ImGui::SliderFloat3()
							dc->color = { col[0], col[1], col[2], col[3] };

							ImGui::Checkbox("visible", &dc->visible);

							break;
						}

						}


					}

					ImGui::PopID();


				}

				ImGui::Indent(-10.f);
				
				if (ImGui::Button("+"))
				{
					ImGui::OpenPopup("entity_menu");

				}

				if (ImGui::BeginPopup("entity_menu"))
				{
					
					if (!&(e->get_component<CollisionComponent>()))
					{

						if (ImGui::Button("CollisionComponent"))
						{
							ImGui::OpenPopup("add CollisionComponent");
							
						}

						ImVec2 center = ImGui::GetMainViewport()->GetCenter();
						ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

						if (ImGui::BeginPopupModal("add CollisionComponent", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{
							
							if (!&(e->get_component<ShapeComponent>()))
							{
								
								if (this->construct_shape(e))
								{
									
									if (&(e->get_component<ShapeComponent>()))
									{
										e->add_component<CollisionComponent>(&(e->get_component<ShapeComponent>()));
										ImGui::CloseCurrentPopup();
									}
								}
							}
							else
							{
								
								if (ImGui::Button("create"))
								{
									e->add_component<CollisionComponent>(&e->get_component<ShapeComponent>());
								//	ImGui::CloseCurrentPopup();
								}
							}
							if (ImGui::Button("cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
							ImGui::EndPopup();
						}
						
					}
					

					if (!&(e->get_component<ShapeComponent>()))
					{

						if (ImGui::Button("ShapeComponent"))
						{
							ImGui::OpenPopup("add ShapeComponent");

						}

						if (ImGui::BeginPopupModal("add ShapeComponent", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{

							
							this->construct_shape(e);
							if (ImGui::Button("cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
							ImGui::EndPopup();
						}

					}

					if (!&(e->get_component<DisplayComponent>()))
					{

						if (ImGui::Button("DisplayComponent"))
						{
							ImGui::OpenPopup("add DisplayComponent");

						}

						if (ImGui::BeginPopupModal("add DisplayComponent", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{

							if (!&(e->get_component<ShapeComponent>()))
							{
								if (this->construct_shape(e))
								{

									if (&(e->get_component<ShapeComponent>()))
									{
										e->add_component<DisplayComponent>(&e->get_component<ShapeComponent>());
										ImGui::CloseCurrentPopup();
									}
								}
							}
							else
							{

								if (ImGui::Button("create"))
								{
									e->add_component<DisplayComponent>(&e->get_component<ShapeComponent>());
									ImGui::CloseCurrentPopup();
								}
							}
							if (ImGui::Button("cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
							ImGui::EndPopup();
						}
					}
					
					ImGui::EndPopup();

				}
				
			}	
			ImGui::PopID();
		}		
		
		ImGui::End();
	}
	

}
	





void ImGuiLayer::on_draw_call(Window*, Renderer*)
{
	ImGui::Render();
}

void ImGuiLayer::on_loop_end()
{
	set_render_data();
}

void ImGuiLayer::set_render_data()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ImGuiLayer::construct_shape(Entity* e)
{


	static ShapeType selected_shape = ShapeType::None;

	ImGui::Text("you can edit the exact shape later");

	if (ImGui::Button("create shape"))
		ImGui::OpenPopup("collision_shape_menu");

	ImGui::SameLine();
	ImGui::TextUnformatted(selected_shape == ShapeType::None ? "<None>" : shape_type_to_string(selected_shape).c_str());
	if (ImGui::BeginPopup("collision_shape_menu"))
	{
		ImGui::Text("shapes");
		ImGui::Separator();

		std::vector<std::string> shapes = get_shapes_as_string();

		for (size_t i = 0; i < shapes.size(); i++)
			if (ImGui::Selectable(shapes[i].c_str()))
				selected_shape = string_to_shape_type(shapes[i]);
		ImGui::EndPopup();
	}
	if (selected_shape != ShapeType::None)
	{

		if (ImGui::Button("create"))
		{
			switch (selected_shape)
			{
				case ShapeType::Polygon:
				{
					e->add_component<ShapeComponent>(Polygon());
					return true;
				}
				case ShapeType::Rectangle:
				{
					e->add_component<ShapeComponent>(Rectangle());
					return true;
				}
			}
		}
	}
	return false;
	
}

bool ImGuiLayer::delete_item_on_right_click()
{
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("delete_item");
	}
	if (ImGui::BeginPopup("delete_item"))
	{

		if (ImGui::Button("delete"))
		{
			
			ImGui::EndPopup();
			ImGui::PopID();
			return true;
		}

		ImGui::EndPopup();
	}
	return false;
}
