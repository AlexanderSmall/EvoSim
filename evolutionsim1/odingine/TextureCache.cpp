#include "TextureCache.h"
#include "ImageLoader.h"

#include <iostream>

namespace odingine {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		// look up texture and check if in map
		std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath);

		// check if not in the map
		if (mit == _textureMap.end()) {
			// load the texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			// insert texture into map
			_textureMap.emplace(make_pair(texturePath, newTexture));

			//std::cout << "Load cached texture" << "\n";
			return newTexture;
		}

		//std::cout << "Used texture" << "\n";
		return mit->second;

	}

}
