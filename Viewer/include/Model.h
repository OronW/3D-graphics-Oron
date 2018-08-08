#pragma once
#include <vector>
#include <glm/glm.hpp>

using namespace std;
/*
 * Model class. An abstract class for all types of geometric data.
 */
class Model
{
public:
	virtual ~Model() {}
	virtual const vector<glm::vec4>* Draw() = 0;
	virtual void createTransformation() = 0;
};
