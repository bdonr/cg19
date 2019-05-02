//
// Created by don on 5/1/19.
//

#include <scg3.h>
#include "ObjectController.h"
#include "Camera.h"

 ObjectControllerSP ObjectController::create(CameraSP camera, TransformationSP obj){
        return std::make_shared<ObjectController>(camera,obj);
}
void ObjectController::checkInput(ViewState* viewState) {
    static double lastTime(glfwGetTime());
    GLFWwindow* window = viewState->getWindow();

    double currTime = glfwGetTime();
    GLfloat diffTime = static_cast<GLfloat>(currTime - lastTime);
    lastTime = currTime;
    // camera movement
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {

            obj.translate(glm::vec3(.1,0,.0));
    }
}

ObjectController::~ObjectController(CameraSP camera,TransformationSP obj):CameraController(camera)  {
    this->obj=obj;
}


ObjectController::~ObjectController() {

}
