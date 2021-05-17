#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>
#include <stb_image.h>
#include <rg/Texture2D.h>
#include <rg/Shader.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// settings

const unsigned int SCR_WIDTH = 1980;
const unsigned int SCR_HEIGHT = 1024;

//TODO: implementirati sun light za kutiju

//Vectors of camera
glm::vec3 lightColor = glm::vec3(0.2f, 0.4f, 0.4f);
glm::vec3 lightPosition = glm::vec3(2.0 ,-0.1,  -7.0);

//sunlight
glm::vec3 sunLightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
glm::vec3 sunLightColor = glm::vec3(0.8f);

//camera
glm::vec3 cameraPos = glm::vec3(0.0, 1.0, 4.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

//light distance constants
float lightConst = 1.0f;
float linearConst = 0.08;
float quadraticConst = 0.032f;

//spotlight
glm::vec3 spotlightColor = glm::vec3(1.0f);
int spotLightFlag = 1;

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
    Shader kocka = Shader(FileSystem::getPath("resources/shaders/cube.vert"), FileSystem::getPath("resources/shaders/cube.frag"));
    Shader obelisk = Shader(FileSystem::getPath("resources/shaders/obelisk.vert"), FileSystem::getPath("resources/shaders/obelisk.frag"));
    kocka.use();
    kocka.setVec3("lightColor", lightColor);//boja kocke na vrhu obeliska

    obelisk.use();
    obelisk.setVec3("lightColor", lightColor);
    obelisk.setVec3("objectColor", glm::vec3(0.76, 0.698, 0.52));
    obelisk.setVec3("material.ambient", glm::vec3(0.2f));
    obelisk.setVec3("material.diffuse", glm::vec3(0.60f, 0.3f, 0.2f));
    obelisk.setVec3("lightPosition", lightPosition);

    Shader sanduk = Shader(FileSystem::getPath("resources/shaders/sanduk.vert"), FileSystem::getPath("resources/shaders/sanduk.frag"));
    sanduk.use();
//
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
    Shader ground_shader = Shader (FileSystem::getPath("resources/shaders/ground_shader.vert"),FileSystem::getPath("resources/shaders/ground_shader.frag"));

//    Pyramid texture
    Texture2D texture_pyramid = Texture2D(GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    texture_pyramid.load(FileSystem::getPath("resources/textures/pyramid_2.jpg"), GL_RGB);
    texture_pyramid.reflect_vertically();
    texture_pyramid.free_data();

//    Sand texture
    Texture2D sand_texture = Texture2D(GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
    sand_texture.load(FileSystem::getPath("/resources/textures/sand.jpg"), GL_RGB);
    sand_texture.reflect_vertically();
    sand_texture.free_data();

//    wood texture
    Texture2D wood_texture = Texture2D(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    wood_texture.load(FileSystem::getPath("resources/textures/container2.png"), GL_RGBA);
    wood_texture.reflect_vertically();
    wood_texture.free_data();

    // metal texture
    Texture2D metal_texture = Texture2D(GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    metal_texture.load(FileSystem::getPath("resources/textures/container2_specular.png"), GL_RGBA);
    metal_texture.reflect_vertically();
    metal_texture.free_data();

    //Initial color of background

    //Enabling depth testing
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    shader modelShader("resources/shaders/model_loading.vs",
                       "resources/shaders/model_loading.fs");

//    modelShader.use();

    Model ourModel(FileSystem::getPath("resources/objects/truck/13630_open3dmodel/open3dmodel.com/Model_C0901061/kraz.obj"));


    //Rendering loop
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(!glfwWindowShouldClose(window)){


        glClearColor(glm::cos(glfwGetTime()/10)/2 + 0.3,glm::cos(glfwGetTime()/10)/2 + 0.23,glm::cos(glfwGetTime()/10)/2+ 0.01,1.0);
        float radius = 8.0;
        glm::vec3 lightPosition = glm::vec3(cos(glfwGetTime()) * radius  ,2.5,  sin(glfwGetTime())*radius);
        sunLightColor = glm::vec3(glm::cos(glfwGetTime()/10.0) / 2.0 + 0.5);
        //sunLightDirection = glm::vec3(-max(glm::cos(glfwGetTime()/5.0), 0.0), -max(glm::sin(glfwGetTime()/5.0), 0.0), 0.0);

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
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH/SCR_HEIGHT, 0.1f, 100.0f);

//        //Set matrices for pyramid
        shader_pyramid.use();
        shader_pyramid.setMat4("model", model);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        //viewPos
        shader_pyramid.setVec3("viewPos", cameraPos);

        //spotLight specification
        shader_pyramid.setFloat("spotLight.lightConst", lightConst);
        shader_pyramid.setFloat("spotLight.linearConst", linearConst);
        shader_pyramid.setFloat("spotLight.quadraticConst", quadraticConst);
        shader_pyramid.setInt("spotLight.spotLightFlag", spotLightFlag);
        shader_pyramid.setVec3("spotLight.position", cameraPos);
        shader_pyramid.setVec3("spotLight.direction", cameraFront);
        shader_pyramid.setVec3("spotLight.color", glm::vec3 (1.0f));
        shader_pyramid.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
        shader_pyramid.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));

//        //sunLight specification
        shader_pyramid.setVec3("dirLight.direction", sunLightDirection);
        shader_pyramid.setVec3("dirLight.color", sunLightColor);
//
//        //bug1 specification
        shader_pyramid.setFloat("pointLight.lightConst", lightConst);
        shader_pyramid.setFloat("pointLight.linearConst", linearConst);
        shader_pyramid.setFloat("pointLight.quadraticConst", quadraticConst);
        shader_pyramid.setVec3("pointLight.position", lightPosition);
        shader_pyramid.setVec3("pointLight.color", lightColor);

//        //pyramid texture
        shader_pyramid.setInt("texture_pyramid", 0);
        texture_pyramid.activate(GL_TEXTURE0);
//
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(0);

        glm::mat4 model_velika = glm::mat4(1.0f);
        model_velika = glm::translate(model_velika, glm::vec3(5.0f, 0.0f, -5.0f));
        model_velika = glm::scale(model_velika, glm::vec3(4.0f, 4.0f, 4.0f));

        shader_pyramid.setMat4("model", model_velika);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        texture_pyramid.activate(GL_TEXTURE0);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        shader_pyramid.setMat4("model", model);
        shader_pyramid.setMat4("view", view);
        shader_pyramid.setMat4("projection", projection);

        //ground
        glm::mat4 ground_model = glm::mat4(1.0f);

        ground_shader.use();
        ground_shader.setMat4("model", ground_model);
        ground_shader.setMat4("view", view);
        ground_shader.setMat4("projection", projection);


        //viewPos
        ground_shader.setVec3("viewPos", cameraPos);

        //sun light (directional light)
        ground_shader.setVec3("dirLight.direction", sunLightDirection);
        ground_shader.setVec3("dirLight.color", sunLightColor);

        //bug light (point light)
        ground_shader.setFloat("pointLight.lightConst", lightConst);
        ground_shader.setFloat("pointLight.linearConst", linearConst);
        ground_shader.setFloat("pointLight.quadraticConst", quadraticConst);
        ground_shader.setVec3("pointLight.position", lightPosition);
        ground_shader.setVec3("pointLight.color", lightColor);

        //spotlight
        ground_shader.setFloat("spotLight.lightConst", lightConst);
        ground_shader.setFloat("spotLight.linearConst", linearConst);
        ground_shader.setFloat("spotLight.quadraticConst", quadraticConst);
        ground_shader.setInt("spotLight.spotLightFlag", spotLightFlag);
        ground_shader.setVec3("spotLight.position", cameraPos);
        ground_shader.setVec3("spotLight.direction", cameraFront);
        ground_shader.setVec3("spotLight.color", glm::vec3 (1.0f));
        ground_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
        ground_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));

        // texture activation
        ground_shader.setInt("texture_sand", 0);
        sand_texture.activate(GL_TEXTURE0);


        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glBindVertexArray(cubeVAO);
//        nova_kocka.use();
//
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
        sanduk.setInt("material.diffuse", 0);
        sanduk.setVec3("lightPosition", lightPosition);
        sanduk.setVec3("lightColor", lightColor);
        sanduk.setVec3("light.ambient",  0.6f, 0.6f, 0.6f);
        sanduk.setVec3("light.diffuse",  glm::vec3(1.0f));
        sanduk.setVec3("light.specular", 0.2, 0.2, 0.2);
        sanduk.setVec3("viewPos", cameraPos);
        //spotLight sanduk
        sanduk.setFloat("spotLight.lightConst", lightConst);
        sanduk.setFloat("spotLight.linearConst", linearConst);
        sanduk.setFloat("spotLight.quadraticConst", quadraticConst);
        sanduk.setInt("spotLight.spotLightFlag", spotLightFlag);
        sanduk.setVec3("spotLight.position", cameraPos);
        sanduk.setVec3("spotLight.direction", cameraFront);
        sanduk.setVec3("spotLight.color", glm::vec3 (1.0f));
        sanduk.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
        sanduk.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
        sanduk.setVec3("sunLightDirection", sunLightDirection);

        wood_texture.activate(GL_TEXTURE0);

        sanduk.setInt("material.specular", 1);
        metal_texture.activate(GL_TEXTURE1);

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

            //spotLight specification
            obelisk.setFloat("spotLight.lightConst", lightConst);
            obelisk.setFloat("spotLight.linearConst", linearConst);
            obelisk.setFloat("spotLight.quadraticConst", quadraticConst);
            obelisk.setInt("spotLight.spotLightFlag", spotLightFlag);
            obelisk.setVec3("spotLight.position", cameraPos);
            obelisk.setVec3("spotLight.direction", cameraFront);
            obelisk.setVec3("spotLight.color", glm::vec3 (1.0f));
            obelisk.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
            obelisk.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));

//
            obelisk.setMat4("model", model_obelisk);
            obelisk.setMat4("view", view);
            obelisk.setMat4("projection", projection);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
//
        modelShader.use();
        glm::mat4 model_model = glm::mat4(1.0f);
        model_model = glm::translate(model_model, glm::vec3(-2.0f, 0.3f , 0.0f));
        model_model = glm::rotate(model_model, (float)glm::radians(-90.0f), glm::vec3(1, 0, 0));
        model_model = glm::scale(model_model, glm::vec3(0.0001));
        modelShader.setMat4("model", model_model);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);
        modelShader.setVec3("lightPosition", lightPosition);
        modelShader.setVec3("sunLightColor", sunLightColor);
        modelShader.setVec3("sunLightDirection", sunLightDirection);

        //spotLight for model
        modelShader.setFloat("spotLight.lightConst", lightConst);
        modelShader.setFloat("spotLight.linearConst", linearConst);
        modelShader.setFloat("spotLight.quadraticConst", quadraticConst);
        modelShader.setInt("spotLight.spotLightFlag", spotLightFlag);
        modelShader.setVec3("spotLight.position", cameraPos);
        modelShader.setVec3("spotLight.direction", cameraFront);
        modelShader.setVec3("spotLight.color", glm::vec3 (1.0f));
        modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
        modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
        modelShader.setVec3("lightColor", lightColor);
        modelShader.setVec3("viewPos", cameraPos);

        modelShader.setVec3("dirLight.direction", sunLightDirection);
        modelShader.setVec3("dirLight.color", sunLightColor);
        ourModel.Draw(modelShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


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
        if(spotLightFlag == 1){
            spotLightFlag = 0;
        }
        else{
            spotLightFlag = 1;
        }
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