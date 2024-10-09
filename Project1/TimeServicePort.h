#pragma once

namespace Game {
	class TimeServicePort {
	public:
		virtual int getCurrentTimeInSeconds() = 0;
		virtual void updateLastElapsedTimeInSeconds() = 0;
		virtual int getLastElapsedTimeInSeconds() = 0;
		virtual void updateLastCurrentTimeInSeconds() = 0;
		virtual int getLastCurrentTimeInSeconds() = 0;
		virtual void pauseTime() = 0;
		virtual void resumeTime() = 0;
		virtual bool getIsPaused() = 0;
		virtual ~TimeServicePort() {}
	};
}