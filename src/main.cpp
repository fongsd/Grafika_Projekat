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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings

const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1024;

//Vectors of camera

const glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPosition = glm::vec3(2.0 ,2.5,  3.0);

glm::vec3 cameraPos = glm::vec3(0.0, 1.0, 4.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
//timing
float delta_time = 0.0f;

float last_frame = 0.0f;
float lastX = SCR_WIDTH / 2.0;

float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
// -------------------------------------------------------


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
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

        -0.5, 0.0, -0.5, 0.0, 0.0,  -1.25f, 1.25f, 0.0f,//bottom-left 0
        -0.5, 0.0, 0.5, 1.0, 0.0, -1.25f, 1.25f, 0.0f,//bottom-right 1
        0.0, 0.5, 0.0, 0.5, 1.0, -1.25f, 1.25f, 0.0f,//peek 4

        -0.5, 0.0, 0.5, 0.0, 0.0,   0.0f, 1.25f, 1.25f,//bottom-right 1
        0.5, 0.0, 0.5, 1.0, 0.0,   0.0f, 1.25f, 1.25f,//top-right 2
        0.0, 0.5, 0.0, 0.5, 1.0,  0.0f, 1.25f, 1.25f,//peek 4

        0.5, 0.0, 0.5, 0.0, 0.0,  1.25f, 1.25f, 0.0f,//top-right 2
        0.5, 0.0, -0.5, 1.0, 0.0, 1.25f, 1.25f, 0.0f,//top-left 3
        0.0, 0.5, 0.0, 0.5, 1.0,  1.25f, 1.25f, 0.0f,//peek 4

        0.5, 0.0, -0.5, 0.0, 0.0,  0.0f, 1.25f, -1.25f,//top-left 3
        -0.5, 0.0, -0.5, 1.0, 0.0, 0.0f, 1.25f, -1.25f,//bottom-left 0
        0.0, 0.5, 0.0, 0.5, 1.0, 0.0f, 1.25f, -1.25f//peek 4

    };

    float ground[] = {
            150.0f, 0.0f,  150.0f,  150.0f, 0.0f,
            -150.0f, 0.0f,  150.0f,  0.0f, 0.0f,
            -150.0f, 0.0f, -150.0f,  0.0f, 150.0f,

            150.0f, 0.0f,  150.0f,  150.0f, 0.0f,
            -150.0f, 0.0f, -150.0f,  0.0f, 150.0f,
            150.0f, 0.0f, -150.0f,  150.0f, 150.0f

    };

    //pocetak kocke

    float cube [] =
            {
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
                    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
                    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
                    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
            };

    unsigned int cubeVBO, cubeVAO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float ), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    //dodavano odavde
    Shader kocka = Shader("resources/shaders/cube.vert", "resources/shaders/cube.frag");
    Shader obelisk = Shader("resources/shaders/obelisk.vert", "resources/shaders/obelisk.frag");
    kocka.use();
    kocka.setVec3("lightColor", lightColor);//boja kocke na vrhu obeliska

    obelisk.use();
    obelisk.setVec3("lightColor", lightColor);
    obelisk.setVec3("objectColor", glm::vec3(0.76, 0.698, 0.52));
    obelisk.setVec3("material.ambient", glm::vec3(0.2f));
    obelisk.setVec3("material.diffuse", glm::vec3(0.60f, 0.3f, 0.2f));
//    obelisk.setVec3("lightPosition", lightPosition);

    Shader sanduk = Shader("resources/shaders/sanduk.vert", "resources/shaders/sanduk.frag");
    sanduk.use();

//    Shader nova_kocka = Shader("resources/shaders/kocka.vert", "resources/shaders/kocka.frag");
//    nova_kocka.use();

    //kraj kocke

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void * )(5* sizeof(float)));
    glEnableVertexAttribArray(2);
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
    Texture2D texture_pyramid = Texture2D(GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    texture_pyramid.load(FileSystem::getPath("resources/textures/pyramid_2.jpg"), GL_RGB);
    texture_pyramid.reflect_vertically();
    texture_pyramid.free_data();

    //Sand texture
    Texture2D sand_texture = Texture2D(GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    sand_texture.load(FileSystem::getPath("/resources/textures/sand.jpg"), GL_RGB);
    sand_texture.reflect_vertically();
    sand_texture.free_data();

    //wood texture
    Texture2D wood_texture = Texture2D(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    wood_texture.load(FileSystem::getPath("resources/textures/container2.png"), GL_RGBA);
    wood_texture.reflect_vertically();
    wood_texture.free_data();

    //Initial color of background
    glClearColor(0.12,0.12,0.2,1.0);

    //Enabling depth testing
    glEnable(GL_DEPTH_TEST);

    //Rendering loop
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){
        float radius = 10.0;
        glm::vec3 lightPosition = glm::vec3(cos(glfwGetTime()) * radius  ,2.5,  sin(glfwGetTime())*radius);
//        glm::vec3 lightPosition = glm::vec3(2.0 ,2.5,  3.0);

        processInput(window);

        //frame-time logic
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        //Create model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        //Create view matrix
        glm::mat4 view = glm::mat4(1.0f);

        //Create view matrix
        view = glm::lookAt(cameraPos , cameraFront + cameraPos, cameraUp);

        //Create projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 3000.0f);

        //Set matrices for pyramid
        shader_pyramid.use();
        shader_pyramid.setMat4("model", model);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        shader_pyramid.setVec3("lightPosition", lightPosition);        //pozicija svetla sa obeliska
        shader_pyramid.setVec3("lightColor", lightColor); //jacina svetla sa obeliska
        shader_pyramid.setVec3("objectColor", glm::vec3(0.2f, 0.3f, 0.4f)); //boja piramide
        shader_pyramid.setInt("texture_pyramid", 0);
        texture_pyramid.activate(GL_TEXTURE0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        //odavde
        glBindVertexArray(0);

        glm::mat4 model_velika = glm::mat4(1.0f);
        model_velika = glm::translate(model_velika, glm::vec3(5.0f, 0.0f, -5.0f));
        model_velika = glm::scale(model_velika, glm::vec3(4.0f, 4.0f, 4.0f));
        shader_pyramid.use();
        shader_pyramid.setMat4("model", model_velika);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        shader_pyramid.setVec3("lightPosition", lightPosition);        //pozicija svetla sa obeliska
        shader_pyramid.setVec3("lightColor", lightColor); //jacina svetla sa obeliska
        shader_pyramid.setVec3("objectColor", glm::vec3(0.2f, 0.3f, 0.4f)); //boja piramide
        shader_pyramid.setVec3("viewPos", cameraPos);
        shader_pyramid.setVec3("light.ambient",  0.6f, 0.6f, 0.6f);
        shader_pyramid.setVec3("light.diffuse",  0.5f, 0.5f, 0.5f);

        shader_pyramid.setInt("texture_pyramid", 0);
        texture_pyramid.activate(GL_TEXTURE0);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        //set matrices for ground
        ground_shader.use();
        shader_pyramid.setMat4("model", model);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);


        glm::mat4 ground_model = glm::mat4(1.0f);
//        ground_model = glm::scale(ground_model, glm::vec3(10.0f, 1.0f, 10.0f));

        ground_shader.setMat4("model", ground_model);
        ground_shader.setMat4("view", view);
        ground_shader.setMat4("projection", projection);
        //activate sand texture
        ground_shader.setVec3("lightPosition", lightPosition);
        ground_shader.setVec3("lightColor", lightColor);
        ground_shader.setInt("texture_sand", 0);
        sand_texture.activate(GL_TEXTURE0);

        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glBindVertexArray(cubeVAO);
//        nova_kocka.use();

        glm::mat4 model_cube = glm::mat4(1.0f);
        model_cube = glm::translate(model_cube, lightPosition);
        float cube_scale = 0.08f;

        kocka.use();

        model_cube = glm::scale(model_cube, glm::vec3(cube_scale));
        kocka.setMat4("model", model_cube);
        kocka.setMat4("view", view);
        kocka.setMat4("projection", projection);


        glDrawArrays(GL_TRIANGLES, 0, 36);

         cube_scale = 0.2;

        sanduk.use();

        model_cube = glm::mat4(1.0f);
        model_cube = glm::translate(model_cube, glm::vec3(1.3, 0.12, -2.3));
        model_cube = glm::scale(model_cube, glm::vec3(cube_scale));
        sanduk.setMat4("model", model_cube);
        sanduk.setMat4("view", view);
        sanduk.setMat4("projection", projection);
        sanduk.setInt("wood_texture", 0);
        wood_texture.activate(GL_TEXTURE0);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        for (int i = 0; i < 5; i++) {
            obelisk.setVec3("viewPos", lightPosition);

            obelisk.use();

            glm::mat4 model_obelisk = glm::mat4(1.0f);
            float angle = 0.50f;
            model_obelisk = glm::translate(model_obelisk, glm::vec3(0.0f, angle * i, 0.0f));
            model_obelisk = glm::translate(model_obelisk, glm::vec3(-3.0f, 0.2f, -5.0f));
            model_obelisk = glm::scale(model_obelisk, glm::vec3(0.3f, 0.50f,  0.3f));


            obelisk.setMat4("model", model_obelisk);
            obelisk.setMat4("view", view);
            obelisk.setMat4("projection", projection);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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



    const float cameraSpeed = 10.0 * delta_time;

    //da ne ide kamera ispod y ose

    if (cameraPos.y  < 0.3f)
    {
        cameraPos.y = 0.3f;
    }

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
    if(key == GLFW_KEY_F && action == GLFW_PRESS){
        //TODO: Uraditi fullscreen dugme
    }

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}