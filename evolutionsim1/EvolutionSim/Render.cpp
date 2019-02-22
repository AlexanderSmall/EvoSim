#pragma once

#include <odingine/Errors.h>
#include <odingine/Camera2D.h>

#include "Render.h"

#include <iostream>

// We need to pass relative addresses to opengl
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static constexpr size_t circleVertices = 13;


odingine::Vertex Render::convert(b2Vec2 const & p, b2Color const &q)
{ 
	odingine::Vertex v;
	v.setPosition(p.x, p.y);
	v.setColor(q.r, q.g, q.b, q.a);
	return v;

}

Render::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	glGenVertexArrays(0, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
}
/*
void Render::SetFlags(uint32 flags)
{
	m_drawFlags = flags;
}
*/

/*
void Render::draw()
{
	//glClearDepth(1.0);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (glGetError() != GL_NO_ERROR) return;

	m_colorProgram.use();

	if (!segments.empty()) {

		if (verbose) printf("drawing %lu objects and %lu segments\n", sizes.size(), segments.size());

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(odingine::Vertex) * segments.size(), &segments[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(odingine::Vertex), BUFFER_OFFSET(0));
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(odingine::Vertex), BUFFER_OFFSET(3*4));

		GLuint start = 0;
		for (auto&& s : sizes) {
			glDrawArrays(GL_LINE_LOOP, start, s);
			start += s;
		}

	}

}
*/

void Render::clear()
{
}

void Render::resize(float width, float height)
{
}

void Render::DrawPolygon(const b2Vec2 * vertices, int32 vertexCount, const b2Color & color)
{
	if (verbose) printf("polygon (not solid)\n  %d vertices\n  %f %f %f\n", vertexCount, color.r, color.g, color.b);
	for (int i = 0; i < vertexCount; ++i) {
		segments.push_back(convert(vertices[i], color));
	}
	sizes.emplace_back(vertexCount);
}

void Render::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	//if (verbose) printf("polygon (solid, deferred)\n");
	//DrawPolygon(vertices, vertexCount, color);
	/*
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_POLYGON);
	for (int i = 0; i < vertexCount; i++) {
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
	*/
}

void Render::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
	//if (verbose) printf("circle (not solid)\n  at %f %f (%f)\n  %f %f %f\n", center.x, center.y, radius, color.r, color.g, color.b);
	//for (auto&& p : getCircle()) {
	//	segments.push_back(convert(center + radius * p, color));
	//}
	//sizes.emplace_back(circleVertices);
}

void Render::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
	if (verbose) printf("circle (solid, deferred)\n");
	segments.push_back(convert(center - radius * axis, color));
	segments.push_back(convert(center + radius * axis, color));
	sizes.push_back(2);
	DrawCircle(center, radius, color);
}

void Render::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	if (verbose) printf("segment\n  %f %f -> %f %f\n  %f %f %f\n", p1.x, p1.y, p2.x, p2.y, color.r, color.g, color.b);
	segments.push_back(convert(p1, color));
	segments.push_back(convert(p2, color));
	sizes.emplace_back(2);
}

void Render::DrawTransform(const b2Transform& xf) {
	if (verbose) printf("transform\n");
}
