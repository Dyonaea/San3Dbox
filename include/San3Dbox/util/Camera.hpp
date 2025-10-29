#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "Shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class Camera{
    
    private:
        bool firstClick = true;

    public:
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        float aspect;
        int width;
        int height;
        float speed = 0.1f;
        float sensitivity = 30.0f;

        Camera(int width, int height){
            Camera::width = width;
            Camera::height = height;
            aspect = (float)width/(float)height;
        }

        void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader *shader, const char *uniform){
            glm::mat4 view = glm::mat4(1.0f); 
            glm::mat4 projection = glm::mat4(1.0f); 

            view = glm::lookAt(position, position + orientation, up);
            projection = glm::perspective(glm::radians(FOVdeg), aspect, nearPlane, farPlane);
            glUniformMatrix4fv(glGetUniformLocation(shader->ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
        }
        void inputs(GLFWwindow *window){
    
            if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                position += speed * -glm::normalize(glm::cross(orientation, up));
            }
            if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                position += speed * orientation;
            }
            if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                position += speed * -orientation;
            }
            if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                position += speed * glm::normalize(glm::cross(orientation, up));
            }
            if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
                position += speed * up;
            }
            if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
               position += speed * -up;
            }
            if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
                speed = 0.04f;
            }
            else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE){
                speed = 0.01f;
            }



            
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            
            if (firstClick)
            {
                glfwSetCursorPos(window, (width / 2), (height / 2));
                firstClick = false;
            }
            
            double mouseX;
            double mouseY;
            
            glfwGetCursorPos(window, &mouseX, &mouseY);
            
            float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
            float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

            glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

            if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
            {
                orientation = newOrientation;
            }

            orientation = glm::rotate(orientation, glm::radians(-rotY), up);

            glfwSetCursorPos(window, (width / 2), (height / 2));
        }
};