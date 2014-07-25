//
//  OpenGLRenderingContext.hpp
//  Renderer
//
//  Created by Christopher Berry on 05/07/2014.
//  Copyright (c) 2014 Christopher Berry. All rights reserved.
//

#ifndef _opengl_renderingcontext_h
#define _opengl_renderingcontext_h

#include "rendering/RenderingContext.hpp"
#include "rendering/OpenGL/OpenGLMeshRenderer.hpp"

namespace Renderer
{
    template <typename T>
	class OpenGLRenderingContextType : public RenderingContextType<T>
	{
		public:
            static const int kNumSupportedOpenGLVersions;
            static const int kSupportedOpenGLVersions[9][2];
        
            static OpenGLRenderingContextType<T> * Create(Window * window,
			int major_version = kSupportedOpenGLVersions[0][0],
            int minor_version = kSupportedOpenGLVersions[0][1])
            {
                if(window == nullptr)
                    return nullptr;
                
                OpenGLRenderingContextType<T> * context = new OpenGLRenderingContextType<T>();
                
                // ensure that the requested OpenGL version is supported
                int requested_major_version = major_version;
                int requested_minor_version = minor_version;
                
                int i = 0;
                for(i = 0; i<kNumSupportedOpenGLVersions; i++)
                {
                    if(kSupportedOpenGLVersions[i][0] == requested_major_version &&
                       kSupportedOpenGLVersions[i][1] == requested_minor_version)
                    {
                        break;
                    }
                }
                
                // if it's not, just go from the top
                if(i == kNumSupportedOpenGLVersions)
                {
                    requested_major_version = kSupportedOpenGLVersions[0][0];
                    requested_minor_version = kSupportedOpenGLVersions[0][1];
                }
                
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, requested_major_version);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, requested_minor_version);
                
                context->_sdl_gl_context = SDL_GL_CreateContext(window->_sdl_window);
                
                // run through the other supported versions if requested/top version fails
                i=0;
                while(context->_sdl_gl_context == nullptr && i < kNumSupportedOpenGLVersions)
                {
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, kSupportedOpenGLVersions[i][0]);
                    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, kSupportedOpenGLVersions[i][1]);
                    
                    context->_sdl_gl_context = SDL_GL_CreateContext(window->_sdl_window);
                    
                    i++;
                }
                
                if (context->_sdl_gl_context == nullptr)
                {
                    fprintf(stdout, "SDL_GL_CreateContext failed!\n");
                    return nullptr;
                }
                
#ifdef _WIN32
                glewExperimental = GL_TRUE;
                
                if (glewInit() != GLEW_OK)
                {
                    fprintf(stdout, "glewInit failed!\n");
                    return nullptr;
                }
#endif
                
                GLint actual_major_version = -1;
                GLint actual_minor_version = -1;
                
                glGetIntegerv(GL_MAJOR_VERSION, &actual_major_version);
                glGetIntegerv(GL_MINOR_VERSION, &actual_minor_version);
                
                fprintf(stdout, "OpenGL version: %d.%d\n", actual_major_version, actual_minor_version);
                
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                
                return context;
            }

            void BeginScene()
            {
                glClear(GL_COLOR_BUFFER_BIT);
            }
        
            void EndScene()
            {
                glFlush();
            }

            MeshRendererType<T> * MeshRenderer()
            {
                return new OpenGLMeshRendererType<T>(this);
            }
        
			~OpenGLRenderingContextType<T>()
            {
                
            }
        
		protected:
			OpenGLRenderingContextType<T>()
            {
                
            }
        
   			SDL_GLContext _sdl_gl_context;
	};
    
    typedef OpenGLRenderingContextType<float> OpenGLRenderingContext;
    typedef OpenGLRenderingContextType<double> OpenGLRenderingContextd;
    
    template <typename T>
    const int OpenGLRenderingContextType<T>::kNumSupportedOpenGLVersions = 9;
    template <typename T>
    const int OpenGLRenderingContextType<T>::kSupportedOpenGLVersions[9][2] = {{4,4},{4,3},{4,2},
        {4,1},{4,0},{3,3},
        {3,2},{3,1},{3,0}};
}

#endif // #ifndef _opengl_renderingcontext_h