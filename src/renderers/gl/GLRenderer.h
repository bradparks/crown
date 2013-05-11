/*
Copyright (c) 2012 Daniele Bartolini, Simone Boscaratto

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <GL/glew.h>

#include "Renderer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderBuffer.h"
#include "IdTable.h"
#include "MallocAllocator.h"

namespace crown
{

const uint32_t MAX_TEXTURE_UNITS = 8;

class TextureResource;

//-----------------------------------------------------------------------------
struct GLTexture
{
	GLuint				gl_object;

	TextureResource*	texture_resource;
};

//-----------------------------------------------------------------------------
struct GLVertexBuffer
{
	GLuint				gl_object;
	size_t				count;
	VertexFormat		format;
};

//-----------------------------------------------------------------------------
struct GLIndexBuffer
{
	GLuint				gl_object;
	uint32_t			index_count;
};

//-----------------------------------------------------------------------------
struct GLRenderBuffer
{
	GLuint				gl_frame_buffer;
	GLuint				gl_render_buffer;
};

/// OpenGL renderer
class GLRenderer : public Renderer
{
public:

						GLRenderer();
						~GLRenderer();

	VertexBufferId		create_vertex_buffer(const void* vertices, size_t count, VertexFormat format);
	VertexBufferId		create_dynamic_vertex_buffer(const void* vertices, size_t count, VertexFormat format);
	void				update_vertex_buffer(VertexBufferId id, const void* vertices, size_t count, size_t offset);
	void				destroy_vertex_buffer(VertexBufferId id);

	IndexBufferId		create_index_buffer(const void* indices, size_t count);
	void				destroy_index_buffer(IndexBufferId id);

	// RenderBufferId	create_render_buffer(uint32_t width, uint32_t height, PixelFormat format);
	// void				destroy_render_buffer(RenderBufferId id);

	void				begin_frame();
	void				end_frame();

	void				set_clear_color(const Color4& color);

	void				set_material_params(const Color4& ambient, const Color4& diffuse, const Color4& specular, const Color4& emission, int32_t shininess);
	void				set_lighting(bool lighting);
	void				set_ambient_light(const Color4& color);

	void				bind_texture(uint32_t unit, TextureId texture);
	void				set_texturing(uint32_t unit, bool texturing);
	void				set_texture_mode(uint32_t unit, TextureMode mode, const Color4& blendColor);
	void				set_texture_wrap(uint32_t unit, TextureWrap wrap);
	void				set_texture_filter(uint32_t unit, TextureFilter filter);

	void				set_light(uint32_t light, bool active);
	void				set_light_params(uint32_t light, LightType type, const Vec3& position);
	void				set_light_color(uint32_t light, const Color4& ambient, const Color4& diffuse, const Color4& specular);
	void				set_light_attenuation(uint32_t light, float constant, float linear, float quadratic);

	void				set_backface_culling(bool culling);

	void				set_separate_specular_color(bool separate);

	void				set_depth_test(bool test);
	void				set_depth_write(bool write);
	void				set_depth_func(CompareFunction func);

	void				set_rescale_normals(bool rescale);

	void				set_blending(bool blending);
	void				set_blending_params(BlendEquation equation, BlendFunction src, BlendFunction dst, const Color4& color);
	void				set_color_write(bool write);

	void				set_fog(bool fog);
	void				set_fog_params(FogMode mode, float density, float start, float end, const Color4& color);

	void				set_alpha_test(bool test);
	void				set_alpha_params(CompareFunction func, float ref);

	void				set_shading_type(ShadingType type);
	void				set_polygon_mode(PolygonMode mode);
	void				set_front_face(FrontFace face);

	void				set_viewport_params(int32_t x, int32_t y, int32_t width, int32_t height);
	void				get_viewport_params(int32_t& x, int32_t& y, int32_t& width, int32_t& height);

	void				set_scissor(bool scissor);
	void				set_scissor_params(int32_t x, int32_t y, int32_t width, int32_t height);
	void				get_scissor_params(int32_t& x, int32_t& y, int32_t& width, int32_t& height);

	void				set_point_sprite(bool sprite);
	void				set_point_size(float size);
	void				set_point_params(float min, float max);

	Mat4				get_matrix(MatrixType type) const;
	void				set_matrix(MatrixType type, const Mat4& matrix);

	void				bind_vertex_buffer(VertexBufferId vb) const;
	//void				bind_render_buffer(RenderBufferId id) const;

	void				draw_triangles(IndexBufferId id) const;

	void				draw_lines(const float* vertices, const float* colors, uint32_t count);

	TextureId			load_texture(TextureResource* texture);
	void				unload_texture(TextureResource* texture);
	TextureId			reload_texture(TextureResource* old_texture, TextureResource* new_texture);

private:

	/// Activates a texture unit and returns true if succes
	bool				activate_texture_unit(uint32_t unit);
	bool				activate_light(uint32_t light);

	void				check_gl_errors();

private:

	MallocAllocator		m_allocator;

	// Matrices
	Mat4				m_matrix[MT_COUNT];

	// Limits
	int32_t				m_max_lights;
	int32_t				m_max_texture_size;
	int32_t				m_max_texture_units;
	int32_t				m_max_vertex_indices;
	int32_t				m_max_vertex_vertices;

	float				m_max_anisotropy;
	float				m_min_max_point_size[2];
	float				m_min_max_line_width[2];

	// Viewport and scissor
	int32_t				m_viewport[4];
	int32_t				m_scissor[4];

	// Texture management
	IdTable 			m_textures_id_table;
	GLTexture			m_textures[MAX_TEXTURES];

	uint32_t			m_active_texture_unit;
	GLuint				m_texture_unit[MAX_TEXTURE_UNITS];
	GLenum				m_texture_unit_target[MAX_TEXTURE_UNITS];

	// Vertex/Index buffer management
	IdTable				m_vertex_buffers_id_table;
	GLVertexBuffer		m_vertex_buffers[MAX_VERTEX_BUFFERS];

	IdTable				m_index_buffers_id_table;
	GLIndexBuffer		m_index_buffers[MAX_INDEX_BUFFERS];

	// Render buffer management
	//IdTable				m_render_buffers_id_table;
	//GLRenderBuffer		m_render_buffers[MAX_RENDER_BUFFERS];

	friend class		TextureResource;
};

} // namespace crown

