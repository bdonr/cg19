//
// Created by don on 09.06.19.
//

#include <iostream>
#include "VideoKeyboardController.h"

namespace scg {
    void VideoKeyboardController::checkInput(ViewState *viewState) {
        if(chooseScene) {
            static double lastTime(glfwGetTime());
            GLFWwindow *window = viewState->getWindow();

            // determine time difference
            double currTime = glfwGetTime();
            GLfloat diffTime = static_cast<GLfloat>(currTime - lastTime);
            lastTime = currTime;

            if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {

                chooseScene = true;
            }
        }
    }

    VideoKeyboardControllerSP VideoKeyboardController::create(CameraSP &camera) {
        return std::make_shared<VideoKeyboardController>(camera);
    }

    VideoKeyboardController::VideoKeyboardController(const CameraSP &camera) : CameraController(camera),chooseScene{false} {
        std::cout << "Video camera control enabled" << std::endl;
        std::cout << "press f to return to original scene" << std::endl;
    }

    VideoKeyboardController::~VideoKeyboardController() {

    }

    bool VideoKeyboardController::isEnable() const {
        return enable;
    }

    void VideoKeyboardController::setEnable(bool enable) {
        this->enable = enable;
    }

    bool VideoKeyboardController::isChooseScene() const {
        return chooseScene;
    }

    void VideoKeyboardController::setChooseScene(bool chooseScene) {
        VideoKeyboardController::chooseScene = chooseScene;
    }


}