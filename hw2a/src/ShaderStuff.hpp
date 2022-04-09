// Basic OpenGL program
// Based on example code from: Interactive Computer Graphics: A Top-Down Approach with Shader-Based OpenGL (6th Edition), by Ed Angel

#ifndef SHADERSTUFF
#define SHADERSTUFF 1

#define DEBUG_ON 0
#define BUFFER_OFFSET(bytes) ((GLvoid*) (bytes))

// Create a NULL-terminated string by reading the provided file
static char*
readShaderSource(const char* shaderFile)
{
    FILE *fp;
    long length, count;
    char *buffer;
 //   struct stat fileinfo;
    
    // open the file containing the text of the shader code
    fp = fopen(shaderFile, "rb");
    
    // check for errors in opening the file
    if ( fp == NULL ) {
        printf("can't open shader source file %s\n", shaderFile);
        return NULL;
    }
    
    // determine the file size
    fseek(fp, 0, SEEK_END); // move position indicator to the end of the file;
    length = ftell(fp);  // return the value of the current position
    if (DEBUG_ON) fprintf(stdout, "length in bytes of shader file: %ld\n", length);
    
    // allocate a buffer with the indicated number of bytes, plus one
    buffer = new char[length + 1];
    
    // read the appropriate number of bytes from the file
    fseek(fp, 0, SEEK_SET);  // move position indicator to the start of the file
    count = fread(buffer, 1, length, fp); // read all of the bytes
    if (DEBUG_ON) fprintf(stdout, "count of bytes successfully read: %ld\n", count);

    // append a NULL character to indicate the end of the string
    buffer[count] = '\0';    //because on some systems, count != length
    
    // close the file
    fclose(fp);
    
    // return the string
    return buffer;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint
InitShader(const char* vShaderFileName, const char* fShaderFileName)
{	
	GLuint vertex_shader, fragment_shader;
	GLuint program;
	
	// check GLSL version
    if (DEBUG_ON) printf("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	// Create shader handlers
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Read source code from file
	std::string vs_text = readShaderSource(vShaderFileName);
	std::string fs_text = readShaderSource(fShaderFileName);
	
	// error check
	if ( vs_text == "" ) {
		printf("Failed to read from vertex shader file %s\n", vShaderFileName);
		exit( 1 );
	} else if (DEBUG_ON) {
		printf("read shader code:\n%s\n", vs_text.c_str());
	}
	if ( fs_text == "" ) {
		printf("Failed to read from fragment shader file %s\n", fShaderFileName);
		exit( 1 );
	} else if (DEBUG_ON) {
		printf("read shader code:\n%s\n", fs_text.c_str());
	}
	
	// Set shader source
	const char *vv = vs_text.c_str();
	const char *ff = fs_text.c_str();
	glShaderSource(vertex_shader, 1, &vv, NULL);
	glShaderSource(fragment_shader, 1, &ff, NULL);
	
	// Compile shaders
	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);
	
	// Check for errors in compiling shaders
	GLint  compiled;
	glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
		printf("vertex_shader failed to compile\n");
		if (DEBUG_ON) {
			GLint logMaxSize, logLength;
			glGetShaderiv( vertex_shader, GL_INFO_LOG_LENGTH, &logMaxSize );
			printf("printing error message of %d bytes\n", logMaxSize);
			char* logMsg = new char[logMaxSize];
			glGetShaderInfoLog( vertex_shader, logMaxSize, &logLength, logMsg );
			printf("%d bytes retrieved\n", logLength);
			printf("error message: %s\n", logMsg);
			delete[] logMsg;
		}
		exit(1);
	}
	glGetShaderiv( fragment_shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
		printf("fragment_shader failed to compile\n");
		if (DEBUG_ON) {
			GLint logMaxSize, logLength;
			glGetShaderiv( fragment_shader, GL_INFO_LOG_LENGTH, &logMaxSize );
			printf("printing error message of %d bytes\n", logMaxSize);
			char* logMsg = new char[logMaxSize];
			glGetShaderInfoLog( fragment_shader, logMaxSize, &logLength, logMsg );
			printf("%d bytes retrieved\n", logLength);
			printf("error message: %s\n", logMsg);
			delete[] logMsg;
		}
		exit(1);
	}
	
	// Create the program
	program = glCreateProgram();
	
	// Attach shaders to program
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	
    // Link and set program to use
	glLinkProgram(program);
	glUseProgram(program);
	
    return program;
}


#endif
