#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <learnopengl/filesystem.h>
#include <rg/Shader.h>
#include <rg/Texture2D.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Vectors of camera
glm::vec3 cameraPos = glm::vec3(0.0, 0.7, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

//timing
float delta_time = 0.0f;
float last_frame = 0.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float pyramid[] = {
//        -0.5, 0.0, -0.5, 0.0, 0.0, //bottom-left 0
//        -0.5, 0.0, 0.5, 1.0, 0.0,//bottom-right 1
//        0.5, 0.0, 0.5, 0.0, 1.0,//top-right 2

//        -0.5, 0.0, -0.5, 0.0, 0.0,//bottom-left 0
//        0.5, 0.0, 0.5, 0.0, 1.0,//top-right 2
//        0.5, 0.0, -0.5, 1.0, 1.0,//top-left 3

        -0.5, 0.0, -0.5, 0.0, 0.0, //bottom-left 0
        -0.5, 0.0, 0.5, 1.0, 0.0,//bottom-right 1
        0.0, 0.6, 0.0, 0.5, 1.0,//peek 4

        -0.5, 0.0, 0.5, 0.0, 0.0,//bottom-right 1
        0.5, 0.0, 0.5, 1.0, 0.0,//top-right 2
        0.0, 0.6, 0.0, 0.5, 1.0,//peek 4

        0.5, 0.0, 0.5, 0.0, 0.0,//top-right 2
        0.5, 0.0, -0.5, 1.0, 0.0,//top-left 3
        0.0, 0.6, 0.0, 0.5, 1.0,//peek 4

        0.5, 0.0, -0.5, 0.0, 0.0,//top-left 3
        -0.5, 0.0, -0.5, 1.0, 0.0,//bottom-left 0
        0.0, 0.6, 0.0, 0.5, 1.0//peek 4

    };

    float ground[] = {
            5.0f, 0.0f,  5.0f,  2.0f, 0.0f,
            -5.0f, 0.0f,  5.0f,  0.0f, 0.0f,
            -5.0f, 0.0f, -5.0f,  0.0f, 2.0f,

            5.0f, 0.0f,  5.0f,  2.0f, 0.0f,
            -5.0f, 0.0f, -5.0f,  0.0f, 2.0f,
            5.0f, 0.0f, -5.0f,  2.0f, 2.0f

    };

    unsigned indices_pyramid[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    unsigned indices_ground[] = {
        0, 1, 2,
        0, 2, 3
    };

    //Vertex Buffer Object & Vertex Array Object
    unsigned VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //pyramid
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //sand
    glBindVertexArray(VAOs[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    //Create shaders
    Shader shader_pyramid = Shader(FileSystem::getPath("resources/shaders/pyramid.vert"), FileSystem::getPath("/resources/shaders/pyramid.frag"));
    Shader ground_shader = Shader(FileSystem::getPath("resources/shaders/ground_shader.vert"),FileSystem::getPath("resources/shaders/ground_shader.frag"));

    //Pyramid texture
    Texture2D texture_pyramid = Texture2D(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    texture_pyramid.load(FileSystem::getPath("resources/textures/pyramid_2.jpg"), GL_RGB);
    texture_pyramid.reflect_vertically();
    texture_pyramid.free_data();

    //Sand texture
    Texture2D sand_texture = Texture2D(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    texture_pyramid.load(FileSystem::getPath("/resources/textures/sand.jpg"), GL_RGB);
    texture_pyramid.reflect_vertically();
    texture_pyramid.free_data();

    //Initial color of background
    glClearColor(0.2,0.4,0.7,1.0);

    //Enabling depth testing
    glEnable(GL_DEPTH_TEST);

    //Rendering loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){
        processInput(window);

        //frame-time logic
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        //Create model matrix
        glm::mat4 model = glm::mat4(1.0f);

        //Create view matrix
        glm::mat4 view = glm::mat4(1.0f);

        //Create view matrix
        view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

        //Create projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);

        //Set matrices for pyramid
        shader_pyramid.use();
        shader_pyramid.setMat4("model", model);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        shader_pyramid.setInt("texture_pyramid", 0);
        texture_pyramid.activate(GL_TEXTURE0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //set matrices for ground
        ground_shader.use();
        shader_pyramid.setMat4("model", model);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        //activate sand texture
        ground_shader.setInt("texture_sand", 0);
        sand_texture.activate(GL_TEXTURE0);

        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader_pyramid.deleteProgram();
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.0 * delta_time;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraFront * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraFront * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

//    const float cameraSpeed = 25.0 * delta_time;
//
//    if(key == GLFW_KEY_W && action == GLFW_PRESS){
//        cameraPos += cameraFront * cameraSpeed;
//    }
//
//    if(key == GLFW_KEY_S && action == GLFW_PRESS){
//        cameraPos -= cameraFront * cameraSpeed;
//    }
//
//    if(key == GLFW_KEY_D && action == GLFW_PRESS){
//        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    }
//
//    if(key == GLFW_KEY_A && action == GLFW_PRESS){
//        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    }

}
