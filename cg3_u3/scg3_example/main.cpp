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


int dmod(double time, double d);

void createSterne(LightSP *mats, LightPositionSP *lightPositionSp);

/**
 * \brief The main function.
 */
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



    // lights
    auto sonne = Light::create();
    sonne->setSpecular(glm::vec4(.1, .1f, .1f, 1.f))->setDiffuse(glm::vec4(.1, .1, .1, 1))->setAmbient(
                    glm::vec4(.1, .1, .1, 1))
            ->setPosition(glm::vec4(0.f, -15.f, 0, 1.f))
            ->init();

    auto matStern = Light::create();
    matStern->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(.1, .1, .1, 1))->setAmbient(
                    glm::vec4(.1, .1, .1, 1))
            ->setPosition(glm::vec4(0.f, -15.f, 0, 1.f))
            ->init();

    LightSP mats[50];
    LightPositionSP lightPositionSp[50];
     createSterne(mats,lightPositionSp);
    auto l = LightPosition::create(sonne);

    // materials
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

    auto matBlack = MaterialCore::create();
    matBlack->setAmbientAndDiffuse(glm::vec4(1, 1, 1, 1))
            ->setSpecular(glm::vec4(0.f, 0.f, 0.f, 1.f))->setShininess(20.f)
            ->init();

    auto matTag = MaterialCore::create();
    matTag->setAmbient(glm::vec4(0, .7, 1, 1))->setDiffuse(glm::vec4(.1, .1, .1, .1))
            ->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
            ->setShininess(20.f)
            ->init();


    auto matNacht = MaterialCore::create();
    matNacht->setAmbient(glm::vec4(0, .7, 1, 1))->setDiffuse(glm::vec4(.1, .1, .1, .1))
            ->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setShininess(2000000.f)
            ->init();


    auto TransAni = TransformAnimation::create();

    TransAni->setUpdateFunc(
            [camera, sonne](TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
                //anim->rotate(0.4f, glm::vec3(0.0f, 0.0f, 1.0f));
                /*  camera->translate(glm::vec3(0,0,-0.01));
                  std::cout<<"X= "<<camera->getPosition().x<<" Y= "<<camera->getPosition().y<<" Z= "<<camera->getPosition().z<<std::endl;
                  if(camera->getPosition().x>10){
                      camera->rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                  }else if(camera->getPosition().x<-10){
                      camera->rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                  }else if(camera->getPosition().z>10){
                      camera->rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                  }else if(camera->getPosition().z<-10){
                      camera->rotate(1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                  }
                  else if(camera->getPosition().y<1.0){
                      camera->rotate(-1.0f, glm::vec3(1.0f, 0.f, 0.0f));
                  }
                  else if(camera->getPosition().y>9.0){
                      camera->rotate(-1.0f, glm::vec3(1.0f, 0.f, 0.0f));
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
    for (int i = 0; i < 50; i++) {
        atmos->addChild(lightPositionSp[i]);
    }
    auto atmosTrans = Transformation::create();


    // textures
    TextureCoreFactory textureFactory("../scg3/textures;../../scg3/textures");
    auto texWood = textureFactory.create2DTextureFromFile(
            "wood_256.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto texBrick = textureFactory.create2DTextureFromFile(
            "neu1.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto sonneTex = textureFactory.create2DTextureFromFile(
            "sonne.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    auto texStadt = textureFactory.create2DTextureFromFile(
            "ct-map.png", GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

    auto himmelTex = textureFactory.create2DTextureFromFile(
            "halbkugel2.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    auto nachtTex = textureFactory.create2DTextureFromFile(
            "halbkugel2.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    GeometryCoreFactory geometryFactory;
    auto light3 = Light::create();

    light3->setDiffuseAndSpecular(glm::vec4(1.f, 1.f, 1.f, 0.f))->setAmbient(glm::vec4(.1, .1, .1, 1))
            ->setPosition(glm::vec4(-4.f, 1.f, 0.f, 1.f))->setSpecular(glm::vec4(1, 1, 1, 1))
            ->init();


    auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
    auto floor = Shape::create();
    floor->addCore(shaderPhongTex)->addCore(matWhite)->addCore(texStadt)
            ->addCore(floorCore);
    auto floorTrans = Transformation::create();
    floorTrans->scale(glm::vec3(0.07, 0.07, 0.07));
    auto TransAni3 = TransformAnimation::create();

    TransAni2->setUpdateFunc([](TransformAnimation *anim2, double currTime, double diffTime, double totalTime) {
        anim2->rotate(-.1f, glm::vec3(0.f, 0.f, 1.f));

    });
    auto himmelCore = geometryFactory.createHalfSphere(20, 101, 110);
    auto himmel = Shape::create();
    himmel->addCore(matTag)->addCore(shaderPhongTex2)
            ->addCore(matTag)->addCore(himmelTex)
            ->addCore(himmelCore);

    auto himmelTrans = Transformation::create();
    himmelTrans->addChild(himmel);

    auto nachtHimmelCore = geometryFactory.createHalfSphere(20, 101, 110);
    auto nachtHimmel = Shape::create();
    nachtHimmel->addCore(matNacht)
            ->addCore(shaderPhongTex2)
            ->addCore(himmelTex)
            ->addCore(himmelCore);

    auto nachtHimmelTrans = Transformation::create();

    TransAni3->setUpdateFunc([](TransformAnimation *anim3, double currTime, double diffTime, double totalTime) {
        // anim3->rotate(.01f, glm::vec3(0, 0, 0.1));

    });
    auto TransAni4 = TransformAnimation::create();
    TransAni4->setUpdateFunc([](TransformAnimation *anim4, double currTime, double diffTime, double totalTime) {
        //anim4->rotate(-.01f, glm::vec3(0, 0, 0.1));
    });
    viewer->addAnimation(TransAni);
    viewer->addAnimation(TransAni2);
    viewer->addAnimation(TransAni3);
    viewer->addAnimation(TransAni4);


    nachtHimmelTrans = TransAni3;
    nachtHimmelTrans->addChild(nachtHimmel);
    nachtHimmelTrans->translate(glm::vec3(0, 0, 0));
    nachtHimmelTrans->rotate(-180, glm::vec3(1, 0, 0));
    himmelTrans = TransAni4;
    himmelTrans->addChild(himmel);
    himmelTrans->translate(glm::vec3(0, 0, 0));


    auto jetcore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
    auto jet = Shape::create();
    jet->addCore(shaderPhongTex)
            ->addCore(matWhite)
            ->addCore(texBrick)
            ->addCore(jetcore);

    auto jetTrans = TransAni;
    jetTrans->translate(glm::vec3(4.3f, 0.2f, 0.3f));
    jetTrans->scale(glm::vec3(0.05, 0.05, 0.05));
    jetTrans->rotate(-90, glm::vec3(0.f, 1.f, 0.f));


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

    auto camObjectTrans = Transformation::create();
    camObjectTrans->translate(glm::vec3(0.f, -0.1f, -0.3f));
    camObjectTrans->scale(glm::vec3(0.05, 0.05, 0.05));
    camObjectTrans->rotate(180, glm::vec3(0.f, 1.f, 0.f));
    camObjectTrans->setVisible(true);

    flugTrans->addChild(camObjectTrans);



//--------------------------------####################################################################
    // teapot shape and transf
    // ormation



    atmosTrans = TransAni2;

    // create scene graph
    scene = Group::create();
    scene->addCore(shaderPhong)->addChild(atmosTrans)->addChild(sonne)->addChild(light3)->addChild(matStern);
    sonne->addChild(camera)->addChild(himmelTrans);
    for (int i = 0; i < 50; i++) {

        mats[i]->addChild(nachtHimmelTrans);
        scene->addChild(mats[i]);
    }
    atmosTrans->addChild(atmos);
    sonne->addChild(floorTrans);
    floorTrans->addChild(floor);
    camObjectTrans->addChild(camObject);
    camera->addChild(camObjectTrans);
    viewer->startAnimations();

    // cam an die transformation des obejktes binden
//TransAni->addChild(camera);
/*
#if SCG_MOVING_CAM
 TransAni->addChild(camera);
#else
  scene->addChild(camera);
#endif
 */
}

void createSterne(LightSP *mats, LightPositionSP *lightPositionSp) {

    for (int j = 0; j < 50; j++) {
        float sx1 = rand() % 8 + 1;
        float sy1 =rand() % 8 + 1;
        float sz1 = rand() % 8 + 1;
        std::cout<<sx1<<" "<<sy1<<" "<<sz1<<std::endl;
        mats[j] = Light::create();
        mats[j]->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(.1, 1, 1, 1))->setAmbient(
                        glm::vec4(.1, .1, .1, 1))

                ->init();
        mats[j]->setPosition(glm::vec4(sx1, sy1, sz1, 1));
        mats[j]->setSpot(glm::vec3(-.1, -.1, -.1), 20., 20.);
        lightPositionSp[j] = LightPosition::create(mats[j]);
    }
}

int dmod(double time, double d) {
    return time - (int) (time / d) * d;
}
