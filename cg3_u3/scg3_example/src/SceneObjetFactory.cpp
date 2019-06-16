//
// Created by don on 21.05.19.
//


#include <iostream>
#include "SceneObjetFactory.h"
#include "LightFactory.h"

SceneObjetFactory *SceneObjetFactory::instance;

const ShapeSP  SceneObjetFactory::getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
                            const GeometryCoreSP &core) {
    auto shape = Shape::create();
    shape->addCore(mat)
            ->addCore(shade)
            ->addCore(textur)
            ->addCore(core);
    return shape;
}

const TransformationExtSP SceneObjetFactory::createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate,
                                        float degree) {
    auto trans = TransformationExt::create();
    trans->translate(translate);
    trans->rotate(degree, rotate);
    trans->scale(scale);
    return trans;
}


const TransformationExtSP SceneObjetFactory::getJet() {

    auto jetcore = torusFactory.createModelFromOBJFile("../scg3/models/jet.obj");
    ShapeSP jet = getShape(shaderFactory->getPhong(true), matFactory->getWhite(), textureFactory->getBrick(),
                           jetcore);

    TransformationExtSP jetTrans = createTransformation(glm::vec3(4.3f, 0.2f, 0.3f), glm::vec3(0.05, 0.05, 0.05),
                                         glm::vec3(0.f, 1.f, 0.f), -90.f);

    jetTrans->addChild(jet);

    return jetTrans;
}


const ShapeSP SceneObjetFactory::getSky() {

    skybox = Shape::create();

    auto skyboxCore = torusFactory.createCube(50.f);
    skybox->addCore(shaderFactory->getSkyBox())
            ->addCore(textureFactory->getHimmel())
            ->addCore(skyboxCore);
    return skybox;
}

const ShapeSP SceneObjetFactory::getVideoSky() {
    if(videoSkybox== nullptr) {
        videoSkybox = Shape::create();

        auto skyboxCore = torusFactory.createCube(50.f);
        videoSkybox->addCore(shaderFactory->getSkyBox())
                ->addCore(textureFactory->getHimmel())
                ->addCore(skyboxCore);
    }
    return videoSkybox;
}


const TransformAnimationSP SceneObjetFactory::createToriWithAnimation() {
    auto torussGroup = Group::create();

    //unterschiedliche formen 3 eck bishin zu vieleck
    auto torusCore = torusFactory.createTorus(0.5, .1, 50, 40);
    std::vector<ShapeSP> torusShape;
    int j = 0;
    for (float i = 1.0f; i >= 0.0; i = i - 0.1) {
        createShapesWithDifferentColors(j, torusCore, torusShape);
        TransformAnimationSP TransAni = createTorusAnimation(torusCore, torusShape, j, i);
        if (j == 9) {
            torussGroup->addChild(getAnimatedStern());
        } else {
            torussGroup->addChild(TransAni);
        }
        std::cout << "j" << j << std::endl;
        j++;

    }
    TransformationSP grouptrans = Transformation::create();
    grouptrans->addChild(torussGroup);
    auto TransAni = TransformAnimation::create();
    TransAni->setUpdateFunc(
            [grouptrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
                grouptrans->rotate(1, glm::vec3(1, 0, 0));
            }
    );
    viewer->addAnimation(TransAni);
    grouptrans->scale(glm::vec3(.1, .1, .1));
    TransAni->addChild(grouptrans);
    return TransAni;
}

TransformAnimationSP
SceneObjetFactory::createTorusAnimation(const GeometryCoreSP &torusCore, const std::vector<ShapeSP> &torusShape, int j,
                                        float i) {
    TransformationSP toursTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(i, i, i),
                                                       glm::vec3(1, 0, 0), 1.f);
    torusShape[j]->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getWhite())->addCore(
            torusCore);

    toursTrans->addChild(torusShape[j]);
    auto TransAni = TransformAnimation::create();
    glm::vec3 rotatex;
    if (j & 2 == 0) {
        rotatex = glm::vec3(1, 0, 0);
    } else {
        rotatex = glm::vec3(0, 1, 1);
    }
    TransAni->setUpdateFunc(
            [toursTrans, j, rotatex](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
                toursTrans->rotate(j, rotatex);
            }
    );
    TransAni->addChild(toursTrans);
    viewer->addAnimation(TransAni);
    return TransAni;
}

const TransformationSP SceneObjetFactory::getStar() {
    auto torusCore = torusFactory.createTorus(0, .9, 3, 50);
    TransformationSP toursTrans = Transformation::create();
    auto torusShape = Shape::create();
    torusShape->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getGold())->addCore(torusCore);
    toursTrans->addChild(torusShape);
    toursTrans->rotate(90, glm::vec3(1, 0, 0));
    toursTrans->scale(glm::vec3(.1f, .1f, .1f));
    return toursTrans;
}

const TransformAnimationSP SceneObjetFactory::getAnimatedStern() {
    auto sternTrans = getStar();
    auto TransAni = TransformAnimation::create();
    TransAni->setUpdateFunc(
            [sternTrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
                sternTrans->rotate(1, glm::vec3(0, 0, 1));
            }
    );
    TransAni->addChild(sternTrans);
    viewer->addAnimation(TransAni);
    return TransAni;

}

void
SceneObjetFactory::createShapesWithDifferentColors(int i, const GeometryCoreSP &torusCore,
                                                   std::vector<ShapeSP> &torusShape) const {
    torusShape.push_back(Shape::create());
    torusShape.at(i)->addCore(ShaderFactory::getInstance()->getPhong(false));
    if (i % 2 == 0) {
        torusShape.at(i)->addCore(matFactory->getBlack());
    } else {
        torusShape.at(i)->addCore(matFactory->getRed());
    }
    torusShape.at(i)->addCore(torusCore);
}


const std::vector<TransformationSP> &SceneObjetFactory::getTargets() {
    if (zielscheiben.size() == 0) {
        for (int i = 0; i < 10; i++) {
            TransformationSP y = createToriWithAnimation();
            y = createRandompos(y);
            y->scale(glm::vec3(1, 1, .4));
            y->setVisible(false);
            if (i == 0) {
                y->setVisible(true);
            }

            zielscheiben.push_back(y);
        }
    }
    return zielscheiben;
}


TransformationSP &SceneObjetFactory::createRandompos(TransformationSP &trans) {
    float x = rand() % 7 -7;
    float y = rand() % 10+2;
    float z = rand() % 7-7;
    trans->translate(glm::vec3(x, y, z));
    return trans;
}


const TransformationSP &SceneObjetFactory::getFloor() {
    if (floorTrans == nullptr) {
        // auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
        auto floorCore = torusFactory.createCuboid(glm::vec3(50, .1, 50));
        auto floor = Shape::create();
        floor->addCore(shaderFactory->getPhongBumb())->addCore(matFactory->getStadt())->addCore(
                        textureFactory->getStadt())
                ->addCore(floorCore);
        floorTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
                                          0.f);
        floorTrans->addChild(floor);
    }
    return floorTrans;
}


const TransformationSP &SceneObjetFactory::getVideoFloor() {
    if (videofloorTrans == nullptr) {
        // auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
        auto floorCore = torusFactory.createCuboid(glm::vec3(50, .1, 50));
        auto floor = Shape::create();
        floor->addCore(shaderFactory->getPhongBumb())->addCore(matFactory->getStadt())->addCore(
                        textureFactory->getStadt())
                ->addCore(floorCore);
        videofloorTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
                                          0.f);
        videofloorTrans->addChild(floor);
    }
    return videofloorTrans;
}



const TransformationSP &SceneObjetFactory::getCamObject() {
    if (camObject == nullptr) {
        auto flugzeugCore = torusFactory.createModelFromOBJFile("../scg3/models/jet.obj");
        auto flugzeugShape = Shape::create();
        flugzeugShape->addCore(shaderFactory->getPhong(true))->addCore(matFactory->getWhite())->addCore(
                        textureFactory->getBrick())
                ->addCore(flugzeugCore);
        camObject = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(0.05, 0.05, 0.05),
                                         glm::vec3(0.f, 1.f, 0.f), 180.);
        camObject->addChild(flugzeugShape);
    }
    return camObject;
}




const TransformationSP &SceneObjetFactory::getTower() {

    if (turmTrans == nullptr) {
        GroupSP group = Group::create();

        auto turmCore = torusFactory.createConicalFrustum(.3, .3, 1, 10, 10, true);
        auto turmShape = Shape::create();
        turmShape->addCore(shaderFactory
                                   ->getPhongBumb())
                ->addCore(matFactory
                                  ->getWhite())->addCore(
                        textureFactory
                                ->getMauer())->addCore(turmCore);

        auto bodyTrans = Transformation::create();
        bodyTrans->addChild(turmShape);
        group->addChild(bodyTrans);


        TransformationSP kugelTrans = Transformation::create();
        auto kugelCore = torusFactory.createSphere(2, 100, 100);
        ShapeSP kugelShape = Shape::create();
        kugelShape->addCore(shaderFactory->getPhongBumb())->addCore(matFactory->getBlack())->addCore(
                textureFactory->getMauer())->addCore(kugelCore);
        kugelTrans->addChild(kugelShape);
        kugelTrans->translate(glm::vec3(0, 0, -.5555));
        kugelTrans->scale(glm::vec3(.1, .1, .1));

        group->addChild(kugelTrans);

        TransformationSP kugelTrans2 = Transformation::create();
        ShapeSP kugelShape2 = Shape::create();
        kugelShape2->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getBlack())->addCore(
                textureFactory->getMauer())->addCore(kugelCore);
        kugelTrans2->addChild(kugelShape2);
        kugelTrans2->translate(glm::vec3(0, 0, -.8));
        kugelTrans2->scale(glm::vec3(.04, .04, .04));

        LightFactory *lightFactory = LightFactory::getInstance();


        group->addChild(lightFactory->getLinks());
        group->addChild(lightFactory->getRechts());
        lightFactory->getRechts()->addChild(kugelTrans2);
        lightFactory->getLinks()->addChild(kugelTrans2);

        turmTrans = Transformation::create();
        turmTrans->addChild(group);
        turmTrans->rotate(90.f, glm::vec3(1, 0, 0));
        turmTrans->translate(glm::vec3(0, 0, -.1));
        turmTrans->scale(glm::vec3(.1, .1, .1));
    }
    return turmTrans;


}


SceneObjetFactory *SceneObjetFactory::getInstance(ViewerSP viewerSp) {
    if (instance == nullptr) {
        instance = new SceneObjetFactory(viewerSp);
    }
    return instance;
}


SceneObjetFactory::SceneObjetFactory(ViewerSP viewer) {
    this->viewer = viewer;
    shaderFactory = ShaderFactory::getInstance();
    matFactory = MatFactory::getInstance();
    textureFactory = TexturFactory::getInstance();
}


const TransformAnimationSP SceneObjetFactory::createJetGroup() {


    auto group = Group::create();
    group->addChild(getFlugzeugAnimated());
    auto groupTrans = TransformationExt::create();
    auto TransAni = TransformAnimation::create();

    groupTrans->translate(glm::vec3(2.0, 1.2, -3.));
    groupTrans->rotate(180.0,glm::vec3(0, 0.1, 0));

    TransAni->setUpdateFunc(
            [groupTrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
                std::cout<<"x :"<<groupTrans->getPosition().x<<std::endl;
                groupTrans->translate(glm::vec3(-.01,.0,.0));
                groupTrans->rotate(20.,glm::vec3(-.01, .0,0.0));
                if(groupTrans->getPosition().x<-2.){
                    groupTrans->translate(glm::vec3(2.0, 0.0, 0.0));

                }
            });

    groupTrans->addChild(group);

    TransAni->addChild(groupTrans);
    viewer->addAnimation(TransAni);
    return TransAni;
}



const TransformAnimationSP SceneObjetFactory::createJetGroup2() {


    auto group2 = Group::create();
    group2->addChild(getFlugzeugAnimated());
    auto groupTrans2 = TransformationExt::create();
    auto TransAni2 = TransformAnimation::create();

    groupTrans2->translate(glm::vec3(0., 1., -5.));
    groupTrans2->rotate(-100.0,glm::vec3(0.1, 0.1, 0));

    //setzt die Flugzeuggruppe die vom Himmel stürzt
    TransAni2->setUpdateFunc(
            [groupTrans2](
                    TransformAnimation *anim1, double currTime, double diffTime, double totalTime) {
                 groupTrans2->translate(glm::vec3(-.01, .0,0.0));

                if(groupTrans2->getPosition().x<-4.0){
                    groupTrans2->translate(glm::vec3(2.0, 0.0, 0.0));
                }

            });

    groupTrans2->addChild(group2);
    TransAni2->addChild(groupTrans2);
    viewer->addAnimation(TransAni2);
    return TransAni2;
}

const TransformAnimationSP SceneObjetFactory::getFlugzeugAnimated() {

    if(jetAniGroup== nullptr) {
        auto flug1 = getJet();
        auto flug2 = getJet();
        auto flug3 = getJet();

        auto flug4 = getJet();
        auto flug5 = getJet();
        auto flug6 = getJet();

        //setzt die Position der Flugzeuge
        flug1->translate(glm::vec3(0, 0, 0));
        flug2->translate(glm::vec3(5, -.3, -3));
        flug3->translate(glm::vec3(-5, -.3, -3));

        flug4->translate(glm::vec3(-3, -.3, 4));
        flug5->translate(glm::vec3(3, -.3, 4));


        jetAniGroup = TransformAnimation::create();
        //Lässt das innere Flugzeug um die eigne achse drehen
        jetAniGroup->setUpdateFunc(
                [flug1, flug2, flug3](
                        TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
                    flug1->rotate(10, glm::vec3(0.0, 0.0, 0.1));

                }
        );

        jetAniGroup->addChild(flug1);
        jetAniGroup->addChild(flug2);
        jetAniGroup->addChild(flug3);
        jetAniGroup->addChild(flug4);
        jetAniGroup->addChild(flug5);
        viewer->addAnimation(jetAniGroup);

    }
    return jetAniGroup;
}

TransformationSP &SceneObjetFactory::createBullet() {
    if (bulletTrans == nullptr) {
        GeometryCoreFactory geometryFactory;
        auto bulletCore = geometryFactory.createSphere(0.005, 10, 10);
        auto bullet = Shape::create();
        bullet->addCore(ShaderFactory::getInstance()->getPhong(true))
                ->addCore(MatFactory::getInstance()->getRed())
                ->addCore(bulletCore);
        bulletTrans = Transformation::create();

        bulletTrans->addChild(bullet);
        bulletTrans->setVisible(false);
    }
    return bulletTrans;
}




