//
//  main.cpp
//  ShuttleVis
//
//  Created by Steven Wang on 7/23/18.
//  Copyright © 2018 Steven Wang. All rights reserved.
//

#define GLEW_STATIC
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Globals
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

class TestVis {
public:
    void initGLFW() {
        // Init GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed
        
        if (nullptr == window)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            
            //return EXIT_FAILURE;
            exit(1);
        }
        
        glfwMakeContextCurrent(window);
        
        glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
        
        // Set the required callback functions
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetCursorPosCallback(window, MouseCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        
        // Options, removes the mouse cursor for a more immersive experience
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
        glewExperimental = GL_TRUE;
        // Initialize GLEW to setup the OpenGL Function pointers
        if (GLEW_OK != glewInit())
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            //return EXIT_FAILURE;
            exit(1);
        }
        
        // Define the viewport dimensions
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Setup some OpenGL options
        glEnable(GL_DEPTH_TEST);
        
        // Enable alpha support
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_TEXTURE_2D);
    }
    
    void initVertices() {
        // Generate objects
        glGenVertexArrays(1, &VAO);
        glGenVertexArrays(1, &VAO2);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &VBO2);
        
        // Set up our vertex data (and buffer(s)) and attribute pointers
        GLfloat cubeVertices[] =
        {
            // Postion           // Color
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            
            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f
        };
        
        // Vertex array and buffer object for the cubes
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
        glEnableVertexAttribArray(0);
        
        // Color attribute
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
        glEnableVertexAttribArray( 1 );
        glBindVertexArray(0);
        
        GLfloat mapVertices[] =
        {
            // Position              // Color           // Texture
            200.0f, -0.5f, 200.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // First triangle
            -200.0f, -0.5f, -200.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            200.0f, -0.5f, -200.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            
            200.0f, -0.5f, 200.0f,   0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Second triangle
            -200.0f, -0.5f, 200.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
            -200.0f, -0.5f, -200.0f, 0.0f, 0.0f, 0.0f,  1.0f, 1.0f
        };
        
        // Vertex array and buffer objects for the map
        glBindVertexArray(VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), mapVertices, GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);
        
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
        // Texture Coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)( 6 * sizeof(GLfloat)));
        glEnableVertexAttribArray( 2 );
        
        // Unbind VAO2
        glBindVertexArray(0);
    }
    
    void loadTexture() {
        // Generate texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
        // Set our texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Load, create texture and generate mipmaps
        image = SOIL_load_image("resources/images/annarbor.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    }

    
    void gameLoop() {
        // Setup and compile our shaders
        Shader cubeShader("resources/shaders/core.vs", "resources/shaders/core.frag");
        Shader mapShader("resources/shaders/core2.vs", "resources/shaders/core2.frag");
        
        // Position of cubes
        glm::vec3 cubePositions[] =
        {
            glm::vec3(-13.0f, 0.0f, -15.0f),
            glm::vec3(-20.0f, 0.0f, -9.0f),
            glm::vec3(-33.0f, 0.0f, -13.0f),
            glm::vec3(-50.0f, 0.0f, -50.0f),
            glm::vec3(-65.0f, 0.0f, -62.0f),
        };
        
        // Game loop
        while(!glfwWindowShouldClose(window)) {
            // Set frame time
            GLfloat currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            // Check and call events
            glfwPollEvents();
            DoMovement();
            
            // Clear the colorbuffer
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            cubeShader.Use();
            
            glm::mat4 projection(1);
            projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
            
            // Create camera transformation
            glm::mat4 view(1);
            view = camera.GetViewMatrix();
            
            // Get the uniform locations
            GLint modelLoc = glGetUniformLocation(cubeShader.Program, "model");
            GLint viewLoc = glGetUniformLocation(cubeShader.Program, "view");
            GLint projLoc = glGetUniformLocation(cubeShader.Program, "projection");
            
            // Pass the matrices to the shader
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glBindVertexArray(VAO);
            
            // Calculate the model matrix for each object and pass it to shader before drawing
            for(GLuint i = 0; i < 14; i++) {
                glm::mat4 model(1);
                model = glm::translate(model, cubePositions[i]);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            
            mapShader.Use();
            
            // Bind Textures using texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(glGetUniformLocation(mapShader.Program, "ourTexture1"), 0);
            
            // Get the uniform locations
            modelLoc = glGetUniformLocation(mapShader.Program, "model");
            viewLoc = glGetUniformLocation(mapShader.Program, "view");
            projLoc = glGetUniformLocation(mapShader.Program, "projection");
            
            // Pass the matrices to the shader
            glm::mat4 model(1);
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
            
            
            // Swap the buffers
            glfwSwapBuffers(window);
        }
        
        // Properly de-allocate all resources once they've outlived their purpose
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO2);
        glDeleteBuffers(1, &VBO2);
        glfwTerminate();
    }
    
private:
    int width;
    int height;
    unsigned char *image;
    GLFWwindow* window;
    GLfloat vertices;
    GLuint VBO;
    GLuint VBO2;
    GLuint VAO;
    GLuint VAO2;
    GLuint texture;
};

// The MAIN function, from here we start our application and run our Game loop
int main() {
    TestVis test;
    test.initGLFW();
    test.initVertices();
    test.loadTexture();
    test.gameLoop();
    return EXIT_SUCCESS;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
    // Camera controls
    if(keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if(keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024) {
        if(action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if(action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos) {
    if(firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
}


void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    camera.ProcessMouseScroll(yOffset);
}








