#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"

#include <cstddef>

namespace odingine {

	Sprite::Sprite()
	{
	}


	Sprite::~Sprite()
	{
		// remove vertex buffer for sprite
		if (_vboID != 0) {
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height)
	{
		_x = x;
		_y = y;
		_width = width;
		_height = height;

		//_texture = ResourceManager::getTexture(texturePath);

		if (_vboID == 0) {	// Generate VBO if one has not already been created
			glGenBuffers(1, &_vboID);	// generate buffer and then allocate pointer of vboID to a buffer
		}

		// temp triangles to test openGL
		Vertex vertexData[6];

		// 1
		vertexData[0].setPosition(x + width, y + height);
		//vertexData[0].setUV(1.0f, 1.0f);

		vertexData[1].setPosition(x, y + height);
		//vertexData[1].setUV(0.0f, 1.0f);

		vertexData[2].setPosition(x, y);
		//vertexData[2].setUV(0.0f, 0.0f);

		// 2
		vertexData[3].setPosition(x, y);
		//vertexData[3].setUV(0.0f, 0.0f);

		vertexData[4].setPosition(x + width, y);
		//vertexData[4].setUV(1.0f, 0.0f);

		vertexData[5].setPosition(x + width, y + height);
		//vertexData[5].setUV(1.0f, 1.0f);

		// 3
		/*
		vertexData[6].setPosition(x + width, y + height);
		vertexData[6].setUV(1.0f, 1.0f);

		vertexData[7].setPosition(x, y + height * 2);
		vertexData[7].setUV(1.0f, 0.0f);

		vertexData[8].setPosition(x + width * 1.3 , y * 1.2);
		vertexData[8].setUV(1.0f, 1.0f);
		*/
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);	// bind buffer using array buffer
		// type of buffer, size of data, pointer to first element, dynamic/static
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);	// unbind the buffer
	}

	void Sprite::draw()
	{
		//glBindTexture(GL_TEXTURE_2D, _texture.id);

		//aglEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID); // bind array buffer with vboID

		//glDrawArrays(GL_TRIANGLES, 0, 6);	// Draws array vertices

		glDrawArrays(GL_LINES, 0, 6);	// Draws array vertices

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}
