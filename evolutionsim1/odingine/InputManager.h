#pragma once

#include <glm/glm.hpp>

#include <unordered_map>

namespace odingine {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		void setMouseCoords(float x, float y);

		bool isKeyDown(unsigned int keyID);


		/// Returns true if the key was just pressed
		bool isKeyPressed(unsigned int keyID);

		// getters
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		// Returns true if the key is held down
		bool wasKeyDown(unsigned int keyID);
		
		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;
		glm::vec2 _mouseCoords;


	};

}

