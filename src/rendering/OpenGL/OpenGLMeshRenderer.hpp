//
//  OpenGLMeshRenderer.hpp
//  Renderer
//
//  Created by Christopher Berry on 05/07/2014.
//  Copyright (c) 2014 Christopher Berry. All rights reserved.
//

#ifndef _opengl_mesh_renderer_h
#define _opengl_mesh_renderer_h

#include "rendering/MeshRenderer.hpp"
#include "rendering/Camera.hpp"
#include "OpenGLShader.hpp"

// OpenGL includes
#ifdef _WIN32
#include <gl/glew.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL2/SDL_opengl.h>

namespace Renderer
{
    template <typename T>
	class OpenGLMeshRendererType : public MeshRendererType<T>
	{
		public:
			static OpenGLMeshRendererType<T> * Create(RenderingContext * rendering_context)
            {
                return new OpenGLMeshRendererType<T>(rendering_context);
            }
        
			void SetMesh(MeshType<T> * mesh)
			{
				if (mesh == nullptr)
				{
					// todo : throw error
					return;
				}

				mesh->Validate();
                
                this->GenerateArrays(mesh);
				this->CreateShader(mesh);

				this->_mesh = mesh;
			}

			void GenerateArrays(MeshType<T> * mesh)
			{
				// generate the vertex position, color and triangle index arrays
				glGenVertexArrays(1, _vertex_array_objects);
				glBindVertexArray(_vertex_array_objects[0]);

				glGenBuffers(1, _vertex_buffer_objects);
				glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_objects[0]);
				glBufferData(GL_ARRAY_BUFFER,
					mesh->VerticesSize() + mesh->ColorsSize(),
					NULL, GL_STATIC_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->VerticesSize(), mesh->Vertices());
				glBufferSubData(GL_ARRAY_BUFFER, mesh->VerticesSize(), mesh->ColorsSize(), mesh->Colors());

				glGenBuffers(1, _vertex_element_buffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_element_buffer[0]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					mesh->TrianglesSize(),
					mesh->Triangles(),
					GL_STATIC_DRAW);
			}

            void CreateShader(MeshType<float> * mesh)
            {                
                this->_material->Use();
                
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid *>(mesh->VerticesSize()));

                OpenGLShader * opengl_shader = dynamic_cast<OpenGLShader *>(this->_material->Shader());
                
                _model_matrix_uniform = glGetUniformLocation(opengl_shader->Program(), "model_matrix");
                _projection_matrix_uniform = glGetUniformLocation(opengl_shader->Program(), "projection_matrix");
                
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
            }
        
			void CreateShader(MeshType<double> * mesh)
            {
                this->_material->Use();
                
                glVertexAttribPointer(0, 4, GL_DOUBLE, GL_FALSE, 0, NULL);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid *>(mesh->VerticesSize()));
                
                OpenGLShader * opengl_shader = dynamic_cast<OpenGLShader *>(this->_material->Shader());
                
                _model_matrix_uniform = glGetUniformLocation(opengl_shader->Program(), "model_matrix");
                _projection_matrix_uniform = glGetUniformLocation(opengl_shader->Program(), "projection_matrix");
                
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
            }

			void Draw(ObjectType<float> * parent_object)
			{
				if (this->_mesh == nullptr)
					return;
                
                this->_material->Use();
				glUniformMatrix4fv(_projection_matrix_uniform, 1, GL_FALSE, Camera::MainCamera()->ProjectionMatrix());
				glUniformMatrix4fv(_model_matrix_uniform, 1, GL_FALSE,
                parent_object->LocalTransform()->ComposedMatrix().Multiply(Camera::MainCamera()->ViewMatrix()));

				glBindVertexArray(_vertex_array_objects[0]);
				glDrawElements(GL_TRIANGLES, this->_mesh->TrianglesCount() * 3, GL_UNSIGNED_INT, NULL);
			}

			void Draw(ObjectType<double> * parent_object)
            {
                if (this->_mesh == nullptr)
                    return;
             
                this->_material->Use();
				glUniformMatrix4dv(_projection_matrix_uniform, 1, GL_FALSE, Camerad::MainCamera()->ProjectionMatrix());
                glUniformMatrix4dv(_model_matrix_uniform, 1, GL_FALSE,
                parent_object->LocalTransform()->ComposedMatrix().Multiply(Camerad::MainCamera()->ViewMatrix()));
                
				glBindVertexArray(_vertex_array_objects[0]);
				glDrawElements(GL_TRIANGLES, this->_mesh->TrianglesCount() * 3, GL_UNSIGNED_INT, NULL);
            }

			~OpenGLMeshRendererType<T>()
            {
                
            }

            OpenGLMeshRendererType<T>(RenderingContextType<T> * rendering_context)
            {
                
            }
        
		protected:
        
            RenderingContextType<T> * _rendering_context;
        
            GLuint _vertex_array_objects[1];
			GLuint _vertex_element_buffer[1];
            GLuint _vertex_buffer_objects[1];
        
            GLint _model_matrix_uniform;
            GLint _projection_matrix_uniform;
	};
    
    typedef OpenGLMeshRendererType<float> OpenGLMeshRenderer;
    typedef OpenGLMeshRendererType<double> OpenGLMeshRendererd;
}

#endif // _opengl_mesh_renderer_h