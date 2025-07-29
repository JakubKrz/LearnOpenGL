#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 2.0f, 0.0f);
float range;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    auto projPath = std::filesystem::current_path();
    std::filesystem::path vertexShaderPath = projPath / "VertexShader.vert";
    std::filesystem::path fragmentShaderPath = projPath / "FragmentShader.frag";
    std::filesystem::path lightFragmentShaderPath = projPath / "lightSource.frag";
    std::filesystem::path lightVertexShaderPath = projPath / "lightSource.vert";
    Shader LightingShader(vertexShaderPath.string().c_str(), fragmentShaderPath.string().c_str());
    Shader lightSourceShader(lightVertexShaderPath.string().c_str(), lightFragmentShaderPath.string().c_str());

    float vertices[] = {
        // positions          // normals           // texture coords
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

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightSourceVAO;
    glGenVertexArrays(1, &lightSourceVAO);
    glBindVertexArray(lightSourceVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::filesystem::path containerPath = projPath / "Resources/container2.png";
    std::filesystem::path containerSpecularPath = projPath / "Resources/container2_specular.png";
    std::filesystem::path matrixPath = projPath / "Resources/matrix.jpg";
    unsigned int diffuseMap = loadTexture(containerPath.string().c_str());
    unsigned int specularMap = loadTexture(containerSpecularPath.string().c_str());
    unsigned int emissionMap = loadTexture(matrixPath.string().c_str());

    LightingShader.use();
    LightingShader.setInt("material.diffuse", 0);
    LightingShader.setInt("material.specular", 1);
    LightingShader.setInt("material.emission", 2);

    glm::vec3 ligthDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    
    range = 0.5;    

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        LightingShader.use();
        
        //LightingShader.setFloat("pointLights[0].constant", 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians((float)glfwGetTime()) * 5.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        LightingShader.setMat4("model", model);
        LightingShader.setMat4("view", view);
        LightingShader.setMat4("projection", projection);


        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime * 5.0f) * 3.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        lightPos = glm::vec3(rotation * glm::vec4(lightPos, 1.0f));       
        glm::vec3 lightPosView = glm::vec3(view * glm::vec4(lightPos, 1.0f));
        //LightingShader.setVec3("light.position", lightPosView);
        // 
        ligthDirection.x = (sin(glfwGetTime() * 0.5f));
        ligthDirection.y = (cos(glfwGetTime() * 0.5f));

        glm::vec3 lightColor;
        lightColor.x = (sin(glfwGetTime() * 2.0f) + 1.0f) * 0.5f;
        lightColor.y = (sin(glfwGetTime() * 0.7f) + 1.0f) * 0.5f;
        lightColor.z = (sin(glfwGetTime() * 1.3f) + 1.0f) * 0.5f;
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        //Directional light
        LightingShader.setVec3("dirLight.direction", glm::mat3(view) * ligthDirection);
        LightingShader.setVec3("dirLight.diffuse", glm::vec3(0.0f,0.0f,0.5f));
        LightingShader.setVec3("dirLight.specular", glm::vec3(0.9f, 0.8f, 1.0f));
        LightingShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        //Point lights
        float constant = 1.0f;
        float linear = 0.07f;
        float quadratic = 0.017f;

        LightingShader.setVec3("spotLight.position", glm::vec3(view * glm::vec4(camera.Position, 1.0f)));
        LightingShader.setVec3("spotLight.direction", glm::mat3(view) * camera.Front);
        LightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        LightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        LightingShader.setVec3("spotLight.ambient", 0.05f, 0.05f, 0.05f);
        LightingShader.setVec3("spotLight.diffuse", glm::vec3(0.6f, 0.6f, 0.3f));
        LightingShader.setVec3("spotLight.specular", glm::vec3(0.7f, 0.7f, 0.0f));
        LightingShader.setFloat("spotLight.constant", constant);
        LightingShader.setFloat("spotLight.linear", linear);
        LightingShader.setFloat("spotLight.quadratic", quadratic);

        glm::vec3 light1Pos = glm::vec3(pointLightPositions[0].x, pointLightPositions[0].y + ligthDirection.y, pointLightPositions[0].z + ligthDirection.x * 3 - 3);
        LightingShader.setVec3("pointLights[0].position", glm::vec3(view* glm::vec4(light1Pos, 1.0f)));
        LightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        LightingShader.setVec3("pointLights[0].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
        LightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 0.0f, 0.0f));
        LightingShader.setFloat("pointLights[0].constant", constant);
        LightingShader.setFloat("pointLights[0].linear", linear);
        LightingShader.setFloat("pointLights[0].quadratic", quadratic);

        LightingShader.setVec3("pointLights[1].position", glm::vec3(view* glm::vec4(pointLightPositions[1], 1.0f)));
        LightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        LightingShader.setVec3("pointLights[1].diffuse", diffuseColor);
        LightingShader.setVec3("pointLights[1].specular", lightColor);
        LightingShader.setFloat("pointLights[1].constant", constant);
        LightingShader.setFloat("pointLights[1].linear", linear);
        LightingShader.setFloat("pointLights[1].quadratic", quadratic);

        LightingShader.setVec3("pointLights[2].position", glm::vec3(view * glm::vec4(pointLightPositions[2], 1.0f)));
        LightingShader.setVec3("pointLights12].ambient", 0.05f, 0.05f, 0.05f);
        LightingShader.setVec3("pointLights[2].diffuse", diffuseColor);
        LightingShader.setVec3("pointLights[2].specular", lightColor);
        LightingShader.setFloat("pointLights[2].constant", constant);
        LightingShader.setFloat("pointLights[2].linear", linear);
        LightingShader.setFloat("pointLights[2].quadratic", quadratic);

        LightingShader.setVec3("pointLights[3].position", glm::vec3(view * glm::vec4(pointLightPositions[3], 1.0f)));
        LightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        LightingShader.setVec3("pointLights[3].diffuse", diffuseColor);
        LightingShader.setVec3("pointLights[3].specular", lightColor);
        LightingShader.setFloat("pointLights[3].constant", constant);
        LightingShader.setFloat("pointLights[3].linear", linear);
        LightingShader.setFloat("pointLights[3].quadratic", quadratic);

        LightingShader.setVec3("light.position", glm::vec3(view * glm::vec4(camera.Position,1.0f)));
        
        LightingShader.setVec3("light.direction", glm::mat3(view) * camera.Front);
        LightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        LightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

        glm::vec3 ligthDir = glm::normalize(glm::mat3(view) * ligthDirection);
        //LightingShader.setVec3("light.direction", ligthDir);

        LightingShader.setVec3("material.specular", 0.7f, 0.7f, 0.7f);
        LightingShader.setFloat("material.shininess", 32.0f);
        
        

        //glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        //glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
       /* LightingShader.setVec3("light.diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
        LightingShader.setVec3("light.specular", glm::vec3(1.0f, 0.0f, 0.0f));
        LightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);*/
        
  /*      LightingShader.setFloat("light.constant", 1.0f);
        LightingShader.setFloat("light.linear", 0.07f);
        LightingShader.setFloat("light.quadratic", 0.017f);*/

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            LightingShader.setMat4("model", model);
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
            LightingShader.setMat4("normalMatrix", normalMatrix);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(lightSourceVAO);
        model = glm::mat4(1.0f);
        lightSourceShader.use();
        lightSourceShader.setMat4("model", model);
        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setVec3("color", glm::vec3(1.0f,0.0f,0.0f));

        model = glm::mat4(1.0f);
        model = glm::translate(model, light1Pos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSourceShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        lightSourceShader.setVec3("color", lightColor);
        for (unsigned int i = 1; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightSourceShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightSourceVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}