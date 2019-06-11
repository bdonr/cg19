#include <utility>

/**
 * \file main.cpp
 * \brief A simple scg3 example application.
 *
 * Requires C++11 and OpenGL 3.2 (or later versions).
 *
 * \author Volker Ahlers\n
 *         volker.ahlers@hs-hannover.de
 */

/*
 * Copyright 2014-2019 Volker Ahlers
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <stdexcept>
#include <vector>

#include <sstream>
#include "Transformation.h"

#include "src/EnvoirementController.h"
#include <thread>

//#include <scg3.h>
#include "../scg3/scg3.h"
#include "src/EnvoirementController.h"
#include "src/MatFactory.h"
#include "src/TexturFactory.h"
#include "src/ShaderFactory.h"
#include "src/SceneObjetFactory.h"
#include "src/GameLogic.h"
#include "src/LightFactory.h"

using namespace scg;


/**
 * \brief Configuration parameters.
 */
struct SCGConfiguration {

    static const int viewerType = 1;  // 0: simple, 1: customized
    // for customized viewer:
    static const int sceneType = 1;   // 0: teapot, 1: table
};


/**
 * \brief Minimal application using a simple viewer with default renderer, shaders,
 *   camera, and light to create a teapot scene.
 */
void useSimpleViewer();

/**
 * \brief Typical application using a customized viewer to create a teapot or table scene.
 */
void useCustomizedViewer();

void checkChooseScene(ViewerSP viewer);

/**
 * \brief Create a scene consisting of a teapot, a camera, and a light.
 */
void showVideoScene(ViewerSP viewer, CameraSP camera, GroupSP &scene);


/**
 * \brief Create a scene consisting of a floor, a table, a teapot, a camera, and a light.
 */
void showStandartScene(ViewerSP viewer, CameraSP camera, GroupSP &scene);

void createVideoScene(ViewerSP viewer, CameraSP camera);

void createStandartScene(ViewerSP viewer, CameraSP camera);

/**
 * \brief The main function.
 */
double bulletTravel = 0;
int actualscene = 1;
GroupSP standartScene;
GroupSP videoScene;
StandardRendererSP renderer;
CameraSP videoCam;
CameraSP flyCam;
KeyboardControllerSP controller;
int main() {

    int result = 0;

    try {
        if (SCGConfiguration::viewerType == 0) {
            useSimpleViewer();
        } else {
            useCustomizedViewer();
        }
    }
    catch (const std::exception &exc) {
        std::cerr << std::endl << "Exception: " << exc.what() << std::endl;
        result = 1;
    }
    return result;
}


// Typical application using a customized viewer.
void useCustomizedViewer() {

    // create viewer and renderer
    ViewerSP viewer = Viewer::create();
    renderer = StandardRenderer::create();
    viewer->init(renderer)
            ->createWindow("Cg19 Projekt", 1024, 768);


    // create camera
     videoCam = PerspectiveCamera::create();
    videoCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);


    flyCam = PerspectiveCamera::create();
    flyCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);

    renderer->setCamera(flyCam);


    checkChooseScene(viewer);


    viewer->addControllers(
            {

                    MouseController::create(flyCam)
            });
    controller = KeyboardController::create(flyCam);
    viewer->addController(controller);

    viewer->startAnimations()
            ->startMainLoop();

}

/*
void showVideoScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {
    renderer->setScene(videoScene);
    renderer->setCamera(videoCam);
}


void showStandartScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {
    renderer->setScene(standartScene);
    renderer->setCamera(flyCam);
}
 */

void checkChooseScene(ViewerSP viewer) {

        if(actualscene==1){
            createStandartScene( viewer,  flyCam);
         }else if(actualscene==0){

            createVideoScene( viewer,  flyCam);
        }


}


void createStandartScene(ViewerSP viewer, CameraSP camera) {
    standartScene = Group::create();
    TransformAnimationSP transAni = TransformAnimation::create();
    transAni->setUpdateFunc(
            [move(camera), viewer](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {

                    if(controller->chooseScene==1){
                        std::cout<<"hier kurz mal der test wie oft ich in der if lande"<<std::endl;
                        controller->isMovement=false;

                        actualscene=0;
                        checkChooseScene(viewer);
                        actualscene=3;
                        controller->chooseScene=2;

                        viewer->startAnimations();
                    }

            }
    );

    SceneObjetFactory * instance = SceneObjetFactory::getInstance(viewer);
    LightFactory * lightFactory= LightFactory::getInstance();
    standartScene->addChild(instance->getHimmel());
    standartScene->addChild(lightFactory->getSonne());
    lightFactory->getSonne()->addChild(flyCam);
    flyCam->addChild(instance->getCamObject());



    standartScene->addChild(transAni);
    viewer->addAnimation(transAni);
    renderer->setScene(standartScene);

}

void createVideoScene(ViewerSP viewer, CameraSP cam) {
    std::cout<<"videoscene"<<std::endl;
    videoScene = Group::create();

   // EnvoirementController::createSunFloorscene();



    EnvoirementController::createVideoSceneHelper(viewer, cam, videoScene);




    renderer->setScene(videoScene);
}
