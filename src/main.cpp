#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

// Callback function for resetting viewport after window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

// Function to process input
void processInput(GLFWwindow *window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

int main() {

    // Initializing GLFW Window
    glfwInit();
    // Setting the OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Setting the OpenGL profile to core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating a window object
    GLFWwindow* window = glfwCreateWindow(800,600,"LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Making the window the current/main context
    glfwMakeContextCurrent(window);

    // Initializing GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setting the viewport
    glViewport(0,0,800,600);

    // Tell GLFW to call this function on every window resize by registering it
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Declaring three vertecies for a triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Creating a Vertex Buffer Object (VBO) to store the vertecies
    unsigned int VBO;
    // Generating a buffer
    glGenBuffers(1, &VBO);
    // Binding the buffer to the GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copying the vertecies into the buffer
    /*
        glBufferData is used to copy user-defined data into the currently bound buffer.
        Parameters:
        1. The type of buffer we want to copy the data into
        2. The size of the data in bytes ( sizeof(data) )
        3. The actual data
        4. How we want the graphics card to manage the data,
           this can take 3 forms:
            - GL_STREAM_DRAW: The data is set once and used by the GPU at MOST a few times.
            - GL_STATIC_DRAW: The data is set once (will not change) and isused many times.
            - GL_DYNAMIC_DRAW: The data is changed a lot and used many times.
    */
    glBufferData(GL_ARRAY_BUFFER,
                sizeof(vertices),
                vertices,
                GL_STATIC_DRAW);
    
    // Creating vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attaching the shader source to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compiling the shader
    glCompileShader(vertexShader);
    
    // Checking if the shader compiled successfully
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Creating and compiling fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Checking if the shader compiled successfully
    int  success2;
    char infoLog2[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);
    if(!success2) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
    }

    // Creating a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // Attaching the shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Linking the program
    glLinkProgram(shaderProgram);

    // Checking if the shader program linked successfully
    int success3;
    char infoLog3[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);
    if(!success3) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog3 << std::endl;
    }

    // Activating the shader program
    glUseProgram(shaderProgram);
    // Every shader and rendering call after glUseProgram will now use this program object,
    // and thus the shaders.

    // Deleting the shader objects after linking them to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    /* Uncollapse to see in-depth explanation of the following code  

    ----- Linking Vertex Attributes -----
    Our vertex buffer data is formatted as follows:
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
            - The position data is stored as 32-bit (4 byte) floating point values.
            - There are 3 values for each vertex.
            - There is no space between each set of 3 values. The values are tightly packed in the array.
            - The first value is at the beginning of the array.

    -------------------------------------

    With this knowledge we can tell OpenGL how to interpret 
    the vertex data using glVertexAttribPointer.

    glVertexAttribPointer has a few parameters:
        - The first parameter specifies which vertex attribute 
        we want to configure. Remember that we specified the 
        location of the position vertex attribute in the 
        vertex shader with layout (location = 0). This sets 
        the location of the vertex attribute to 0 and since 
        we want to pass data to this vertex attribute, we 
        pass in 0.
        
        - The next argument specifies the size of the vertex 
        attribute. The vertex attribute is a vec3 so it is 
        composed of 3 values.

        - The third argument specifies the type of the data 
        which is GL_FLOAT (a vec* in GLSL consists of floating 
        point values).

        - The next argument specifies if we want the data to be 
        normalized. If we're inputting integer data types (int, 
        byte) and we've set this to GL_TRUE, the integer data is 
        normalized to 0 (or -1 for signed data) and 1 when 
        converted to float. This is not relevant for us so we'll 
        leave this at GL_FALSE.

        - The fifth argument is known as the stride and tells us 
        the space between consecutive vertex attributes. Since the 
        next set of position data is located exactly 3 times the 
        size of a float away we specify that value as the stride. 
        Note that since we know that the array is tightly packed 
        (there is no space between the next vertex attribute value) 
        we could've also specified the stride as 0 to let OpenGL 
        determine the stride (this only works when values are 
        tightly packed). Whenever we have more vertex attributes 
        we have to carefully define the spacing between each vertex 
        attribute but we'll get to see more examples of that later 
        on.

        - The last parameter is of type void* and thus requires that 
        weird cast. This is the offset of where the position data 
        begins in the buffer. Since the position data is at the 
        start of the data array this value is just 0. We will 
        explore this parameter in more detail later on.

    */

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

    // Render Loop, to keep window open until it is told to close
    while(!glfwWindowShouldClose(window)) {

        /* 
        We want to place all render events within
        this render loop   
        */

        // Clear the frame at start of each iteration
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Change color with these parameters
        glClear(GL_COLOR_BUFFER_BIT);

        // Process input
        processInput(window);

        // Polling for events
        glfwSwapBuffers(window);
        // Swapping the color buffer
        glfwPollEvents();

    }

    // Terminating GLFW
    glfwTerminate();
    return 0;
}