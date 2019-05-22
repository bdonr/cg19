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

//#include <scg3.h>
#include "../scg3/scg3.h"
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturHelper.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"
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
    logic(camera, ZielKugelTrans1, ZielKugelTrans2, ZielKugelTrans3, kugel1, kugel2, kugel3, bulletTrans,SceneObjetFactory::getSonne(),viewer);


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

void logic(CameraSP &camera, TransformationSP &ZielKugelTrans1, TransformationSP &ZielKugelTrans2,
           TransformationSP &ZielKugelTrans3, ShapeSP &kugel1, ShapeSP &kugel2, ShapeSP &kugel3,
           TransformationSP &bulletTrans,LightSP light,ViewerSP viewer) {
    ZielKugelTrans1 = SceneObjetFactory::getZielscheiben()[0];
    ZielKugelTrans2 = SceneObjetFactory::getZielscheiben()[1];
    ZielKugelTrans3 = SceneObjetFactory::getZielscheiben()[2];

    bulletTrans = Transformation::create();
    auto TransAni = TransformAnimation::create();
    ZielKugelTrans2->setVisible(false);
    ZielKugelTrans3->setVisible(false);
    auto camObjectTrans = Transformation::create();
    bulletTrans->translate(glm::vec3(0.02f, -0.08f, -0.2f));
    TransAni->setUpdateFunc(
            [camera, light, bulletTrans, ZielKugelTrans1, ZielKugelTrans2, ZielKugelTrans3, camObjectTrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
/* die projektile bewegen sich die ganze zeit, sie laufen vorwärts und springen dan beim errreichen
 * der maximal distanz zurück, die kugeln sind daurhaft unsichbar nur beim drücken von SPACE werden
 * diese sichtbar und können interagieren
 /            std::cout<<camera->getMatrix()[3][0];
             std::cout<<camera->getMatrix()[3][1]<<;
             std::cout<<camera->getMatrix()[3][2];
 **/               bulletTrans->translate(glm::vec3(0, 0., -0.02));
                bulletTravelAndTest();
                if (bulletTravel > 25) {
                    bulletTrans->translate(glm::vec3(0, 0, 2.5));
/*
 * 0.1 * 5 = 0.5 * 5 = 2.5 rechung für den zurücksprung
 */
                }
/*kolision detection beim rammen des flugzeugs gegen die objekte*/
                glm::mat4 tempCamObjMat = camera->getMatrix();
                tempCamObjMat = glm::translate(tempCamObjMat, glm::vec3(0, -0.08, -0.2));

                float CamObjX = tempCamObjMat[3][0];
                float CamObjY = tempCamObjMat[3][1];
                float CamObjZ = tempCamObjMat[3][2];

                glm::vec3 camObjPos = glm::vec3(CamObjX, CamObjY, CamObjZ);
                glm::vec3 kugelObjPos1 = glm::vec3(ZielKugelTrans1->getMatrix()[3][0],
                                                   ZielKugelTrans1->getMatrix()[3][1],
                                                   ZielKugelTrans1->getMatrix()[3][2]);
                glm::vec3 kugelObjPos2 = glm::vec3(ZielKugelTrans2->getMatrix()[3][0],
                                                   ZielKugelTrans2->getMatrix()[3][1],
                                                   ZielKugelTrans2->getMatrix()[3][2]);
                glm::vec3 kugelObjPos3 = glm::vec3(ZielKugelTrans3->getMatrix()[3][0],
                                                   ZielKugelTrans3->getMatrix()[3][1],
                                                   ZielKugelTrans3->getMatrix()[3][2]);
                glm::vec3 camSomeDif1 = camObjPos;
                camSomeDif1 -= kugelObjPos1;
                glm::vec3 camSomeDif2 = camObjPos;
                camSomeDif2 -= kugelObjPos2;
                glm::vec3 camSomeDif3 = camObjPos;
                camSomeDif3 -= kugelObjPos3;

                float diff1 = sqrt(pow(camSomeDif1.x, 2) + pow(camSomeDif1.y, 2) + pow(camSomeDif1.z, 2));
                float diff2 = sqrt(pow(camSomeDif2.x, 2) + pow(camSomeDif2.y, 2) + pow(camSomeDif2.z, 2));
                float diff3 = sqrt(pow(camSomeDif3.x, 2) + pow(camSomeDif3.y, 2) + pow(camSomeDif3.z, 2));

                double rad1 = 0.2;
                double rad2 = 0.3;


                std::vector<TransformationSP> x = SceneObjetFactory::getZielscheiben();

                for(int i =1; i<x.size(); i++){
                    glm::vec3 kk = glm::vec3(x[i-1]->getMatrix()[3][0],
                                    x[i-1]->getMatrix()[3][1],
                                    x[i-1]->getMatrix()[3][2]);
                    float diff1 = sqrt(pow(camObjPos.x-kk.x, 2) + pow(camObjPos.y-kk.y, 2) + pow(camObjPos.z-kk.z, 2));
                    if(x[i-1]->isVisible()) {
                        if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                            x[i-1]->setVisible(false);
                            x[i]->setVisible(true);
                        }
                    }
                    if(i==5){
                        i=0;
                    }
                }
                /**
                if (ZielKugelTrans1->isVisible()) {
                    if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                        ZielKugelTrans1->setVisible(false);
                        ZielKugelTrans2->setVisible(true);
                    }
                } else if (ZielKugelTrans2->isVisible()) {
                    if (glm::abs(rad1 - rad2) < diff2 && diff2 < (rad1 + rad2)) {
                        ZielKugelTrans2->setVisible(false);
                        ZielKugelTrans3->setVisible(true);
                    }
                } else if (ZielKugelTrans3->isVisible()) {
                    if (glm::abs(rad1 - rad2) < diff3 && diff3 < (rad1 + rad2)) {
                        ZielKugelTrans3->setVisible(false);
                        ZielKugelTrans1->setVisible(true);
                    }
                }**/
/*kolision detection beim treffen der projektile auf objecte*/
                glm::mat4 tempBulletMat = camera->getMatrix();
                tempBulletMat *= bulletTrans->getMatrix();

                float bulletX = tempBulletMat[3][0];
                float bulletY = tempBulletMat[3][1];
                float bulletZ = tempBulletMat[3][2];

                glm::vec3 bullObjPos = glm::vec3(bulletX, bulletY, bulletZ);
                glm::vec3 bullSomeDif1 = bullObjPos;
                bullSomeDif1 -= kugelObjPos1;
                glm::vec3 bullSomeDif2 = bullObjPos;
                bullSomeDif2 -= kugelObjPos2;
                glm::vec3 bullSomeDif3 = bullObjPos;
                bullSomeDif3 -= kugelObjPos3;

                float bulldiff1 = sqrt(pow(bullSomeDif1.x, 2) + pow(bullSomeDif1.y, 2) + pow(bullSomeDif1.z, 2));
                float bulldiff2 = sqrt(pow(bullSomeDif2.x, 2) + pow(bullSomeDif2.y, 2) + pow(bullSomeDif2.z, 2));
                float bulldiff3 = sqrt(pow(bullSomeDif3.x, 2) + pow(bullSomeDif3.y, 2) + pow(bullSomeDif3.z, 2));

                double bullrad1 = 0.1;
                double bullrad2 = 0.1;
                if (ZielKugelTrans1->isVisible()) {
                    if (glm::abs(bullrad1 - bullrad2) < bulldiff1 && bulldiff1 < (bullrad1 + bullrad2) &&
                        bulletTrans->isVisible()) {
                        ZielKugelTrans1->setVisible(false);
                        ZielKugelTrans2->setVisible(true);
                    }
                } else if (ZielKugelTrans2->isVisible()) {
                    if (glm::abs(bullrad1 - bullrad2) < bulldiff2 && bulldiff2 < (bullrad1 + bullrad2) &&
                        bulletTrans->isVisible()) {
                        ZielKugelTrans2->setVisible(false);
                        ZielKugelTrans3->setVisible(true);
                    }
                } else if (ZielKugelTrans3->isVisible()) {
                    if (glm::abs(bullrad1 - bullrad2) < bulldiff3 && bulldiff3 < (bullrad1 + bullrad2) &&
                        bulletTrans->isVisible()) {
                        ZielKugelTrans3->setVisible(false);
                        ZielKugelTrans1->setVisible(true);
                    }
                }


                if (camera->getPosition().x > 15) {
                    camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                } else if (camera->getPosition().x < -15) {
                    camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                } else if (camera->getPosition().z > 15) {
                    camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                } else if (camera->getPosition().z < -15) {
                    camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                } else if (camera->getPosition().y < 1.0) {
                    camera->rotate(-15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                } else if (camera->getPosition().y > 10.0) {
                    camera->rotate(15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                }

                /*
                 * interesant: die bewegung der kamera ist spiegelverkehrt wegen dem blick in die negative Z-Achse
                 * also bewegt sich die camera nach -Z
                 * aber
                 * auch die geschwindigkeit der transformation ist anders etwa halb so stark
                 * die rotation skalierung ist gleich allerdings richtet sich die cammere nach den weltcoordinaten aus
                 * und die objekte nach ihren eigenen UND die rations richtung ist spiegelverkehrt.
                 */
                // camera->rotate(-0.7f, glm::vec3(1.0f, 0.0f, 0.0f))->translate(glm::vec3(0, 0, -0.001));


                // std::cout << totalTime << std::endl;

            });
    viewer->addAnimation(TransAni);

}

void bulletTravelAndTest() {
    if (bulletTravel > 25) {
        bulletTravel = 0;
    }
    bulletTravel = bulletTravel + 0.2;




}

