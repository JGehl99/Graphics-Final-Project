#ifndef LIGHTSOURCE
#define LIGHTSOURCE

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Each LightModel has exactly 1 Light object inside it.
// It holds everything needed to calculate the ambient,
// diffuse, and specular lighting.

class Light {
public:

	// lightPos represents the very center of the LightModel
	glm::vec3 lightPos; 

	// Holds light color
	glm::vec3 lightColor;

	// Ambient, diffuse, and specular strength values
	float aStr;
	float dStr;
	float sStr;

	// Specular coefficients
	float constant;
	float linear;
	float quadratic;

	Light() = default;

	Light(glm::vec3 lp, glm::vec3 lc, float a, float d, float s, float c, float l, float q):
		  lightPos(lp), lightColor(lc), aStr(a), dStr(d), sStr(s), constant(c), linear(l), quadratic(q){}

};

// toString for Light
std::ostream& operator<<(std::ostream& os, Light& l) {
	std::cout << "Light:" << std::endl;
	std::cout << "\tlightPos: " << glm::to_string(l.lightPos) << std::endl;
	std::cout << "\tlightCol: " << glm::to_string(l.lightColor) << std::endl;
	std::cout << "\taStr: " << l.aStr << std::endl;
	std::cout << "\tdStr: " << l.dStr << std::endl;
	std::cout << "\tsStr: " << l.sStr << std::endl;
	std::cout << "\tconstant: " << l.constant << std::endl;
	std::cout << "\tlinear: " << l.linear << std::endl;
	std::cout << "\tquadratic: " << l.quadratic << std::endl;
	std::cout << std::endl;
	return os;
}

#endif