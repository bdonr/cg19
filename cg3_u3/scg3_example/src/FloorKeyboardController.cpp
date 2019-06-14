//
// Created by don on 11.06.19.
//


#include "FloorKeyboardController.h"

#include <iostream>
#include "scg_glew.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "KeyboardController.h"
#include "scg_glm.h"
#include "ViewState.h"

namespace  scg {

    void FloorKeyboardController::checkInput(scg::ViewState *viewState) {
        static double lastTime(glfwGetTime());
        GLFWwindow *window = viewState->getWindow();
        // determine time difference
        double currTime = glfwGetTime();
        GLfloat diffTime = static_cast<GLfloat>(currTime - lastTime);
        lastTime = currTime;


        if(movement){
        // camera movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

            camera_->translate(glm::vec3(0.0f, 0.0f, -0.008f));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera_->translate(glm::vec3(0.0f, 0.0f, 0.003f));
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera_->rotateYaw(rotateVelocity_ / 4 * diffTime);
            //camera_->translate(glm::vec3(-moveVelocity_ * diffTime, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera_->rotateYaw(-rotateVelocity_ / 4 * diffTime);
            //camera_->translate(glm::vec3(moveVelocity_ * diffTime, 0.0f, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            //camera_->translate(glm::vec3(0.0f, moveVelocity_ * diffTime, 0.0f));
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            //camera_->translate(glm::vec3(0.0f, -moveVelocity_ * diffTime, 0.0f));
        }

        // camera rotation
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            //if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
            //   glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
            camera_->rotateRoll(-rotateVelocity_ * diffTime);
            //}
            /*else {
              if (isFlyMode_) {
                camera_->rotateYaw(rotateVelocity_ * diffTime);
              }
              else {
                camera_->rotateAzimuth(-rotateVelocity_ * diffTime);
              }
            }*/
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            //if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS ||
            //  glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
            camera_->rotateRoll(rotateVelocity_ * diffTime);
            /*}
            else {
              if (isFlyMode_) {
                camera_->rotateYaw(-rotateVelocity_ * diffTime);
              }
              else {
                camera_->rotateAzimuth(rotateVelocity_ * diffTime);
              }
            }*/
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            if (isFlyMode_) {
                camera_->rotatePitch(-rotateVelocity_ * diffTime);
            } else {
                camera_->rotateElevation(rotateVelocity_ * diffTime);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if (isFlyMode_) {
                camera_->rotatePitch(rotateVelocity_ * diffTime);
            } else {
                camera_->rotateElevation(-rotateVelocity_ * diffTime);
            }
        }

        // continuous flight velocity
        //E verringer flightVelocity_ und flugzeug wird schneller
        static bool toggleSpeedUp = false;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !toggleSpeedUp && flightVelocity_ > -1.2) {
            flightVelocity_ -= flightVelocityStep_;
            toggleSpeedUp = true;
            std::cout << flightVelocity_ << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
            toggleSpeedUp = false;
        }
        //Q erhöht flightVelocity_ und macht flugzeug langsammer
        static bool toggleSpeedDown = false;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !toggleSpeedDown && flightVelocity_ < -0.61) {
            flightVelocity_ += flightVelocityStep_;
            toggleSpeedDown = true;
            std::cout << flightVelocity_ << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
            toggleSpeedDown = false;
        }

        if (bullet) {
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                bullet->setVisible(true);
            } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
                bullet->setVisible(false);
            }
        }

        // toggle fly/examine mode
        /* static bool toggleKeySpace = false;
         if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !toggleKeySpace) {
           static bool prevDrawCenterMode = false;
           isFlyMode_ = !isFlyMode_;
           if (isFlyMode_) {
             // fly mode: restore previous draw center point mode
             camera_->setDrawCenter(prevDrawCenterMode);
           }
           else {
             // examine mode: draw center point, saving current mode
             prevDrawCenterMode = camera_->isDrawCenter();
             camera_->setDrawCenter(true);
           }
           toggleKeySpace = true;
         }
         if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
           toggleKeySpace = false;
         }
       */
        // toggle frame rate output
        static bool toggleKeyH = false;
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !toggleKeyH) {
            viewState->setFrameRateOutput(!viewState->isFrameRateOutput());
            toggleKeyH = true;
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_RELEASE) {
            toggleKeyH = false;
        }

        // toggle draw center point mode
        static bool toggleKeyJ = false;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !toggleKeyJ) {
            camera_->setDrawCenter(!camera_->isDrawCenter());
            toggleKeyJ = true;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
            toggleKeyJ = false;
        }

        // toggle visibility of mouse cursor
        static bool toggleKeyK = false;
        static double mouseXOld(0), mouseYOld(0);
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !toggleKeyK) {
            viewState->setMouseCursorVisible(!viewState->isMouseCursorVisible());
            if (viewState->isMouseCursorVisible()) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPos(window, mouseXOld, mouseYOld);
            } else {
                glfwGetCursorPos(window, &mouseXOld, &mouseYOld);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            toggleKeyK = true;
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
            toggleKeyK = false;
        }
        // toggle line/fill mode
        static bool toggleKeyL = false;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !toggleKeyL) {
            static bool isLineMode = false;
            isLineMode = !isLineMode;
            isLineMode ?
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) :
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            toggleKeyL = true;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
            toggleKeyL = false;
        }

        // continuous flight action

          //  camera_->translate(glm::vec3(0.0f, 0.0f, flightVelocity_ * diffTime));



        //Hier wird die Scene gewechselt
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            std::cout << "Im FFFF" << std::endl;
            renderer->getCamera()->setVisible(true);
            renderer->setScene(flightShowCam);
            renderer->setCamera(flightShowCam);
            camera_->setPosition(glm::vec3(2.2,3.73,-4.10));
            if(!pitch){
                camera_->rotatePitch(-3.0);
                pitch=true;
            }

            //camera_->setVisible(false);

            movement=false;


        }
//
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            renderer->getCamera()->setVisible(true);
            renderer->setScene(gameScene);
            renderer->setCamera(gameCam);
            std::cout << "Im ZZZZ" << std::endl;



            FloorKeyboardController::movement=true;
            movement=true;


        }


    }

    FloorKeyboardController::FloorKeyboardController(CameraSP camera) : CameraController(camera),movement{true}  {
        std::cout << "Floor camera control enabled" << std::endl;

        moveVelocity_ = 0.3f;
        rotateVelocity_ = 50.0f;
        flightVelocityStep_ = 0.2f;
    }

    void FloorKeyboardController::setBullet(NodeSP N) {
        this->bullet = N;
    }

    FloorKeyboardController::~FloorKeyboardController() {
    }


    FloorKeyboardControllerSP FloorKeyboardController::create(CameraSP &camera) {
        return std::make_shared<FloorKeyboardController>(camera);
    }

    void FloorKeyboardController::setRenderer(StandardRendererSP &renderer) {
        this->renderer=renderer;
    }
    void FloorKeyboardController::setVideoScene(GroupSP videoScene) {
        this->flightShowScene=videoScene;

    }

    void FloorKeyboardController::setGameScene(GroupSP gameScene) {
        this->gameScene=gameScene;

    }

    void FloorKeyboardController::setGameCam(CameraSP gameCam){
         this->gameCam=gameCam;
    }
    void FloorKeyboardController::setFlightShowCam(CameraSP flightShowCam){
         this->flightShowCam=flightShowCam;
    }

}