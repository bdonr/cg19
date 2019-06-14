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


    // create camera
    CameraSP flightShowCam = PerspectiveCamera::create();
    flightShowCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);


    CameraSP gameCam = PerspectiveCamera::create();
    gameCam->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);



    // Create Scenes
    auto flightShowScene = Group::create();
    auto gameScene = Group::create();




    //create our controller
    videocontroller = VideoKeyboardController::create(flightShowCam);
    floorcontroller = FloorKeyboardController::create(gameCam);

    //set Camera and first scene

    renderer->setCamera(gameCam);



    //give scenes and renderer for controllers
    floorcontroller->setVideoScene(flightShowScene);
    videocontroller->setGameScene(gameScene);
    floorcontroller->setGameScene(gameScene);
    floorcontroller->setRenderer(renderer);
    videocontroller->setRenderer(renderer);
    floorcontroller->setGameCam(gameCam);
    floorcontroller->setFlightShowCam(flightShowCam);


    //create scenes once with Helper
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

    EnvoirementController::createSunFloorscene(viewer, gameCam, gameScene);

}

void createVideoScene(ViewerSP viewer, CameraSP flightShowCam, GroupSP flightShowScene) {

    flightShowCam->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);

    //Scene bauen mittell helper factory
    SceneObjetFactory *insta = SceneObjetFactory::getInstance(viewer);




    //flugzeug Links
    TransformationSP showFlugzeug1 = insta->getFlugzeug();
    showFlugzeug1->scale(glm::vec3(5., 5., 5.));
    showFlugzeug1->translate(glm::vec3(0, 2, 0));
    showFlugzeug1->translate(glm::vec3(10.0, 0., 6.));


    //Flugzeug Rechts
    TransformationSP showFlugzeug2 = insta->getFlugzeug();
    showFlugzeug2->scale(glm::vec3(5., 5., 5.));
    showFlugzeug2->rotate(180, glm::vec3(0., 1., 0.));
    showFlugzeug2->translate(glm::vec3(-8., 0., -12.));
    showFlugzeug2->translate(glm::vec3(0, 2, 0));

    //Licht holen
    LightFactory *lightFactory = LightFactory::getInstance();


    //SHOWSCENE
    //Der show das Licht hinzufügen
    flightShowScene->addChild(lightFactory->getVideoSonne());
    lightFactory->getVideoSonne()->addChild(flightShowCam);
    //flightShowScene->addChild(flightShowCam);

    //Transformationen dem Licht hinzugfügen
    lightFactory->getVideoSonne()->addChild(insta->createFlugzeugGruppe());
    lightFactory->getVideoSonne()->addChild(insta->getHimmel());
    lightFactory->getVideoSonne()->addChild(insta->getFloor());
    lightFactory->getSonne()->addChild(showFlugzeug1);
    lightFactory->getVideoSonne()->addChild(showFlugzeug2);

    TransformAnimationSP transAniShow = TransformAnimation::create();


    transAniShow->setUpdateFunc(
            [showFlugzeug1, showFlugzeug2](TransformAnimation *anim, double currTime, double diffTime,
                                           double totalTime) {

                std::cout << "flugzeug1 :" << showFlugzeug1->getPosition().z << std::endl;
                std::cout << "flugzeug2 :" << showFlugzeug2->getPosition().z << std::endl;

                //durchgehender flug der flugzeuge
                showFlugzeug1->translate(glm::vec3(.0, .0, .01));
                showFlugzeug2->translate(glm::vec3(.0, .0, .01));

                //Aneinander vorbei fliegen
                if (showFlugzeug1->getPosition().z > 6.3 && showFlugzeug1->getPosition().z < 6.5) {
                    showFlugzeug1->rotate(5.0, glm::vec3(.0, .0, -.3));

                }
                if (showFlugzeug2->getPosition().z >-11.7 && showFlugzeug2->getPosition().z<-11.5) {
                    showFlugzeug2->rotate(5.0, glm::vec3(.0, .0, -.3));

                }

                //wieder normal fliegen
                if (showFlugzeug1->getPosition().z > 12.7 && showFlugzeug1->getPosition().z < 12.9) {
                    showFlugzeug1->rotate(8.0, glm::vec3(.0, .0, .3));

                }
                if (showFlugzeug2->getPosition().z >-5.2 && showFlugzeug2->getPosition().z<-5.0) {
                    showFlugzeug2->rotate(8.0, glm::vec3(.0, .0, .3));

                }



            });

    viewer->addAnimation(transAniShow);

//hier nochmal gucken schwarzes bild nach setten der scene
// EnvoirementController::createVideoSceneHelper(viewer, camera, flightShowScene);

}
