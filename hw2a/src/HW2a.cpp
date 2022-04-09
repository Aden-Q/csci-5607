// Skeleton code for hw2a
// Based on example code from: Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL (6th Edition), by Ed Angel


#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>

#define DEBUG_ON 0  // repetitive of the debug flag in the shader loading code, included here for clarity only

// This file contains the code that reads the shaders from their files and compiles them
#include "ShaderStuff.hpp"

//----------------------------------------------------------------------------

// initialize some basic structure types
typedef struct {
    GLfloat x, y;
} FloatType2D;

typedef struct {
    GLfloat r, g, b;
} ColorType3D;

GLfloat M[16]; // general transformation matrix

// define some assorted global variables, to make life easier
GLint m_location;
GLdouble mouse_x, mouse_y;
GLint window_width = 500;
GLint window_height = 500;
GLdouble pi = 4.0*atan(1.0);

GLFWcursor *hand_cursor, *arrow_cursor;


//----------------------------------------------------------------------------
// function that is called whenever an error occurs
static void
error_callback(int error, const char* description){
    fputs(description, stderr);  // write the error description to stderr
}

//----------------------------------------------------------------------------
// function that is called whenever a keyboard event occurs; defines how keyboard input will be handled
static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // checks to see if the escape key was pressed
    glfwSetWindowShouldClose(window, GL_TRUE);  // closes the window
}

//----------------------------------------------------------------------------
// function that is called whenever a mouse or trackpad button press event occurs
static void
mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    
    // Check which mouse button triggered the event, e.g. GLFW_MOUSE_BUTTON_LEFT, etc.
    // and what the button action was, e.g. GLFW_PRESS, GLFW_RELEASE, etc.
    // (Note that ordinary trackpad click = mouse left button)
    // Also check if any modifier keys were active at the time of the button press, e.g. GLFW_MOD_ALT, etc.
    // Take the appropriate action, which could (optionally) also include changing the cursor's appearance

}

//----------------------------------------------------------------------------
// function that is called whenever a cursor motion event occurs
static void
cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    
    // determine the direction of the mouse or cursor motion
    // update the current mouse or cursor location
    //  (necessary to quantify the amount and direction of cursor motion)
    // take the appropriate action
    
}

//----------------------------------------------------------------------------

void init( void )
{
    ColorType3D colors[4];
    FloatType2D vertices[4];
    GLuint vao[1], buffer, program, location1, location2;
  
    // hard code the geometry of interest: 4 base colors and 4 corners of a square
    // this part can be customized to read in an object description from a file
    colors[0].r = 1;  colors[0].g = 1;  colors[0].b = 1;  // white
    colors[1].r = 1;  colors[1].g = 1;  colors[1].b = 0;  // yellow
    colors[2].r = 0;  colors[2].g = 1;  colors[2].b = 0;  // green
    colors[3].r = 0;  colors[3].g = 0;  colors[3].b = 1;  // blue
    
    vertices[0].x = -0.5;  vertices[0].y = -0.5; // lower left
    vertices[1].x =  0.5;  vertices[1].y = -0.5; // lower right
    vertices[2].x =  0.5;  vertices[2].y =  0.5; // upper right
    vertices[3].x = -0.5;  vertices[3].y =  0.5; // upper left
    
    // Create and bind a vertex array object
    glGenVertexArrays( 1, vao );
    glBindVertexArray( vao[0] );

    // Create and initialize a buffer object large enough to hold both vertex position and color data
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(colors), vertices, GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    
    // Define the names of the shader files
    std::stringstream vshader, fshader;
    vshader << SRC_DIR << "/vshader2a.glsl";
    fshader << SRC_DIR << "/fshader2a.glsl";
    
    // Load the shaders and use the resulting shader program
    program = InitShader( vshader.str().c_str(), fshader.str().c_str() );
    
    // Determine locations of the necessary attributes and matrices used in the vertex shader
    location1 = glGetAttribLocation( program, "vertex_position" );
    glEnableVertexAttribArray( location1 );
    glVertexAttribPointer( location1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    location2 = glGetAttribLocation( program, "vertex_color" );
    glEnableVertexAttribArray( location2 );
    glVertexAttribPointer( location2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)) );
    m_location = glGetUniformLocation( program, "M");
    
    // Define static OpenGL state variables
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white, opaque background
    
    // Define some GLFW cursors (in case you want to dynamically change the cursor's appearance)
    // If you want, you can add more cursors, or even define your own cursor appearance
    arrow_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    hand_cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    
}

//----------------------------------------------------------------------------

int main(int argc, char** argv) {

    int i;
    GLFWwindow* window;

    // Define the error callback function
    glfwSetErrorCallback(error_callback);
    
    // Initialize GLFW (performs platform-specific initialization)
    if (!glfwInit()) exit(EXIT_FAILURE);
    
    // Ask for OpenGL 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Use GLFW to open a window within which to display your graphics
	window = glfwCreateWindow(window_width, window_height, "HW2a", NULL, NULL);
	
    // Verify that the window was successfully created; if not, print error message and terminate
    if (!window)
	{
        printf("GLFW failed to create window; terminating\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
	}
    
	glfwMakeContextCurrent(window); // makes the newly-created context current
    
    // Load all OpenGL functions (needed if using Windows)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("gladLoadGLLoader failed; terminating\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    
	glfwSwapInterval(1);  // tells the system to wait for the rendered frame to finish updating before swapping buffers; can help to avoid tearing

    // Define the keyboard callback function
    glfwSetKeyCallback(window, key_callback);
    // Define the mouse button callback function
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // Define the mouse motion callback function
    glfwSetCursorPosCallback(window, cursor_pos_callback);

	// Create the shaders and perform other one-time initializations
	init();

	// event loop
    while (!glfwWindowShouldClose(window)) {

        // fill/re-fill the window with the background color
		glClear( GL_COLOR_BUFFER_BIT );
        
        // define/re-define the modelview matrix. In this template, we define M to be the identity matrix; you will need define M according to the user's actions.
        for (i=0; i<16; i++) {
            M[i] = (i%5==0);
        }
        
        // sanity check that your matrix contents are what you expect them to be
        if (DEBUG_ON) printf("M = [%f %f %f %f\n     %f %f %f %f\n     %f %f %f %f\n     %f %f %f %f]\n",M[0],M[4],M[8],M[12], M[1],M[5],M[9],M[13], M[2],M[6],M[10],M[14], M[3],M[7],M[11],M[15]);
        
        glUniformMatrix4fv( m_location, 1, GL_FALSE, M );   // send the updated model transformation matrix to the GPU
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );    // draw a triangle between the first vertex and each successive vertex pair in the hard-coded, 4-vertex model
		glFlush();	// ensure that all OpenGL calls have executed before swapping buffers

        glfwSwapBuffers(window);  // swap buffers
        glfwWaitEvents(); // wait for a new event before re-drawing

	} // end graphics loop

	// Clean up
	glfwDestroyWindow(window);
	glfwTerminate();  // destroys any remaining objects, frees resources allocated by GLFW
	exit(EXIT_SUCCESS);

} // end main


