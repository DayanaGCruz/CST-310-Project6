#include <iostream>
#include <cmath>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// FreeType .ttf font loading
#include <ft2build.h>
#include "TextRenderer.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include <filesystem>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 1000;

// Camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
bool keys[1024];
GLuint VBO, containerVAO;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

 // Initialize TextRenderer to render text on the screen



// This creates the cubes depending on their positions, shiny values, vertices,
// angle, and rotation.
void cube(float lightPosX, float lightPosY, float lightPosZ, float lightViewLocX, float lightViewLocY, float lightViewLocZ, int shinyValue, float vertex1, float vertex2, float vertex3, float angleDegree, float rotateY)
{
  Shader lightingShader("lighting.vs", "lighting.frag");
  Shader textShader("text.vs", "text.fs");
  TextRenderer textRenderer("font.ttf", WIDTH, HEIGHT);
  lightingShader.Use();
  GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
  GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
  GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
  GLint lightViewLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
  GLint Shiness = glGetUniformLocation(lightingShader.Program, "shiny");
  
  glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
  glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
          
  // change position of the light object
  glUniform3f(lightPosLoc, lightPosX, lightPosY, lightPosZ);
          
  // change location of the dot 
  glUniform3f(lightViewLoc, lightViewLocX, lightViewLocY, lightViewLocZ);
          
  // change value of shiny on cube
  glUniform1fARB(Shiness, shinyValue);
          
  // Camera/View transformation
  glm::mat4 view;
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
          
  // Projection 
  glm::mat4 projection;
  projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
          
  // Get the uniform locations
  GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
  GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
  GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
          
  // Pass the matrices to the shader
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
  glBindVertexArray(containerVAO);
          
  // Calculate the model matrix for each object and pass it to shader before drawing
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(vertex1, vertex2, vertex3));
  GLfloat angle = angleDegree;
  model = glm::rotate(model, angle, glm::vec3(0.0f, rotateY, 0.0f));
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Convert the cube position to screen coordinates
glm::vec4 pos = projection * view * model * glm::vec4(0.0f, -0.6f, 0.0f, 1.0f);
pos /= pos.w; // Perspective divide

float screenX = (pos.x * 0.5f + 0.5f) * WIDTH;
float screenY = (pos.y * -0.5f + 0.5f) * HEIGHT;

// Adjust screenY to position the text below the cube
float textOffset = 300.0f;
screenY -= textOffset;

std::string text = std::to_string(shinyValue);
textRenderer.RenderText(textShader, text, screenX, screenY, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
}   

// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Project 6", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = 
    {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
  
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(containerVAO);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Set the vertex attributes (only position data for the lamp))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
          // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();

        // Render
        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // Dark gray background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind Textures using texture units
        
        cube(0.7f, -2.2f, 1.8f, -1.2f, -0.6f, 0.0f, 32, -2.5f, -1.2f, -2.0f, 41.7f, -0.4f);
        cube(0.7f, 2.2f, 1.8f, -1.0f, 1.0f, 0.0f, 2, -2.5f, 1.2f, -2.0f, 41.7f, -0.4f);
        cube(0.7f, 2.2f, 1.0f, 0.3f, 0.9f, 0.0f, 4, -0.8f, 1.2f, -2.0f, 41.9f, -0.9f);
        cube(1.7f, 2.2f, 1.0f, 1.3f, 0.8f, 0.0f, 8, 0.8f, 1.2f, -2.0f, 42.1f, -0.9f);
        cube(2.7f, 2.2f, 1.0f, 2.5f, 0.7f, 0.0f, 16, 2.3f, 1.2f, -2.0f, 42.2f, -0.9f);
        cube(1.7f, -2.2f, 1.8f, 0.0f, -0.6f, 0.0f, 64, -0.8f, -1.2f, -2.0f, 41.9f, -0.4f);
        cube(1.7f, -2.2f, 1.8f, 1.1f, -0.6f, 0.0f, 128, 0.8f, -1.2f, -2.0f, 42.1f, -0.4f);
        cube(2.7f, -2.2f, 1.8f, 2.5f, -0.6f, 0.0f, 256, 2.3f, -1.2f, -2.0f, 42.2f, -0.4f);

        glBindVertexArray(0);
        // Swap the screen buffers
        glfwSwapBuffers(window);                   
    
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &containerVAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 0.01f;
    if (keys[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
