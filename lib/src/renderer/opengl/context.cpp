#include "se/renderer/opengl/context.hpp"

#include <map>
#include <string>

#include "se/logger.hpp"
#include "se/exceptions.hpp"

#define SET_ATTRIBUTE(attr, val) if (SDL_GL_SetAttribute(SDL_GL_##attr, val) != 0)\
	throw se::exceptions::RuntimeError("Can't set SDL2 opengl attribute " #attr " : " + std::string(SDL_GetError()))



namespace se::renderer::opengl {
	Context::Context(const se::renderer::ContextInfos &infos) :
		se::renderer::Context(infos),
		m_context {nullptr}
	{
		SET_ATTRIBUTE(CONTEXT_MAJOR_VERSION, 4);
		SET_ATTRIBUTE(CONTEXT_MINOR_VERSION, 6);
		SET_ATTRIBUTE(RED_SIZE, 8);
		SET_ATTRIBUTE(GREEN_SIZE, 8);
		SET_ATTRIBUTE(BLUE_SIZE, 8);
		SET_ATTRIBUTE(DEPTH_SIZE, 0);
		SET_ATTRIBUTE(STENCIL_SIZE, 0);
		SET_ATTRIBUTE(CONTEXT_PROFILE_MASK, GL_CONTEXT_CORE_PROFILE_BIT);
		#ifndef NDEBUG
			SET_ATTRIBUTE(CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		#endif

		m_context = SDL_GL_CreateContext(m_infos.linkedWindow->getInternalObject());
		if (!m_context)
			throw se::exceptions::RuntimeError("Can't create an SDL2 opengl context : " + std::string(SDL_GetError()));

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			throw se::exceptions::RuntimeError("Can't load OpenGL functions");

		#ifndef NDEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(s_debugMessageCallback, nullptr);
		#endif
	}


	Context::~Context() {
		if (m_context != nullptr)
			SDL_GL_DeleteContext(m_context);
	}


	void Context::handleWindowSizeChanged() {

	}


	void APIENTRY Context::s_debugMessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *msg,
		const void *userData
	) {
		static std::map<GLenum, std::string> sourceMap {
			{GL_DEBUG_SOURCE_API,             "API"},
			{GL_DEBUG_SOURCE_APPLICATION,     "app"},
			{GL_DEBUG_SOURCE_OTHER,           "other"},
			{GL_DEBUG_SOURCE_SHADER_COMPILER, "shader"},
			{GL_DEBUG_SOURCE_THIRD_PARTY,     "third"},
			{GL_DEBUG_SOURCE_WINDOW_SYSTEM,   "window"}
		};

		static std::map<GLenum, std::string> typeMap {
			{GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "deprecated"},
			{GL_DEBUG_TYPE_ERROR,               "error"},
			{GL_DEBUG_TYPE_MARKER,              "marker"},
			{GL_DEBUG_TYPE_OTHER,               "other"},
			{GL_DEBUG_TYPE_PERFORMANCE,         "performance"},
			{GL_DEBUG_TYPE_POP_GROUP,           "pop"},
			{GL_DEBUG_TYPE_PORTABILITY,         "portability"},
			{GL_DEBUG_TYPE_PUSH_GROUP,          "push"},
			{GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,  "undefined"}
		};

		static std::map<GLenum, se::LogSeverity> severityMap {
			{GL_DEBUG_SEVERITY_NOTIFICATION, se::LogSeverity::eVerbose},
			{GL_DEBUG_SEVERITY_LOW,          se::LogSeverity::eInfo},
			{GL_DEBUG_SEVERITY_MEDIUM,       se::LogSeverity::eWarning},
			{GL_DEBUG_SEVERITY_HIGH,         se::LogSeverity::eError}
		};

		std::string message {msg, length};

		SE_LOGGER.log({severityMap[severity]}, "[{}] {} > {}", sourceMap[source], typeMap[type], message);
	}


} // namespace se::renderer::opengl
