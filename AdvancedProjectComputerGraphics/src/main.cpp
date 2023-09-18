#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "renderer.h"

#include "vertexBuffer.h"
#include "indexBuffer.h"

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback( int error, const char* description ) {
	fprintf( stderr, "Error: %s\n", description );
}

static void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GLFW_TRUE );
}

int main( void ) {
	GLFWwindow* window;
	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	glfwSetErrorCallback( error_callback );

	if( !glfwInit() )
		exit( EXIT_FAILURE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );

	window = glfwCreateWindow( 640, 480, "Simple example", NULL, NULL );
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
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 2
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	GLCall( glGenVertexArrays( 1, &vao ) );
	GLCall( glBindVertexArray( vao ) );

	VertexBuffer vb( vertices, sizeof( vertices ) );

	IndexBuffer ib( indices, 6 );

	vertex_shader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex_shader, 1, &vertex_shader_text, NULL );
	glCompileShader( vertex_shader );

	fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment_shader, 1, &fragment_shader_text, NULL );
	glCompileShader( fragment_shader );

	program = glCreateProgram();
	glAttachShader( program, vertex_shader );
	glAttachShader( program, fragment_shader );
	glLinkProgram( program );

	mvp_location = glGetUniformLocation( program, "MVP" );
	vpos_location = glGetAttribLocation( program, "vPos" );
	vcol_location = glGetAttribLocation( program, "vCol" );

	glEnableVertexAttribArray( vpos_location );
	glVertexAttribPointer( vpos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( vertices[ 0 ] ), ( void* ) 0 );

	glEnableVertexAttribArray( vcol_location );
	glVertexAttribPointer( vcol_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( vertices[ 0 ] ), ( void* ) ( sizeof( float ) * 3 ) );

	while( !glfwWindowShouldClose( window ) ) {
		float ratio;
		int width, height;
		glm::mat4x4 p, mvp;

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / ( float ) height;

		glViewport( 0, 0, width, height );
		glClear( GL_COLOR_BUFFER_BIT );

		glm::mat4 m( 1.0f );
		m = glm::rotate( m, ( float ) glfwGetTime(), glm::vec3( 1.f ) );
		p = glm::ortho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f );
		mvp = p * m;

		GLCall( glUseProgram( program ) );
		GLCall( glUniformMatrix4fv( mvp_location, 1, GL_FALSE, ( const GLfloat* ) &mvp ) );

		GLCall( glBindVertexArray( vao ) );
		ib.bind();

		GLCall( glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr ) );


		glfwSwapBuffers( window );
		glfwPollEvents();
	}

	glfwDestroyWindow( window );

	glfwTerminate();
	exit( EXIT_SUCCESS );
}
