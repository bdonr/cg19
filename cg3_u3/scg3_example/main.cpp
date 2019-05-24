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

#include <src/EnvoirementHelper.h>
#include <thread>

//#include <scg3.h>
#include "../scg3/scg3.h"
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturHelper.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"
#include "GameLogic.h"
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
 * \brief Create a scene consisting of a teapot, a camera, and a light.
 */
void createTeapotScene(ViewerSP viewer, CameraSP camera, GroupSP &scene);


/**
 * \brief Create a scene consisting of a floor, a table, a teapot, a camera, and a light.
 */
void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP &scene);

void bulletTravelAndTest();

int dmod(double time, double d);

void createSterne(LightSP *mats, LightPositionSP *lightPositionSp);

MaterialCoreSP
createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine);

Texture2DCoreSP createTexture(TextureCoreFactory &textureFactory, const std::string &name);

Texture2DCoreSP createTextureMip(TextureCoreFactory &textureFactory, const std::string &name);
Texture2DCoreSP createTextureBumb(TextureCoreFactory &textureFactory, const std::string &name,const std::string &normalFileName);
TransformationSP
createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);

ShapeSP
getPtr(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur, const GeometryCoreSP &core);

void logic(CameraSP &camera, TransformationSP &ZielKugelTrans1, TransformationSP &ZielKugelTrans2,
           TransformationSP &ZielKugelTrans3, ShapeSP &kugel1, ShapeSP &kugel2, ShapeSP &kugel3,
           TransformationSP &bulletTrans,LightSP light,ViewerSP viewer);

void checkDurchflugZielscheibe(const glm::vec3 &camObjPos,double time);

/**
 * \brief The main function.
 */
double bulletTravel = 0;

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


// Minimal application using a simple viewer.
void useSimpleViewer() {

    // create viewer with default renderer, camera, and light
    auto viewer = Viewer::create();
    CameraSP camera;
    GroupSP scene;
    LightSP light;
    viewer->initSimpleRenderer(camera, scene, light);


    camera->translate(glm::vec3(0.f, 0.f, 1.f))
            ->dolly(-1.f);

    viewer->addAnimation(scg::AnimationSP());
    viewer->startMainLoop();
}



// Typical application using a customized viewer.
void useCustomizedViewer() {

    // create viewer and renderer
    auto viewer = Viewer::create();
    auto renderer = StandardRenderer::create();
    viewer->init(renderer)
            ->createWindow("s c g 3   e x a m p l e", 1024, 768);


    // create camera
    auto camera = PerspectiveCamera::create();
    camera->translate(glm::vec3(0.f, 0.f, 3.f))
            ->dolly(-1.f);
    renderer->setCamera(camera);

    // create scene
    GroupSP scene;
    switch (SCGConfiguration::sceneType) {
        case 0:
            createTeapotScene(viewer, camera, scene);
            break;
        case 1:
            createTableScene(viewer, camera, scene);
            break;
        default:
            throw std::runtime_error("Invalid value of SCGConfiguration::sceneType [main()]");
    }
    renderer->setScene(scene);

    // start animations, enter main loop

    viewer->startAnimations()
            ->startMainLoop();
}



void createTeapotScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {

}


void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {


    TransformationSP ZielKugelTrans1;
    TransformationSP ZielKugelTrans2;
    TransformationSP ZielKugelTrans3;
    ShapeSP kugel1;
    ShapeSP kugel2;
    ShapeSP kugel3;
    TransformationSP bulletTrans;
    GameLogic::logic(camera, ZielKugelTrans1, ZielKugelTrans2, ZielKugelTrans3, kugel1, kugel2, kugel3, bulletTrans,SceneObjetFactory::getSonne(),viewer);


    // set texture matrix
//  texWood->scale2D(glm::vec2(4.f, 4.f));

    // floor shape and transformation
    GeometryCoreFactory geometryFactory;
    

    auto bulletCore = geometryFactory.createSphere(0.005, 10, 10);
    auto bullet = Shape::create();
    bullet->addCore(ShaderFactory::getPhong(true))
            ->addCore(MatFactory::getRed())
            ->addCore(bulletCore);
    // create scene graph
    scene = Group::create();

    KeyboardControllerSP controller = KeyboardController::create(camera);
    viewer->addController(controller);
    EnvoirementHelper::createSunFloorscene(viewer,camera,scene);
    SceneObjetFactory::getSonne()->addChild(ZielKugelTrans1)
            ->addChild(ZielKugelTrans2)
            ->addChild(ZielKugelTrans3)->addChild(camera);
    bulletTrans->addChild(bullet);
    bulletTrans->setVisible(false);
    camera->addChild(bulletTrans);

    viewer->startAnimations();
    controller->setDing(bulletTrans);

}


