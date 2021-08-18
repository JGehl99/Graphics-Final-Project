#ifndef CAMERA_
#define CAMERA_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Window.h"

using namespace glm;


// Class for the camera
class Camera {

private:
	// vec3 values for glm::lookAt()
	vec3 pos;
	vec3 target;
	vec3 up;

	// Direction of the camera, normalized
	vec3 dir;

	// Direction of vector along the +x axis relative to the
	// camera's position, normalized
	vec3 right;

	// Direction of vector along the -z axis relative to the
	// camera's position, normalized
	vec3 front;

	// View mat
	mat4 view;

	// Proj Matrix
	float fov;
	float aspect;
	glm::mat4 projection;

	// Boolean for if movement is enabled
	bool movement = false;

public:

	Camera(vec3 cPos, vec3 cTar, vec3 upV, float fov_, float a):
		pos(cPos), target(cTar), up(upV), fov(fov_), aspect(a) {

		// Create proj mat
		projection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);

		// Calculate dir and right based on params
		dir = normalize(target-pos); 
		right = normalize(cross(up, dir));
		front = normalize(cross(right, up)); 

		// Calculate view mat at pos, dir, up
		view = lookAt( 
			pos,
			dir,
			up
		);

	};

	// Adjust the rotation of the dir vector around y axis
	void rotateDirY(float angle) {
		dir = rotate(dir, radians(angle), vec3(0.0f, 1.0f, 0.0f));
		updateView();
	}
	// Adjust the rotation of the dir vector around right vec,
	// since right vec is always horizontal and exactly right
	// relative to the camera pos, using that as the rotation axis
	// allows the camera to look up and down at any yaxis rotation
	// without affecting the z axis rotation
	void rotateDirX(float angle) {
		dir = rotate(dir, radians(angle), right);
		updateView();
	}

	// Move forward/backward along front vector by dist
	void move(float dist) {
		pos += normalize(front) * dist;
		updateView();
	}
	
	// Move left/right along right vector by dist
	void strafe(float dist) {
		pos -= normalize(right) * dist;
		updateView();
	}

	// Move up/down along up by dist
	void height(float dist) {
		pos += normalize(up) * dist;
		updateView();
	}

	// Recalculate right vector, front vector, and view mat
	void updateView() {
		right = normalize(cross(up, dir));
		front = normalize(cross(right, up));
		view = lookAt(
			pos,
			dir + pos,
			up
		);
	}

	// Update pos, dir, up, right, front, and view
	void updateView(glm::vec3 p, glm::vec3 d, glm::vec3 u) { 
		pos = p;
		dir = d;
		up = u; 

		right = normalize(cross(up, dir));
		front = normalize(cross(right, up));

		view = lookAt(pos, pos + dir, up);
	}

	// Get movement
	bool isMovementEnabled() {
		return movement;
	}

	// Toggle movement
	void toggleMovement() {
		movement = !movement;
	}

	void camera_callback(Window w) {
		GLFWwindow* window = w.getWindow();

		// If screen is paused, disable mouse and move controls, unlock mouse
		if (!w.isPaused()) {

			// If movement is disabled, disable move controls
			if (movement) {
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
					std::cout << "Hit W" << std::endl;
					move(0.05f);
				}
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
					std::cout << "Hit A" << std::endl;
					strafe(-0.05f);
				}
				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
					std::cout << "Hit S" << std::endl;
					move(-0.05f);
				}
				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
					std::cout << "Hit D" << std::endl;
					strafe(0.05f);
				}
				if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
					std::cout << "Hit LShift" << std::endl;
					height(-0.05f);
				}
				if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
					std::cout << "Hit Space" << std::endl;
					height(0.05f);
				}
			}

			// Get Cursor Pos
			double x, y;
			glfwGetCursorPos(w.getWindow(), &x, &y);

			// Set cursor pos to middle of the screen
			glfwSetCursorPos(window, w.getWidth() / 2, w.getHeight() / 2);

			// Calculate offset to middle of screen, 
			double dx = (w.getWidth() / 2 - x);
			double dy = (y - w.getHeight() / 2);

			// Rotate around y by sensitivity * dx
			if (dx != 0) {
				rotateDirY(0.05f * dx);
			}
			// Rotate around y by sensitivity * dx
			if (dy != 0) {
				rotateDirX(0.05f * dy);
			}
		}
	}
	
	// Getter and Setter for view mat
	mat4& getView() { return view; }
	mat4& setView(mat4 m) { this->view = m; }

	// Getter and Setter for proj mat
	mat4& getProj() { return projection; }
	mat4& setProj(mat4 m) { this->projection = m; }

	// Getter and Setter for pos
	vec3& getPos() { return pos; };
	vec3& setPos(vec3 vec) { this->pos = vec; };
};

#endif
