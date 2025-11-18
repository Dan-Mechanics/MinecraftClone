#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

class Model {
public:
	Model(const char* file);
	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accesor);
	std::vector<GLuint> getIndices(json accesor);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec2> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec2> groupFloatsVec4(std::vector<float> floatVec);

};

#endif // !MODEL_CLASS_H
