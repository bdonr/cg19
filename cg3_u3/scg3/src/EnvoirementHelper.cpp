//
// Created by don on 21.05.19.
//

#include <iostream>
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturHelper.h"


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



void EnvoirementHelper::createSunFloorscene(ViewerSP viewer, CameraSP camera, GroupSP &scene){


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

    auto TransAni2 = TransformAnimation::create();


    GeometryCoreFactory geometryFactory;


    auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/hannover.obj");
    auto floor = Shape::create();
    floor->addCore(skybox)->addCore(MatFactory::getWhite())->addCore(TexturHelper::getStadt())
            ->addCore(floorCore);
    auto floorTrans = Transformation::create();
    floorTrans->scale(glm::vec3(0.007, 0.007, 0.007));

    auto himmelCore = geometryFactory.createSphere(20, 101, 110);
    auto nachtHimmelCore = geometryFactory.createHalfSphere(20, 101, 110);
    auto jetcore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
    auto camObjectCore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");

    //ShapeSP nachtHimmel = getPtr(shaderPhongTex2, matNacht, himmelTex, himmelCore);
    ShapeSP himmel = getPtr(shaderPhongTex2, MatFactory::getTag(), TexturHelper::getHimmel(), himmelCore);
    ShapeSP jet = getPtr(shaderPhongTex, MatFactory::getWhite(), TexturHelper::getBrick(), jetcore);
    ShapeSP camObject = getPtr(shaderPhongTex, MatFactory::getWhite(),TexturHelper::getBrick(), camObjectCore);


    auto flug = Group::create();
    flug->addCore(shaderPhongTex)
            ->addCore(MatFactory::getWhite())
            ->addCore(TexturHelper::getBrick());
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

    scene->addChild(atmosAnim);
    scene->addChild(sonne);
    sonne->addChild(camera);
    sonne->addChild(himmel);
    sonne->addChild(floorTrans);

    camObjectTrans->addChild(camObject);
    camera->addChild(camObjectTrans);
    viewer->startAnimations();
}