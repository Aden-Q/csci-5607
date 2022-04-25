// This template code was originally written by Matt Overby while a TA for CSci5607

// The loaders are included by glfw3 (glcorearb.h) if we are not using glew.
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Includes
#include "trimesh.hpp"
#include "shader.hpp"
#include <cstring> // memcpy
#include <math.h>

// Constants
#define WIN_WIDTH 500
#define WIN_HEIGHT 500

class Mat4x4 {
public:

	float m[16];

	Mat4x4(){ // Default: Identity
		m[0] = 1.f;  m[4] = 0.f;  m[8]  = 0.f;  m[12] = 0.f;
		m[1] = 0.f;  m[5] = 1.f;  m[9]  = 0.f;  m[13] = 0.f;
		m[2] = 0.f;  m[6] = 0.f;  m[10] = 1.f;  m[14] = 0.f;
		m[3] = 0.f;  m[7] = 0.f;  m[11] = 0.f;  m[15] = 1.f;
	}

	void make_identity(){
		m[0] = 1.f;  m[4] = 0.f;  m[8]  = 0.f;  m[12] = 0.f;
		m[1] = 0.f;  m[5] = 1.f;  m[9]  = 0.f;  m[13] = 0.f;
		m[2] = 0.f;  m[6] = 0.f;  m[10] = 1.f;  m[14] = 0.f;
		m[3] = 0.f;  m[7] = 0.f;  m[11] = 0.f;  m[15] = 1.f;
	}

	void print(){
		std::cout << m[0] << ' ' <<  m[4] << ' ' <<  m[8]  << ' ' <<  m[12] << "\n";
		std::cout << m[1] << ' ' <<   m[5] << ' ' <<  m[9]  << ' ' <<   m[13] << "\n";
		std::cout << m[2] << ' ' <<   m[6] << ' ' <<  m[10] << ' ' <<   m[14] << "\n";
		std::cout << m[3] << ' ' <<   m[7] << ' ' <<  m[11] << ' ' <<   m[15] << "\n";
	}

	void make_scale(float x, float y, float z){
		make_identity();
		m[0] = x; m[5] = y; m[10] = z;
	}

	void translate(float dx, float dy, float dz){
		m[12] += dx; 
		m[13] += dy;
		m[14] += dz;
	}
};

static inline const Vec3f operator*(const Mat4x4 &m, const Vec3f &v){
	Vec3f r( m.m[0]*v[0]+m.m[4]*v[1]+m.m[8]*v[2],
		m.m[1]*v[0]+m.m[5]*v[1]+m.m[9]*v[2],
		m.m[2]*v[0]+m.m[6]*v[1]+m.m[10]*v[2] );
	return r;
}


//
//	Global state variables
//
namespace Globals {
	double cursorX, cursorY; // cursor positions
	float win_width, win_height; // window size
	float aspect;
	Vec3f eye;	// initial eye/camera position
	Vec3f view_dir;  // initial view direction
	Vec3f up_dir;  // up direction of eye/camera
	Vec3f u, v, n;  // three directions in the camera coordinates
	// parameters for the viewing volume
	float near, far;
	float left, right;
	float bottom, top;
	GLuint verts_vbo[1], colors_vbo[1], normals_vbo[1], faces_ibo[1], tris_vao;
	TriMesh mesh;

	//  Model, view and projection matrices, initialized to the identity
	Mat4x4 model; // not used in this assignment; included for completeness only
	Mat4x4 view;
	Mat4x4 projection;
}


//
//	Callbacks
//
static void error_callback(int error, const char* description){ fprintf(stderr, "Error: %s\n", description); }

// Function to move the view origin
void moveOrigin(char key);

// Function to rotate the viewing direction
void rotateView(char key);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	// Close on escape or Q
	if( action == GLFW_PRESS ){
		switch ( key ) {
			case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE); break;
			case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GL_TRUE); break;
            // ToDo: update the viewing transformation matrix according to key presses
			case GLFW_KEY_W: moveOrigin('W'); break;
			case GLFW_KEY_S: moveOrigin('S'); break;
			case GLFW_KEY_A: moveOrigin('A'); break;
			case GLFW_KEY_D: moveOrigin('D'); break;
			case GLFW_KEY_LEFT_BRACKET: moveOrigin('['); break;
			case GLFW_KEY_RIGHT_BRACKET: moveOrigin(']'); break;
			case GLFW_KEY_LEFT: rotateView('L'); break;
			case GLFW_KEY_RIGHT: rotateView('R'); break;
			case GLFW_KEY_UP: rotateView('U'); break;
			case GLFW_KEY_DOWN: rotateView('D'); break;
		}
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	using namespace Globals;
	win_width = float(width);
	win_height = float(height);
    aspect = win_width / win_height;
	
    glViewport(0, 0, width, height);

	// ToDo: update the perspective matrix as the window size changes
	if (aspect > 1)
	{
		float new_width = aspect * (top - bottom);
		left = -new_width / 2;
		right = new_width / 2;
	}
	else
	{
		float new_height = (right - left) / aspect;
		bottom = -new_height / 2;
		top = new_height / 2;
	}
	// update the perspective projection matrix
	projection.m[0] = 2 * near / (right - left);
	projection.m[5] = 2 * near / (top - bottom);
	projection.m[8] = (right + left) / (right - left);
	projection.m[9] = (top + bottom) / (top - bottom);
}


// Function to set up geometry
void init_scene();


//
//	Main
//
int main(int argc, char *argv[]){

	// Load the mesh
	std::stringstream obj_file; obj_file << MY_DATA_DIR << "sibenik/sibenik.obj";
	if( !Globals::mesh.load_obj( obj_file.str() ) ){ return 0; }
	Globals::mesh.print_details();

	// Set up the window variable
	GLFWwindow* window;
    
    // Define the error callback function
	glfwSetErrorCallback(&error_callback);

	// Initialize glfw
	if( !glfwInit() ){ return EXIT_FAILURE; }

	// Ask for OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the glfw window
	Globals::win_width = WIN_WIDTH;
	Globals::win_height = WIN_HEIGHT;
	window = glfwCreateWindow(int(Globals::win_width), int(Globals::win_height), "HW2b", NULL, NULL);
	if( !window ){ glfwTerminate(); return EXIT_FAILURE; }

	// Define callbacks to handle user input and window resizing
	glfwSetKeyCallback(window, &key_callback);
	glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);

	// More setup stuff
	glfwMakeContextCurrent(window); // Make the window current
    glfwSwapInterval(1); // Set the swap interval

	// make sure the openGL and GLFW code can be found
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to gladLoadGLLoader" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Initialize the shaders
	// MY_SRC_DIR was defined in CMakeLists.txt
	// it specifies the full path to this project's src/ directory.
	mcl::Shader shader;
	std::stringstream ss; ss << MY_SRC_DIR << "shader.";
	shader.init_from_files( ss.str()+"vert", ss.str()+"frag" );

	// Initialize the scene
	init_scene();
	framebuffer_size_callback(window, int(Globals::win_width), int(Globals::win_height)); 

	// Perform some OpenGL initializations
	glEnable(GL_DEPTH_TEST);  // turn hidden surfce removal on
	glClearColor(1.f,1.f,1.f,1.f);  // set the background to white

	// Enable the shader, this allows us to set uniforms and attributes
	shader.enable();

	// Bind buffers
	glBindVertexArray(Globals::tris_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Globals::faces_ibo[0]);
    
	// Game loop
	while( !glfwWindowShouldClose(window) ){

		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Send updated info to the GPU
		glUniformMatrix4fv( shader.uniform("model"), 1, GL_FALSE, Globals::model.m  ); // model transformation (always the identity matrix in this assignment)
		glUniformMatrix4fv( shader.uniform("view"), 1, GL_FALSE, Globals::view.m  ); // viewing transformation
		glUniformMatrix4fv( shader.uniform("projection"), 1, GL_FALSE, Globals::projection.m ); // projection matrix

		// Draw
		glDrawElements(GL_TRIANGLES, Globals::mesh.faces.size()*3, GL_UNSIGNED_INT, 0);

		// Finalize
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // end game loop

	// Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Disable the shader, we're done using it
	shader.disable();
    
	return EXIT_SUCCESS;
}


void init_scene() {

	using namespace Globals;

	// initialize the view origin, view direction, and up direction
	eye[0] = -5;
	eye[1] = -10;
	eye[2] = 0;
	view_dir[0] = -1;
	view_dir[1] = 0;
	view_dir[2] = 0;
	up_dir[0] = 0;
	up_dir[1] = 1;
	up_dir[2] = 0;

	// define the initial viewing transformation
	// composite of translation and rotation
	u = view_dir.cross(up_dir);
	u.normalize();
	v = u.cross(view_dir);
	v.normalize();
	n = view_dir * float(-1);
	n.normalize();
	view.m[0] = u[0];
	view.m[4] = u[1];
	view.m[8] = u[2];
	view.m[1] = v[0];
	view.m[5] = v[1];
	view.m[9] = v[2];
	view.m[2] = n[0];
	view.m[6] = n[1];
	view.m[10] = n[2];
	view.translate(-eye.dot(u), -eye.dot(v), -eye.dot(n));

	// define the initial projection transformation
	// composite of perspective warp and normalization
	near = 6;
	far = 16;
	left = -4;
	right = 4;
	bottom = -4;
	top = 4;
	projection.m[0] = 2 * near / (right - left);
	projection.m[5] = 2 * near / (top - bottom);
	projection.m[8] = (right + left) / (right - left);
	projection.m[9] = (top + bottom) / (top - bottom);
	projection.m[10] = -(far + near) / (far - near);
	projection.m[11] = -1;
	projection.m[14] = -2 * far * near / (far - near);
	projection.m[15] = 0;

	// Create the buffer for vertices
	glGenBuffers(1, verts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, verts_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(mesh.vertices[0]), &mesh.vertices[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create the buffer for colors
	glGenBuffers(1, colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.colors.size()*sizeof(mesh.colors[0]), &mesh.colors[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create the buffer for normals
	glGenBuffers(1, normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh.normals.size()*sizeof(mesh.normals[0]), &mesh.normals[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create the buffer for indices
	glGenBuffers(1, faces_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faces_ibo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.faces.size()*sizeof(mesh.faces[0]), &mesh.faces[0][0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Create the VAO
	glGenVertexArrays(1, &tris_vao);
	glBindVertexArray(tris_vao);

	int vert_dim = 3;

	// location=0 is the vertex
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, verts_vbo[0]);
	glVertexAttribPointer(0, vert_dim, GL_FLOAT, GL_FALSE, sizeof(mesh.vertices[0]), 0);

	// location=1 is the color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo[0]);
	glVertexAttribPointer(1, vert_dim, GL_FLOAT, GL_FALSE, sizeof(mesh.colors[0]), 0);

	// location=2 is the normal
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo[0]);
	glVertexAttribPointer(2, vert_dim, GL_FLOAT, GL_FALSE, sizeof(mesh.normals[0]), 0);

	// Done setting data for the vao
	glBindVertexArray(0);

}

void moveOrigin(char key) {
	using namespace Globals;
	float delta = 0.2;
	if (key == 'W') {
		// move forward
		eye += view_dir * delta;
	}
	else if (key == 'S') {
		// move backwrd
		eye += view_dir * (-delta);
	}
	else if (key == 'A') {
		// move to the left
		eye += u * (-delta);
	}
	else if (key == 'D') {
		// move to the right
		eye += u * delta;
	}
	else if (key == '[') {
		// move down
		eye += up_dir * delta;
	}
	else if (key == ']') {
		// move up
		eye += up_dir * (-delta);
	}
	// update the viewing matrix
	view.m[12] = - eye.dot(u);
	view.m[13] = -eye.dot(v);
	view.m[14] = -eye.dot(n);
}

void rotateView(char key) {
	using namespace Globals;
	float delta = 0.1;
	if (key == 'L' || key == 'R')
	{
		// rotate clockwise, about the y-axis
		if (key == 'R')
			delta = -delta;
		// rotate counterclockwise, about the y-axis
		float new_x = view_dir[0] * cos(delta) + view_dir[2] * sin(delta);
		float new_z = view_dir[2] * cos(delta) - view_dir[0] * sin(delta);
		view_dir[0] = new_x;
		view_dir[2] = new_z;
	}
	else if (key == 'U')
	{
		// rotate up, about the viewing direction
		if (key == 'D')
			delta = -delta;
		// rotate down, about the viewing direction
		
	}

	// update the viewing matrix
	u = view_dir.cross(up_dir);
	u.normalize();
	v = u.cross(view_dir);
	v.normalize();
	n = view_dir * float(-1);
	n.normalize();
	view.m[0] = u[0];
	view.m[4] = u[1];
	view.m[8] = u[2];
	view.m[1] = v[0];
	view.m[5] = v[1];
	view.m[9] = v[2];
	view.m[2] = n[0];
	view.m[6] = n[1];
	view.m[10] = n[2];
	view.m[12] = -eye.dot(u);
	view.m[13] = -eye.dot(v);
	view.m[14] = -eye.dot(n);
}