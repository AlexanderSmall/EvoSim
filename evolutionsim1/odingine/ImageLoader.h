#pragma once
#include "GlTexture.h"

#include <string>

namespace odingine {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}
