#include "RenderingContext.h"

#include <stdio.h>

namespace Renderer
{
	RenderingContext * RenderingContext::Create(Window * window)
	{
		if (window == nullptr)
			return nullptr;

		RenderingContext * rendering_context = new RenderingContext();
		rendering_context->_window = window;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		rendering_context->_sdl_gl_context = SDL_GL_CreateContext(window->_sdl_window);

		if (rendering_context->_sdl_gl_context == NULL)
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

		GLint major_version = -1;
		GLint minor_version = -1;

		glGetIntegerv(GL_MAJOR_VERSION, &major_version);
		glGetIntegerv(GL_MINOR_VERSION, &minor_version);

		fprintf(stdout, "OpenGL version: %d.%d\n", major_version, minor_version);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		return rendering_context;
	}

	RenderingContext::RenderingContext()
	{

	}

	RenderingContext::~RenderingContext()
	{
		SDL_GL_DeleteContext(_sdl_gl_context);
		delete _window;
		SDL_Quit();
	}
}