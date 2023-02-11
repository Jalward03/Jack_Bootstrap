#include "Softbody.h"

void Softbody::Build(PhysicsScene* scene, glm::vec2 position, float damping, float springForce, float spacing, std::vector<std::string>& strings)
{
	int numColumns = strings.size();
	int numRows = strings[0].length();

	Circle** circles = new Circle * [numRows * numColumns];
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			if (strings[j][i] == '0')
			{
				circles[i * numColumns + j] = new Circle(position = glm::vec2(i, j) * 
					spacing, glm::vec2(0, 0), 1.f, 2.f, glm::vec4(1, 0, 0, 1));
				scene->AddActor(circles[i * numColumns + j]);
			}
			else
			{
				circles[i * numColumns + j] = nullptr;
			}
		}
	}

}
