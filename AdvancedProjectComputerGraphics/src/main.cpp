#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 0
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 1
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 2
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 3

		 0.1f,  0.6f, -0.2f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 4
		 0.1f, -0.6f, -0.2f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 5
		-0.1f, -0.6f, -0.2f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 6
		-0.1f,  0.6f, -0.2f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 7
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4
	};

	GLCall( glEnable( GL_BLEND ) );
	GLCall( glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) );

	VertexArray va;
	VertexBuffer vb( vertices, sizeof( vertices ) );
	VertexBufferLayout layout;

	layout.push<float>( 3 );
	layout.push<float>( 3 );
	layout.push<float>( 2 );
	va.addBuffer( vb, layout );
	va.bind();

	IndexBuffer ib( indices, sizeof( indices ) );

	Shader mainShader( "res/shaders/template.vs", "res/shaders/template.fs" );

	Texture texture1( "res/textures/nether_brick.png" );
	texture1.bind( 0 );


	Renderer renderer;

	glPolygonMode( GL_FRONT, GL_FILL );

	while( !glfwWindowShouldClose( window ) ) {
		float ratio;
		int width, height;
		glm::mat4x4 projection, model, view;

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / ( float ) height;

		glViewport( 0, 0, width, height );

		renderer.clear();

		glm::mat4 m( 1.0f );
		model = glm::rotate( m, ( float ) glfwGetTime(), glm::vec3( 1.f ) );
		projection = glm::ortho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f );


		mainShader.bind();
		mainShader.SetuniformsMat4f( "model", model );
		mainShader.setUniform1i( "ourTexture", 0 );

		renderer.draw( va, ib, mainShader );


		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );

	glfwTerminate();
	exit( EXIT_SUCCESS );
}
