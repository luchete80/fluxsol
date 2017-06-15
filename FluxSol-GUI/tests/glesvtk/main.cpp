http://malideveloper.arm.com/sample-code/simple-opengl-es-2-0-example/

const float triangleVertices[] =
{
     0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
};

	
	
2
3
4
5
6
const float triangleColors[] =
{
    1.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0,
};


bool setupGraphics(int width, int height)
{
    /* Full paths to the shader files */
    string vertexShaderPath = "Triangle_triangle.vert"; 
    string fragmentShaderPath = "Triangle_triangle.frag";
 
    GLuint vertexShaderID = 0;
    GLuint fragmentShaderID = 0;
 
    /* Initialize OpenGL ES. */
    GL_CHECK(glEnable(GL_BLEND));
    /* Should do: src * (src alpha) + dest * (1-src alpha). */
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
 
    /* Process shaders. */
    Shader::processShader(&vertexShaderID, vertexShaderPath.c_str(), GL_VERTEX_SHADER);
    Shader::processShader(&fragmentShaderID, fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);
 
    programID = GL_CHECK(glCreateProgram());
    if (programID == 0)
    {
        //Could not create Program error
        return false;
    }
 
    GL_CHECK(glAttachShader(programID, vertexShaderID));
    GL_CHECK(glAttachShader(programID, fragmentShaderID));
    GL_CHECK(glLinkProgram(programID));
    GL_CHECK(glUseProgram(programID));
 
    /* Positions. */
    GL_CHECK(iLocPosition = glGetAttribLocation(programID, "a_v4Position"));
    /* Fill colors. */
    GL_CHECK(iLocFillColor = glGetAttribLocation(programID, "a_v4FillColor"));
 
    GL_CHECK(glViewport(0, 0, width, height));
 
    /* Set clear screen color. */
    GL_CHECK(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
 
    return true;
}