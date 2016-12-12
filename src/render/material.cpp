#include "material.h"

Material::Material(Texture *texture, const glm::vec4 &color, float intensity, float specularity) :
		color(color),
		mainTexture(texture) {

	floatValues.insert(std::pair<std::string, float>("intensity", intensity));
	floatValues.insert(std::pair<std::string, float>("specularity", specularity));
}
void Material::bind() const {
	mainTexture->bind(0);
}