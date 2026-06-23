#pragma once

#include "../src/lavish_core.h"

namespace lavish {

	namespace detail {

		

	}

	void Initialize_ImplOpenGL( ) {

		detail::gContext.mBackendPollEventsFn = PollDrawList_ImplOpenGL;
		

	}

	void PollDrawList_ImplOpenGL( float screenWidth, float screenHeight ) {
		
		auto& drawList = detail::gContext.mMainDrawList;

		GLuint vao = detail::gContext.mRenderBasis.mVao;
		GLuint vbo = detail::gContext.mRenderBasis.mVbo;
		GLuint ebo = detail::gContext.mRenderBasis.mEbo;
		GLuint program = detail::gContext.mRenderBasis.mShader;

		glUseProgram( program );

		glProgramUniform2f( program, 0, screenWidth, screenHeight );

		glNamedBufferSubData( vbo, 0, drawList.mVertexBuffer.size( ) * sizeof( detail::Vertex ), drawList.mVertexBuffer.data( ) );
		glNamedBufferSubData( ebo, 0, drawList.mIndexBuffer.size( ) * sizeof( uint32_t ), drawList.mIndexBuffer.data( ) );

		glBindVertexArray( vao );
		glDrawElements( GL_TRIANGLES, static_cast< GLsizei >(drawList.mIndexBuffer.size( )), GL_UNSIGNED_INT, 0 );
		
	}

}