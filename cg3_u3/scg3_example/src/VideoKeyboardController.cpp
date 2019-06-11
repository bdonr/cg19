//
// Created by don on 09.06.19.
//

#include <iostream>
#include "VideoKeyboardController.h"

namespace scg {
    void VideoKeyboardController::checkInput(ViewState *viewState) {

        static double lastTime(glfwGetTime());
        GLFWwindow *window = viewState->getWindow();

        // determine time difference
        double currTime = glfwGetTime();
        GLfloat diffTime = static_cast<GLfloat>(currTime - lastTime);
        lastTime = currTime;

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {

            //camera_->translate(glm::vec3(0.0f, 0.0f, -0.008f));
        }
    }

    VideoKeyboardControllerSP VideoKeyboardController::create(CameraSP &camera) {
        return std::make_shared<VideoKeyboardController>(camera);
    }

    VideoKeyboardController::VideoKeyboardController(const CameraSP &camera) : CameraController(camera) {
        std::cout << "Video camera control enabled" << std::endl;
        std::cout << "press f to return to original scene" << std::endl;
    }

    VideoKeyboardController::~VideoKeyboardController() {

    }

    bool VideoKeyboardController::isEnable() const {
        return enable;
    }

    void VideoKeyboardController::setEnable(bool enable) {
        this->enable = true;
    }


}