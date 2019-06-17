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

/**
 * \brief Create a scene consisting of a floor, a table, a teapot, a camera, and a light.
 */


void createVideoScene(ViewerSP viewer, CameraSP videoCam, GroupSP flightShowScene);

void createGameScene(ViewerSP viewer, CameraSP flyCam, GroupSP gameScene);

/**
 * \brief The main function.
 */
double bulletTravel = 0;
int actualscene = 1;

VideoKeyboardControllerSP videocontroller;
FloorKeyboardControllerSP floorcontroller;
bool drehungFlugzeug1;
GroupSP flightShowScene;
GroupSP gameScene;
EnvoirementController* envoirementControllerinstance;

int main() {

    int result = 0;
    useCustomizedViewer();


    return result;
}


// Typical application using a customized viewer.
void useCustomizedViewer() {

    // create viewer and renderer
    auto viewer = Viewer::create();
    auto renderer = StandardRenderer::create();
    viewer->init(renderer)
            ->createWindow("Cg19 Projekt", 1024, 768);

    envoirementControllerinstance=EnvoirementController::getControllerInstance(viewer);
    // create camera
    CameraSP flightShowCam = PerspectiveCamera::create();
    flightShowCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);


    CameraSP gameCam = PerspectiveCamera::create();
    gameCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);



    // Create Scenes
     flightShowScene = Group::create();
     gameScene = Group::create();




    //create our controller
    videocontroller = VideoKeyboardController::create(flightShowCam);
    floorcontroller = FloorKeyboardController::create(gameCam);

    floorcontroller->setBullet(SceneObjetFactory::getInstance(viewer)->createBullet());
    //set Camera and first scene

    renderer->setCamera(gameCam);



    //give scenes and renderer for controllers
    floorcontroller->setVideoScene(flightShowScene);
    //videocontroller->setGameScene(gameScene);
    floorcontroller->setGameScene(gameScene);
    floorcontroller->setRenderer(renderer);
    //videocontroller->setRenderer(renderer);
    floorcontroller->setGameCam(gameCam);
    floorcontroller->setFlightShowCam(flightShowCam);
    floorcontroller->setViewer(viewer);




    //create scenes once with EnviromentController
    createGameScene(viewer, gameCam,gameScene );
    createVideoScene(viewer, flightShowCam,flightShowScene );

    renderer->setScene(gameScene);

    viewer->addControllers(
            {       videocontroller,
                    floorcontroller,
                    MouseController::create(flightShowCam)
            });


    viewer->startAnimations()
            ->startMainLoop();

}

void createGameScene(ViewerSP viewer, CameraSP gameCam, GroupSP gameScene) {

    envoirementControllerinstance->createStandartScene(viewer, gameCam, gameScene);

}

void createVideoScene(ViewerSP viewer, CameraSP flightShowCam, GroupSP flightShowScene) {


    envoirementControllerinstance->createShowScene(viewer, flightShowCam, flightShowScene);

}
