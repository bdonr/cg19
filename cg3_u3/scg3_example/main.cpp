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
//#include <scg3.h>
#include "../scg3/scg3.h"

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

    // define red material
    auto matRed = MaterialCore::create();
    matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))
            ->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setShininess(20.f)
            ->init();

    // add teapot shape to scene graph
    GeometryCoreFactory geometryFactory;
    auto teapot = Shape::create();
    teapot->addCore(matRed)
            ->addCore(geometryFactory.createTeapot(1.f));
    light->addChild(teapot);

    //add wall to scene Graph
    /*   auto wall = Shape::create();
       wall->addCore(matRed)
               ->addCore(geometryFactory.createRectangle(glm::vec2(20,30)));
       light->addChild(wall);
   */
    // move camera backwards, enter main loop
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

    ShaderCoreFactory shaderFactory("../scg3/shaders;../../scg3/shaders");

#ifdef SCG_CPP11_INITIALIZER_LISTS
    // Gouraud shader
    auto shaderPhong = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert_inner.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag_inner.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER)
            });

    auto shaderPhong2 = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER)
            });

    auto shaderPhongTex = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert_inner.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag_inner.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
            });

    auto shaderPhongTex2 = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
            });
#else
    std::vector<ShaderFile> shaderFiles;
    shaderFiles.push_back(ShaderFile("simple_gouraud_vert.glsl", GL_VERTEX_SHADER));
    shaderFiles.push_back(ShaderFile("simple_gouraud_frag.glsl", GL_FRAGMENT_SHADER));
    auto shaderGouraud = shaderFactory.createShaderFromSourceFiles(shaderFiles);
#endif
    TextureCoreFactory textureFactory("../scg3/textures;../../scg3/textures");
    auto texWood = textureFactory.create2DTextureFromFile(
            "panorama-fake-sky.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto texStadt = textureFactory.create2DTextureFromFile(
            "ct-map.png", GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
    // camera controllers
    camera->translate(glm::vec3(-2.f, 5.f, 0.f))
            ->dolly(-1.f);
#ifdef SCG_CPP11_INITIALIZER_LISTS
    viewer->addControllers(
            {
                    KeyboardController::create(camera),
                    MouseController::create(camera)
            });
#else
    viewer->addController(KeyboardController::create(camera))
        ->addController(MouseController::create(camera));
#endif

    // white point light at position (10,10,10)
    auto light = Light::create();
    auto l = LightPosition::create(light);
    auto light2 = Light::create();

    light2->setDiffuseAndSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setAmbient(glm::vec4(1, 1, 1, 1))
            ->setPosition(glm::vec4(-4.f, 1.f, 0.f, 1.f))
            ->init();

    light->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setAmbient(glm::vec4(1, 1, 1, 1))
            ->setPosition(glm::vec4(0.f, 0.f, 0.f, 1.f))
            ->init();
    auto matRed = MaterialCore::create();
    matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))
            ->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setShininess(20.f)
            ->init();

    auto lightTrans = Transformation::create();
    lightTrans->translate(glm::vec3(4.3f, 0.2f, 0.3f));
    lightTrans->scale(glm::vec3(0.05, 0.05, 0.05));
    lightTrans->rotate(-90, glm::vec3(0.f, 1.f, 0.f));

    lightTrans->addChild(light2);

    // red material
    auto matWhite = MaterialCore::create();
    matWhite->setAmbientAndDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.f))
            ->setShininess(20.f)
            ->init();
    GeometryCoreFactory geometryFactory;
    auto matGreen = MaterialCore::create();
    matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.f))
            ->init();
    auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
    auto floor = Shape::create();
    floor->addCore(shaderPhongTex2)->addCore(matWhite)->addCore(texStadt)
            ->addCore(floorCore);
    auto floorTrans = Transformation::create();
    floorTrans->scale(glm::vec3(0.07, 0.07, 0.07));

    floorTrans->addChild(floor);
    // teapot shape

    auto front = geometryFactory.createSphere(20, 101, 110);
    auto himmel = Shape::create();
    himmel->addCore(shaderPhongTex2)
            ->addCore(matWhite)
            ->addCore(texWood)
            ->addCore(front);

    auto TransAni = TransformAnimation::create();
    auto TransAni2 = TransformAnimation::create();
    auto textTrans = Transformation::create();


    TransAni->setUpdateFunc([](TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
        anim->rotate(30.f, glm::vec3(1.f, 1.f, 0.f));
    });
    TransAni2->setUpdateFunc([&light](TransformAnimation *anim2, double currTime, double diffTime, double totalTime) {
        anim2->rotate(30.f, glm::vec3(1.f, 1.f, 0.f));
        if (totalTime > 10) {
            light->setAmbient(glm::vec4(0, 1, 0, 1));
        }
    });


    // teapot transformation



    auto horizontTrans = TransAni;

    horizontTrans->translate(glm::vec3(0, 0, 0))->rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));
    horizontTrans->addChild(himmel);
    // create scene graph
    scene = Group::create();
    scene->addCore(shaderPhong);
    scene->addChild(camera)
            ->addChild(light)->addChild(light2);
    light->addChild(horizontTrans);
    light2->addChild(floorTrans);
}


void createTableScene(ViewerSP viewer, CameraSP camera, GroupSP &scene) {

    ShaderCoreFactory shaderFactory("../scg3/shaders;../../scg3/shaders");
    double x = 0.0;
    double y = 0.0;
#ifdef SCG_CPP11_INITIALIZER_LISTS
    // Phong shader
    auto shaderPhong = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER)
            });

    // Phong shader with texture mapping
    auto shaderPhongTex = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
            });

    auto skybox = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("color_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("color_frag.glsl", GL_FRAGMENT_SHADER),
                    //  ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    //   ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
            });

    auto shaderPhongTex2 = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert_inner.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag_inner.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
            });

#else
    // Phong shader
    std::vector<ShaderFile> shaderFiles;
    shaderFiles.push_back(ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER));
    shaderFiles.push_back(ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER));
    shaderFiles.push_back(ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER));
    shaderFiles.push_back(ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER));
    auto shaderPhong = shaderFactory.createShaderFromSourceFiles(shaderFiles);

    // Phong shader with texture mapping
    shaderFiles.clear();
    shaderFiles.push_back(ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER));
    shaderFiles.push_back(ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER));
    shaderFiles.push_back(ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER));
    shaderFiles.push_back(ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER));
    auto shaderPhongTex = shaderFactory.createShaderFromSourceFiles(shaderFiles);
#endif

    // camera controllers
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);
    KeyboardControllerSP controller = KeyboardController::create(camera);
#ifdef SCG_CPP11_INITIALIZER_LISTS
    viewer->addControllers(
            {
                    controller,
                    MouseController::create(camera)
            });
#else
    viewer->addController(KeyboardController::create(camera))
          ->addController(MouseController::create(camera));
#endif



    // lights
    auto light = Light::create();
    light->setSpecular(glm::vec4(1.f, 1.f, 1.f, 0.f))->setDiffuse(glm::vec4(1, 1, 1, 1))->setAmbient(
                    glm::vec4(.1, .1, .1, 1))
            ->setPosition(glm::vec4(0.f, 15.f, 0.f, 1.f))
            ->init();

    auto l = LightPosition::create(light);

    // materials
    auto matBlack = MaterialCore::create();
    matBlack->setAmbientAndDiffuse(glm::vec4(0.f, 0.f, 0.f, 1.f))
            ->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setShininess(1.f)
            ->init();

    auto matRed = MaterialCore::create();
    matRed->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))
            ->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setShininess(20.f)
            ->init();

    auto matGreen = MaterialCore::create();
    matGreen->setAmbientAndDiffuse(glm::vec4(0.1f, 0.8f, 0.3f, 1.f))
            ->init();

    auto matWhite = MaterialCore::create();
    matWhite->setAmbientAndDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setSpecular(glm::vec4(0.5f, 0.5f, 0.5f, 1.f))
            ->setShininess(20.f)
            ->init();

    auto lichthell = MaterialCore::create();
    lichthell//->setAmbientAndDiffuse(glm::vec4(1.f, 0.5f, 0.5f, 1.f))
            ->setAmbient(glm::vec4(1.f, 1.f, 0.f, 1.f))
            ->setDiffuse(glm::vec4(1.f, 1.f, 0.f, 1.f))
            ->setSpecular(glm::vec4(1.f, 1.f, 0.f, 1.f))
            ->setShininess(51.f)
            ->setEmission(glm::vec4(1.f, 1.f, 0.f, 1.f))
            ->init();

    auto TransAni = TransformAnimation::create();
    auto ZielKugelTrans1 = Transformation::create();
    auto ZielKugelTrans2 = Transformation::create();
    ZielKugelTrans2->setVisible(false);
    auto ZielKugelTrans3 = Transformation::create();
    ZielKugelTrans3->setVisible(false);
    auto kugel1 = Shape::create();
    auto kugel2 = Shape::create();
    auto kugel3 = Shape::create();
    auto camObjectTrans = Transformation::create();

    auto bulletTrans = Transformation::create();
    bulletTrans->translate(glm::vec3(0.02f, -0.08f, -0.2f));

    TransAni->setUpdateFunc(
            [camera, light, bulletTrans, ZielKugelTrans1, ZielKugelTrans2, ZielKugelTrans3, camObjectTrans, kugel1, kugel2, kugel3](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
/* die projektile bewegen sich die ganze zeit, sie laufen vorwärts und springen dan beim errreichen
 * der maximal distanz zurück, die kugeln sind daurhaft unsichbar nur beim drücken von SPACE werden
 * diese sichtbar und können interagieren
 * */
                bulletTrans->translate(glm::vec3(0, 0., -0.1));
                bulletTravelAndTest();
                if (bulletTravel > 5) {
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

                double rad1 = 0.04;
                double rad2 = 0.08;
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
                }
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
    auto TransAni2 = TransformAnimation::create();
    auto atmos = Group::create();
    atmos->addChild(l);

    auto atmosTrans = Transformation::create();


    // textures
    TextureCoreFactory textureFactory("../scg3/textures;../../scg3/textures");
    auto texWood = textureFactory.create2DTextureFromFile(
            "wood_256.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto texBrick = textureFactory.create2DTextureFromFile(
            "neu1.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto sonne = textureFactory.create2DTextureFromFile(
            "sonne.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto texStadt = textureFactory.create2DTextureFromFile(
            "ct-map.png", GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

    auto himmelTex = textureFactory.create2DTextureFromFile(
            "panorama-fake-sky.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto bulletTex = textureFactory.create2DTextureFromFile(
            "metal_scratches.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto nachtTex = textureFactory.create2DTextureFromFile(
            "1280px-carina_nebula.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    // set texture matrix
//  texWood->scale2D(glm::vec2(4.f, 4.f));

    // floor shape and transformation
    GeometryCoreFactory geometryFactory;
    auto light3 = Light::create();

    light3->setDiffuseAndSpecular(glm::vec4(1.f, 0.f, 0.f, 0.f))->setAmbient(glm::vec4(1, 0, 0, 1))
            ->setPosition(glm::vec4(-4.f, 1.f, 0.f, 1.f))
            ->init();

    auto himmelCore = geometryFactory.createSphere(20, 101, 110);
    auto himmel = Shape::create();
    himmel->addCore(shaderPhongTex2)
            ->addCore(matWhite)
            ->addCore(himmelTex)
            ->addCore(himmelCore);

    auto nachtCore = geometryFactory.createSphere(20, 101, 110);
    auto nachtHimmel = Shape::create();
    nachtHimmel->addCore(shaderPhongTex2)
            ->addCore(matWhite)
            ->addCore(nachtTex)
            ->addCore(nachtCore);
    auto nachtTrans = Transformation::create();


    auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
    auto floor = Shape::create();
    floor->addCore(shaderPhongTex)->addCore(matWhite)->addCore(texStadt)
            ->addCore(floorCore);
    auto floorTrans = Transformation::create();
    floorTrans->scale(glm::vec3(0.07, 0.07, 0.07));
    auto TransAni3 = TransformAnimation::create();

    TransAni2->setUpdateFunc([light, &atmos, &himmel, nachtTex, himmelTex, matWhite, shaderPhongTex2, &geometryFactory](
            TransformAnimation *anim2, double currTime, double diffTime, double totalTime) {
        anim2->rotate(.005f, glm::vec3(.0f, .0f, .1f));

    });
    auto horizontTrans = Transformation::create();
    horizontTrans->translate(glm::vec3(0, 0, 0))->rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));
    horizontTrans = TransAni3;
    horizontTrans->addChild(himmel);

    viewer->addAnimation(TransAni);
    viewer->addAnimation(TransAni2);
    viewer->addAnimation(TransAni3);


    auto bulletCore = geometryFactory.createSphere(0.005, 10, 10);
    auto bullet = Shape::create();
    bullet->addCore(shaderPhong)
            ->addCore(matBlack)
            ->addCore(bulletCore);

    auto kugelcore1 = geometryFactory.createSphere(0.05, 100, 100);

    kugel1->addCore(shaderPhong)
            ->addCore(matRed)
            ->addCore(kugelcore1);

    ZielKugelTrans1->translate(glm::vec3(4.3f, 4.f, 2.3f));

    auto kugelcore2 = geometryFactory.createSphere(0.05, 100, 100);

    kugel2->addCore(shaderPhong)
            ->addCore(matRed)
            ->addCore(kugelcore2);

    ZielKugelTrans2->translate(glm::vec3(-2.3f, 7.f, -3.3f));

    auto kugelcore3 = geometryFactory.createSphere(0.05, 100, 100);

    kugel3->addCore(shaderPhong)
            ->addCore(matRed)
            ->addCore(kugelcore3);

    ZielKugelTrans3->translate(glm::vec3(0.3f, 4.f, 0.f));


    auto flug = Group::create();
    flug->addCore(shaderPhongTex)
            ->addCore(matWhite)
            ->addCore(texWood);
    auto flugTrans = Transformation::create();

    auto camObjectCore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
    auto camObject = Shape::create();
    camObject->addCore(shaderPhongTex)
            ->addCore(matWhite)
            ->addCore(texBrick)
            ->addCore(camObjectCore);

    camObjectTrans->translate(glm::vec3(0.f, -0.09f, -0.2f));
    camObjectTrans->scale(glm::vec3(0.05, 0.05, 0.05));
    camObjectTrans->rotate(180, glm::vec3(0.f, 1.f, 0.f));
    camObjectTrans->setVisible(true);

    flugTrans->addChild(camObjectTrans);

    atmosTrans = TransAni2;
    atmosTrans->addChild(atmos);
    // create scene graph
    scene = Group::create();
    scene->addCore(shaderPhong)->addChild(atmosTrans)->addChild(light);
    light->addChild(floorTrans)
            ->addChild(camera)
            ->addChild(ZielKugelTrans1)
            ->addChild(ZielKugelTrans2)
            ->addChild(ZielKugelTrans3);
    bulletTrans->addChild(bullet);
    bulletTrans->setVisible(false);
    floorTrans->addChild(floor);
    light->addChild(horizontTrans);
    camObjectTrans->addChild(camObject);
    ZielKugelTrans1->addChild(kugel1);
    ZielKugelTrans2->addChild(kugel2);
    ZielKugelTrans3->addChild(kugel3);
    camera->addChild(camObjectTrans)
            ->addChild(bulletTrans);


    // cam an die transformation des obejktes binden
//TransAni->addChild(camera);
/*
#if SCG_MOVING_CAM
 TransAni->addChild(camera);
#else
  scene->addChild(camera);
#endif
 */
    controller->setDing(bulletTrans);
}

void bulletTravelAndTest() {
    if (bulletTravel > 5) {
        bulletTravel = 0;
    }
    bulletTravel = bulletTravel + 0.2;

}
