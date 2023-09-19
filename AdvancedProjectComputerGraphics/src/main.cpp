#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

static void error_callback( int error, const char* description ) {
	fprintf( stderr, "Error: %s\n", description );
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GLFW_TRUE );
}

int main( void ) {
	GLFWwindow* window;

	glfwSetErrorCallback( error_callback );

	if( !glfwInit() )
		exit( EXIT_FAILURE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );

	window = glfwCreateWindow( 640, 480, "Advanced Example", NULL, NULL );
	if( !window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetKeyCallback( window, key_callback );

	glfwMakeContextCurrent( window );
	gladLoadGL();
	glfwSwapInterval( 1 );

	float vertices[] = {
		// Positions		// Colors
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 0
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 1
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // 2
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f  // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray va;
	VertexBuffer vb( vertices, sizeof( vertices ) );
	VertexBufferLayout layout;

	layout.push<float>( 3 );
	layout.push<float>( 3 );
	va.addBuffer( vb, layout );
	va.bind();

	IndexBuffer ib( indices, 6 );

	Shader mainShader( "res/shaders/template.vs", "res/shaders/template.fs" );

	while( !glfwWindowShouldClose( window ) ) {
		float ratio;
		int width, height;
		glm::mat4x4 projection, model, view;

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / ( float ) height;

		glViewport( 0, 0, width, height );
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		glm::mat4 m( 1.0f );
		model = glm::rotate( m, ( float ) glfwGetTime(), glm::vec3( 1.f ) );
		projection = glm::ortho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f );


		mainShader.bind();
		mainShader.SetuniformsMat4f( "model", model );
		va.bind();
		ib.bind();

		GLCall( glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr ) );


		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );

	glfwTerminate();
	exit( EXIT_SUCCESS );
}
