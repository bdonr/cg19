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
VideoKeyboardControllerSP videocontroller ;
FloorKeyboardControllerSP floorcontroller ;
TransformAnimationSP transAni;
ViewerSP viewer;

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
    viewer= Viewer::create();
    renderer = StandardRenderer::create();
    viewer->init(renderer)
            ->createWindow("Cg19 Projekt", 1024, 768);


    // create camera
    videoCam = PerspectiveCamera::create();
    videoCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);

    // Create Scenes
    videoScene = Group::create();
    standartScene = Group::create();

    flyCam = PerspectiveCamera::create();
    flyCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);

    videocontroller = VideoKeyboardController::create(videoCam);
    floorcontroller = FloorKeyboardController::create(flyCam);

    //set Camera and first scene
    renderer->setCamera(flyCam);


    //set scene in controller
    floorcontroller->setVideoScene(videoScene);
    floorcontroller->setGameScene(standartScene);
    floorcontroller->setRenderer(renderer);


    createStandartScene(viewer, flyCam);
    createVideoScene(viewer, flyCam);

    renderer->setScene(standartScene);

    viewer->addControllers(
            {
                    floorcontroller,
                    videocontroller,
                    MouseController::create(flyCam)
            });


    viewer->startAnimations()
            ->startMainLoop();

}

/*
void showVideoScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {
    renderer->setScene(videoScene);
}


void showStandartScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {
    renderer->setScene(standartScene);
}
*/
void checkChooseScene(ViewerSP viewer) {


}


void createStandartScene(ViewerSP viewer, CameraSP camera) {

   EnvoirementController::createSunFloorscene(viewer, camera, standartScene);

}

void createVideoScene(ViewerSP viewer, CameraSP camera) {


    videoCam->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);
    LightFactory *lightFactory = LightFactory::getInstance();
    videoScene->addChild(lightFactory->getVideoSonne());

    lightFactory->getVideoSonne()->addChild(insta->createFlugzeugGruppe());
    lightFactory->getVideoSonne()->addChild(insta->getFloor());

    //hier nochmal gucken schwarzes bild nach setten der scene
  // EnvoirementController::createVideoSceneHelper(viewer, camera, videoScene);


}
