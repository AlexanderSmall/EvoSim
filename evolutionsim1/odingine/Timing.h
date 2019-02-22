#pragma once

namespace odingine {

	class FpsLimiter {
	public:
		FpsLimiter();
		void init(float _maxFPS);

		void setMaxFPS(float _maxFPS);

		void begin();
			
		float end();	// end will return the current fps	

	private:
		void calculateFPS();

		float _maxFPS;
		float _fps;
		float _frameTime;

		unsigned int _startTicks;
	
	};

}