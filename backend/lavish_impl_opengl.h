#pragma once
#include "glad/glad.h"
#include "../src/lavish_core.h"

namespace lavish {

	namespace detail {

		struct RenderBasis_OpenGLImpl {

			GLuint mVbo{};
			GLuint mVao{};
			GLuint mEbo{};
			GLuint mShader{};

		};

		RenderBasis_OpenGLImpl gRenderBasis_OpenGLImpl{};

	}

	void PollDrawList_ImplOpenGL( float screenWidth, float screenHeight ) {

		auto& drawList = detail::gContext.mMainDrawList;

		GLuint vao = detail::gRenderBasis_OpenGLImpl.mVao;
		GLuint vbo = detail::gRenderBasis_OpenGLImpl.mVbo;
		GLuint ebo = detail::gRenderBasis_OpenGLImpl.mEbo;
		GLuint program = detail::gRenderBasis_OpenGLImpl.mShader;

		glUseProgram( program );

		glProgramUniform2f( program, 0, screenWidth, screenHeight );

		glNamedBufferSubData( vbo, 0, drawList.mVertexBuffer.size( ) * sizeof( detail::Vertex ), drawList.mVertexBuffer.data( ) );
		glNamedBufferSubData( ebo, 0, drawList.mIndexBuffer.size( ) * sizeof( uint32_t ), drawList.mIndexBuffer.data( ) );

		glBindVertexArray( vao );
		glDrawElements( GL_TRIANGLES, static_cast< GLsizei >(drawList.mIndexBuffer.size( )), GL_UNSIGNED_INT, 0 );

	}

	void Initialize_ImplOpenGL( ) {

		detail::gContext.mBackendPollEventsFn = PollDrawList_ImplOpenGL;
		
		GLuint& vbo = detail::gRenderBasis_OpenGLImpl.mVbo;
		GLuint& vao = detail::gRenderBasis_OpenGLImpl.mVao;
		GLuint& ebo = detail::gRenderBasis_OpenGLImpl.mEbo;
		GLuint& program = detail::gRenderBasis_OpenGLImpl.mShader;

		glCreateBuffers( 1, &vbo );
		glNamedBufferStorage( vbo, 15000, nullptr, GL_DYNAMIC_STORAGE_BIT );

		glCreateBuffers( 1, &ebo );
		glNamedBufferStorage( ebo, 15000, nullptr, GL_DYNAMIC_STORAGE_BIT );

		glCreateVertexArrays( 1, &vao );
		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( detail::Vertex ) );

		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribBinding( vao, 0, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof( detail::Vertex, mX ) );

		glEnableVertexArrayAttrib( vao, 1 );
		glVertexArrayAttribBinding( vao, 1, 0 );
		glVertexArrayAttribFormat( vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof( detail::Vertex, mU ) );

		glEnableVertexArrayAttrib( vao, 2 );
		glVertexArrayAttribBinding( vao, 2, 0 );
		glVertexArrayAttribFormat( vao, 2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof( detail::Vertex, mColor ) );

		glVertexArrayElementBuffer( vao, ebo );

		program = glCreateProgram( );

		GLuint vs = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vs, 1, &detail::gVertexShaderSource, nullptr );
		glCompileShader( vs );

		GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fs, 1, &detail::gFragmentShaderSource, nullptr );
		glCompileShader( fs );

		glAttachShader( program, vs );
		glAttachShader( program, fs );

		glLinkProgram( program );

		glDeleteShader( fs );
		glDeleteShader( vs );

	}

}