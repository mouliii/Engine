#include "CrEngine.h"

class DrawSystem : public System
{
public:
	DrawSystem() = default;

	void on_draw_call(Window* render_window, Renderer* renderer);

};


class CollisionSystem : public System
{
public:
	CollisionSystem() = default;

	void on_game_tick(TimeStep delta_time);
	
	bool AABB(Rectangle& a, Rectangle& b);
	bool move_and_collide(Entity entity, vec2f translation);
	void project_vertices(std::vector<vec2f>& vertices, const vec2f& axis, float& min, float& max);
	bool polygon_intersect(CollisionComponent* a, CollisionComponent* b, vec2f& normal, float& depth);
	void find_bounding_box(CollisionComponent* collision_component, Rectangle& bbox);
	vec2f find_center(const std::vector<vec2f>& vertices);

};


