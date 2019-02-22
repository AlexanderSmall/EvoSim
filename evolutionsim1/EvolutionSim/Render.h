#pragma once

#include <odingine/Vertex.h>
#include <odingine/GLSLProgram.h>
#include <odingine/Camera2D.h>

#include <Box2D/Box2D.h>
#include <GL/GL.h>

#include <string>
#include <vector>

class Render : public b2Draw {
	
	Render();

	// print debugging info
	bool verbose = false;

	// draw buffer
	void draw();

	// clear buffer
	void clear();

	// resize any stuff
	void resize(float width, float height);

	//convert
	odingine::Vertex convert(b2Vec2 const & p, b2Color const &q);


	// Box 2D drawing interface
	// Implementation will fill its buffers
	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	virtual void DrawTransform(const b2Transform& xf);

private:

	//moggle::shader_program program;
	//moggle::matrix4<float> projection;

	odingine::GLSLProgram m_colorProgram;
	
	std::vector<odingine::Vertex> segments;
	std::vector<GLuint> sizes;
	GLuint vao, buf;

};
