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

void checkChooseScene(ViewerSP &viewer, CameraSP &camera);

/**
 * \brief Create a scene consisting of a teapot, a camera, and a light.
 */
void createTeapotScene(ViewerSP viewer, CameraSP camera, GroupSP &scene);


/**
 * \brief Create a scene consisting of a floor, a table, a teapot, a camera, and a light.
 */
void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP &scene);

void createScene2(ViewerSP viewer, CameraSP camera);

void createScene1(ViewerSP viewer, CameraSP camera);

/**
 * \brief The main function.
 */
double bulletTravel = 0;
int chooseScene = 1;
GroupSP scene1;
GroupSP scene2;
StandardRendererSP renderer;

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
    CameraSP camera = PerspectiveCamera::create();
    camera->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);
    renderer->setCamera(camera);


    createScene1(viewer, camera);
    createScene2(viewer, camera);
    checkChooseScene(viewer, camera);
    renderer->setScene(scene1);

    viewer->startAnimations()
            ->startMainLoop();

}


void createTeapotScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {
    renderer->setScene(scene2);
}


void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {
    renderer->setScene(scene1);

}

void checkChooseScene(ViewerSP &viewer, CameraSP &camera) {
    switch (chooseScene) {
        case 0:
            createTeapotScene(viewer, camera, scene1);
            break;
        case 1:
            createTableScene(viewer, camera, scene2);
            break;
        default:
            throw std::runtime_error("Invalid value of SCGConfiguration::sceneType [main()]");
    }
}


void createScene1(ViewerSP viewer, CameraSP camera) {
    scene1 = Group::create();
    EnvoirementController::createVideoScene(viewer, camera, scene1);
}

void createScene2(ViewerSP viewer, CameraSP camera) {
    scene2 = Group::create();

    TransformAnimationSP transAni = TransformAnimation::create();
    transAni->setUpdateFunc(
            [&camera, &viewer](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {

                if (chooseScene == 1) {
                    chooseScene = 0;
                    checkChooseScene(viewer, camera);
                    chooseScene = 3;
                }

            }
    );
}
