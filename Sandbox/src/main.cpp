#include <vector>

#include "Debug/Instrumentor.h"
#include "Core/Application.h"

std::ostream& operator<<(std::ostream& ostr, const glm::vec4 &vec)
{
	ostr << "{ " << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << " }";
	return ostr;
}


int main()
{
	PROFILE_BEGIN_SESSION("Runtime", "Profile-Runtime.json");

	Application* app = new Application("Unit World");

	app->Run();

	delete app;

	PROFILE_END_SESSION();

	return 0;
}