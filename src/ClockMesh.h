#ifndef CLOCKMESH_
#define CLOCKMESH_

#include "Mesh.h"

class ClockMesh : public Mesh {

private:

	// Mesh objects for clock hands
	Mesh& secondHand;
	Mesh& minuteHand;
	Mesh& hourHand;

	// Time
	time_t currentTime;
	struct tm localTime;

	// Holds time values
	int minute;
	int sec;
	int hour;

public:

	// ClockMesh constructor
	ClockMesh(Texture& tex, Shader& s, Camera& c, std::string path, std::vector<Light*>* lsrc, Mesh& sh, Mesh& mh, Mesh& hh)
		: Mesh(tex, s, c, path, lsrc),
		secondHand(sh),
		minuteHand(mh),
		hourHand(hh) {

		// Translate and scale hands to the proper location
		secondHand.translate(glm::vec3(0.0f, 0.3f, 0.1f));
		secondHand.scale(glm::vec3(0.01f, 0.3f, 0.01f));

		minuteHand.translate(glm::vec3(0.0f, 0.2f, 0.1f));
		minuteHand.scale(glm::vec3(0.02f, 0.25f, 0.01f));

		hourHand.translate(glm::vec3(0.0f, 0.1f, 0.1f));
		hourHand.scale(glm::vec3(0.03f, 0.2f, 0.01f));
	}

	// Initialize time and rotate the hands to the proper starting time
	void initTime() {
		time(&currentTime);
		localtime_s(&localTime, &currentTime);

		sec = localTime.tm_sec;
		minute = localTime.tm_min;
		hour = localTime.tm_hour;

		rotateSecond(6 * sec);
		rotateMinute(6 * minute);
		rotateHour(0.5 * (hour * 60 + minute));
	}

	// Update the time values and hand rotations
	void updateTime() {

		// Get current time
		time(&currentTime);
		localtime_s(&localTime, &currentTime);

		// If time has changed since last loop, update time and rotate hand by 1 step
		if (sec != localTime.tm_sec) {
			sec = localTime.tm_sec;
			rotateSecond(6);
		}

		// If minute has changed since last loop, update minute and hour hand by 1 step
		// Hour is incremented 1 minute at a time to make it a smooth rotation
		if (minute != localTime.tm_min) {
			minute = localTime.tm_min;
			rotateMinute(6);
			rotateHour(0.5);
		}

		// Update hour value if it changed since last loop
		if (hour != localTime.tm_hour % 12) {
			hour = localTime.tm_hour;
		}
	}


	// Translate object by vec
	glm::mat4& translate(glm::vec3 vec) {
		glm::mat4 mat = glm::translate(glm::mat4(1.0f), vec);
		model = mat * model;

		secondHand.translate(vec);
		minuteHand.translate(vec);
		hourHand.translate(vec);

		return model;
	}

	// Rotate around object origin by angle around axis
	glm::mat4& rotate(float angle, glm::vec3 axis) {
		model = glm::rotate(model, glm::radians(angle), axis);

		glm::vec3 middlePoint = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		secondHand.rotate(middlePoint, angle, axis);
		minuteHand.rotate(middlePoint, angle, axis);
		hourHand.rotate(middlePoint, angle, axis);


		return model;
	}

	// Rotate second hand by angle around center of clock
	void rotateSecond(float angle) {

		glm::vec3 middlePoint = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		secondHand.rotate(middlePoint, -1 * angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	// Rotate minute hand by angle around center of clock
	void rotateMinute(float angle) {

		glm::vec3 middlePoint = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		minuteHand.rotate(middlePoint, -1 * angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	// Rotate hour hand by angle around center of clock
	void rotateHour(float angle) {

		glm::vec3 middlePoint = model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		hourHand.rotate(middlePoint, -1 * angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}
};



#endif
