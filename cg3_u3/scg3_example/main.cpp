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

MaterialCoreSP
createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine);

Texture2DCoreSP createTexture(TextureCoreFactory &textureFactory, const std::string &name);

Texture2DCoreSP createTextureMip(TextureCoreFactory &textureFactory, const std::string &name);
Texture2DCoreSP createTextureBumb(TextureCoreFactory &textureFactory, const std::string &name,const std::string &normalFileName);
TransformationSP
createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);

ShapeSP
getPtr(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur, const GeometryCoreSP &core);

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


    camera->translate(glm::vec3(0.f, 0.f, 1.f))
            ->dolly(-1.f);

    viewer->addAnimation(scg::AnimationSP());
    viewer->startMainLoop();
}

MaterialCoreSP
createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine) {
    auto mat = MaterialCore::create();
    mat->setAmbient(ambient)->setDiffuse(diffuse)
            ->setSpecular(specular)
            ->setShininess(shine)
            ->init();
    return mat;
}
MaterialCoreSP
createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine,const glm::vec4& emission) {
    auto mat = MaterialCore::create();
    mat->setAmbient(ambient)->setDiffuse(diffuse)
            ->setSpecular(specular)
            ->setShininess(shine)->setEmission(emission)
            ->init();
    return mat;
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

    ShaderCoreFactory shaderFactory("../scg3/shaders;../../scg3/shaders");
    double y = 0.0;
#ifdef SCG_CPP11_INITIALIZER_LISTS
    // Phong shader
    auto shaderPhong = shaderFactory.createShaderFromSourceFiles(
            {
                    ShaderFile("phong_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("phong_frag.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                    ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
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
                    ShaderFile("bump_vert.glsl", GL_VERTEX_SHADER),
                    ShaderFile("bump_frag.glsl", GL_FRAGMENT_SHADER),
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
    sonne->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))->setAmbient(
                    glm::vec4(.4, .4, .4, 1))->setSpot(glm::normalize(glm::vec4(1,1,1,1)),180.f,.1f)
            ->setPosition(glm::vec4(0.f, 36.f, 0, 1.f))
            ->init();
    auto matStern = Light::create();
    matStern->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(.1, .1, .1, 1))->setAmbient(
                    glm::vec4(.1, .1, .1, 1))
            ->setPosition(glm::vec4(0.f, -15.f, 0, 1.f))
            ->init();

    LightSP mats[50];
    LightPositionSP lightPositionSp[50];
    createSterne(mats, lightPositionSp);
    auto l = LightPosition::create(sonne);

    // materials
    MaterialCoreSP matRed = createMat(glm::vec4(1.f, 0.5f, 0.5f, 1.f), glm::vec4(1.f, 0.5f, 0.5f, 1.f),
                                      glm::vec4(1.f, 1.f, 1.f, 1.f), 20.0);
    MaterialCoreSP matWhite = createMat(glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.f), 200.0);
    MaterialCoreSP matGreen = createMat(glm::vec4(0.1f, 0.8f, 0.3f, 1.f), glm::vec4(0.1f, 0.8f, 0.3f, 1.f),
                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.f), 20.0);
    MaterialCoreSP matBlack = createMat(glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec4(1.f, 1.f, 1.f, 1.f),
                                        20.0);
    //  MaterialCoreSP matNacht = createMat(glm::vec4(0, .7, 1, 1), glm::vec4(.1, .1, .1, .1), glm::vec4(1.f, 1.f, 1.f, 1.f), 20000000.0);
    MaterialCoreSP matTag = createMat(glm::vec4(0.f, .7f, 1.f, 1), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                                     0.0299,glm::vec4(0.f,0.f,.0f,.1));


    auto TransAni2 = TransformAnimation::create();


    // textures
    TextureCoreFactory textureFactory("../scg3/textures;../../scg3/textures");
    Texture2DCoreSP texWood = createTexture(textureFactory, "wood_256.png");
    Texture2DCoreSP texBrick = createTexture(textureFactory, "neu1.png");
    Texture2DCoreSP texStadt = createTextureBumb(textureFactory, "ct-map.png","ct-map-bumb.png");
    Texture2DCoreSP himmelTex = createTexture(textureFactory, "panorama-fake-sky.png");
    // Texture2DCoreSP nachtTex = createTextureMip(textureFactory, "panorama-fake-sky.png");

    GeometryCoreFactory geometryFactory;


    auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
    auto floor = Shape::create();
    floor->addCore(skybox)->addCore(matWhite)->addCore(texStadt)
            ->addCore(floorCore);
    auto floorTrans = Transformation::create();
    floorTrans->scale(glm::vec3(0.07, 0.07, 0.07));

    auto himmelCore = geometryFactory.createSphere(20, 101, 110);
    auto nachtHimmelCore = geometryFactory.createHalfSphere(20, 101, 110);
    auto jetcore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
    auto camObjectCore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");

    //ShapeSP nachtHimmel = getPtr(shaderPhongTex2, matNacht, himmelTex, himmelCore);
    ShapeSP himmel = getPtr(shaderPhongTex2, matTag, himmelTex, himmelCore);
    ShapeSP jet = getPtr(shaderPhongTex, matWhite, texBrick, jetcore);
    ShapeSP camObject = getPtr(shaderPhongTex, matWhite, texBrick, camObjectCore);


    auto flug = Group::create();
    flug->addCore(shaderPhongTex)
            ->addCore(matWhite)
            ->addCore(texWood);
    auto flugTrans = Transformation::create();

    auto atmos = Group::create();
    atmos->addChild(l);
    for (int i = 0; i < 50; i++) {
        atmos->addChild(lightPositionSp[i]);
    }


    auto TransAni4 = TransformAnimation::create();
    TransAni4->setUpdateFunc([](TransformAnimation *anim4, double currTime, double diffTime, double totalTime) {
        anim4->rotate(-.1f, glm::vec3(0, 0, 0.1));
    });

    viewer->addAnimation(TransAni4);
    viewer->startAnimations();
    //  TransformationSP nachtHimmelTrans = createTransformation(glm::vec3(0,0,0),glm::vec3(1,1,1),glm::vec3(1,0,0),2000000.f);
    TransformationSP himmelTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
                                                        4.f);
    TransformationSP jetTrans = createTransformation(glm::vec3(4.3f, 0.2f, 0.3f), glm::vec3(0.05, 0.05, 0.05),
                                                     glm::vec3(0.f, 1.f, 0.f), -90.f);
    TransformationSP camObjectTrans = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(0.05, 0.05, 0.05),
                                                           glm::vec3(0.f, 1.f, 0.f), 180.);


    himmelTrans->addChild(himmel);
//    nachtHimmelTrans->addChild(nachtHimmel);


    camObjectTrans->setVisible(true);

    flugTrans->addChild(camObjectTrans);
    floorTrans->addChild(floor);





//--------------------------------####################################################################
    // teapot shape and transf
    // ormation

    auto teapotAnim = TransformAnimation::create();
    float angularVel = .0011f;
    glm::vec3 axis(0.f, 0.f, 1.f);
    teapotAnim->setUpdateFunc(
            [angularVel, axis](TransformAnimation* animation,
                               double currTime, double diffTime, double totalTime) {
                animation->rotate(angularVel, axis);
            });
    viewer->addAnimation(teapotAnim);
// add transformation (translation) to be applied before animation
    auto teapotAnimTrans = Transformation::create();
    teapotAnimTrans->translate(glm::vec3(0.3f, 0.f, 0.f));


    auto teapotTrans = Transformation::create();
    teapotTrans->rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));

    teapotAnim->addChild(teapotAnimTrans);
    teapotAnimTrans->addChild(sonne);





// add transformation (translation) to be applied before animation

// add transformation (translation) to be applied before animation
    auto atmosAnim = TransformAnimation::create();
    float angularVel2 = .001f;
    glm::vec3 axis2(0.f, 0.f, 1.f);
    atmosAnim->setUpdateFunc(
            [angularVel2, axis2](TransformAnimation* animation,
                               double currTime, double diffTime, double totalTime) {
                animation->rotate(angularVel2, axis2);
            });
    viewer->addAnimation(atmosAnim);
// add transformation (translation) to be applied before animation
    auto atmosAnimTrans = Transformation::create();

    auto atmosTrans = Transformation::create();
    atmosTrans->rotate(180.f, glm::vec3(0.f, 0.f, 1.f));

    atmosAnim->addChild(atmosAnimTrans);
    atmosAnimTrans->addChild(atmos);








    scene = Group::create();
    scene->addChild(atmosAnim);
    scene->addChild(sonne);
    sonne->addChild(camera);
   sonne->addChild(himmel);
    sonne->addChild(floorTrans);

//    mats[49]->addChild(nachtHimmelTrans);


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

ShapeSP
getPtr(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
       const GeometryCoreSP &core) {
    auto nachtHimmel = Shape::create();
    nachtHimmel->addCore(mat)
            ->addCore(shade)
            ->addCore(textur)
            ->addCore(core);
    return nachtHimmel;
}

Texture2DCoreSP createTexture(TextureCoreFactory &textureFactory, const std::string &name) {
    auto texWood = textureFactory.create2DTextureFromFile(
            name, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

Texture2DCoreSP createTextureBumb(TextureCoreFactory &textureFactory, const std::string &name,const std::string &normalFileName) {
    auto texWood = textureFactory.createBumpMapFromFiles(name,normalFileName,GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}
Texture2DCoreSP createTextureMip(TextureCoreFactory &textureFactory, const std::string &name) {
    auto texWood = textureFactory.create2DTextureFromFile(
            name, GL_REPEAT, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
    return texWood;
}

TransformationSP
createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree) {
    auto jetTrans = Transformation::create();
    jetTrans->translate(translate);
    jetTrans->rotate(degree, rotate);
    jetTrans->scale(scale);
    return jetTrans;
}

void createSterne(LightSP *mats, LightPositionSP *lightPositionSp) {

    for (int j = 0; j < 50; j++) {
        float sx1 = rand() % 800 + 1;
        float sy1 = rand() % 180 + 19;
        float sz1 = rand() % 800 + 1;
        std::cout << sx1 << " " << sy1 << " " << sz1 << std::endl;
        mats[j] = Light::create();
        mats[j]->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(.0, 0, 0, 1))->setAmbient(
                        glm::vec4(.1, .1, .1, 1))

                ->init();
        mats[j]->setPosition(glm::vec4(sx1, sy1, sz1, 1));
        lightPositionSp[j] = LightPosition::create(mats[j]);
    }
}

int dmod(double time, double d) {
    return time - (int) (time / d) * d;
}
