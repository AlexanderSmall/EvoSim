#pragma once

#include "TextureCache.h"

#include <string>

namespace odingine {

	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

		//static GLuint bountTexture;

	private:
		static TextureCache _textureCache;
	};
}

