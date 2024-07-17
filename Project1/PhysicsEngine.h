#pragma once
#include "TimeServicePort.h"

namespace Game {
	class PhysicsEngine {
	private:
		TimeServicePort* timeServicePort;
	public:
		PhysicsEngine(TimeServicePort* timeServicePort_) : timeServicePort(timeServicePort_) {}

		float calcDisplacement(float speedPoints) {
			float elapsedTime = this->timeServicePort->getLastCurrentTimeInSeconds() - this->timeServicePort->getLastElapsedTimeInSeconds();
			return elapsedTime * speedPoints;
		}
	};
}