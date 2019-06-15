//
// Created by don on 21.05.19.
//

/**
 * This class Creates SubTrees of Transformations
 * It creates Transformations with its Groups Shapes and Cores
 *  Feel free to add more Transformations as you need .
 *  They can be easily added to your scene.
 *  Just by adding them whith scene->addChild(your-TransFormation);
 */
#include <iostream>
#include "SceneObjetFactory.h"
#include "LightFactory.h"

SceneObjetFactory *SceneObjetFactory::instance;


/**
 * Create Shape with some properties
 * @param shade
 * @param mat
 * @param textur
 * @param core
 * @return ShapeSP
 */
const ShapeSP
SceneObjetFactory::getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
                            const GeometryCoreSP &core) {
    auto nachtHimmel = Shape::create();
    nachtHimmel->addCore(mat)
            ->addCore(shade)
            ->addCore(textur)
            ->addCore(core);
    return nachtHimmel;
}


/**
 * Create a Basic TransformationSP
 * @param translate
 * @param scale
 * @param rotate
 * @param degree
 * @return TransformationSP
 */
const TransformationExtSP SceneObjetFactory::createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate,
                                        float degree) {
    auto trans = TransformationExt::create();
    trans->translate(translate);
    trans->rotate(degree, rotate);
    trans->scale(scale);
    return trans;
}


/**
 * Create a jet
 * @return TransformationSP
 */
const TransformationExtSP SceneObjetFactory::getFlugzeug() {

    auto jetcore = torusFactory.createModelFromOBJFile("../scg3/models/jet.obj");
    ShapeSP jet = getShape(shaderFactory->getPhong(true), matFactory->getWhite(), textureFactory->getBrick(),
                           jetcore);

    TransformationExtSP jetTrans = createTransformation(glm::vec3(4.3f, 0.2f, 0.3f), glm::vec3(0.05, 0.05, 0.05),
                                         glm::vec3(0.f, 1.f, 0.f), -90.f);


    jetTrans->addChild(jet);

    return jetTrans;
}

/**
 * Create a Heavensphere
 * @return TransformationSP
 */
const ShapeSP SceneObjetFactory::getHimmel() {

    skybox = Shape::create();

    auto skyboxCore = torusFactory.createCube(50.f);
    skybox->addCore(shaderFactory->getSkyBox())
            ->addCore(textureFactory->getHimmel())
            ->addCore(skyboxCore);




    /**
    auto himmelCore = geometryFactory.createSphere(30, 101, 110);
    ShapeSP himmel = getShape(shaderFactory->getPhongreverse(true), matFactory->getTag(),
                              textureFactory->getHimmel(),
                              himmelCore);
    himmelTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
                                       20.f);
    himmelTrans->addChild(himmel);
}**/

    return skybox;
}

const ShapeSP SceneObjetFactory::getVideoHimmel() {

    videoSkybox = Shape::create();

    auto skyboxCore = torusFactory.createCube(50.f);
    videoSkybox->addCore(shaderFactory->getSkyBox())
            ->addCore(textureFactory->getHimmel())
            ->addCore(skyboxCore);
    return videoSkybox;
}

/**
 * Creates a Group of Torus with its  TransformationSP
 * @param TransformationSP
 */
const TransformAnimationSP SceneObjetFactory::createTorrusseTrans() {
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

const TransformationSP SceneObjetFactory::getStern() {
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
    auto sternTrans = getStern();
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

/**
 * Creates ten Groups of Torus
 * @return std::vector<TransformationSP>&
 */
const std::vector<TransformationSP> &SceneObjetFactory::getZielscheiben() {
    if (zielscheiben.size() == 0) {
        for (int i = 0; i < 10; i++) {
            TransformationSP y = createTorrusseTrans();
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

/**
 * Translates a given TransformationSP to a RandomPosition
 * @param trans
 * @return TransformationSP
 */
TransformationSP &SceneObjetFactory::createRandompos(TransformationSP &trans) {
    float x = rand() % 1;
    float y = rand() % 1;
    float z = rand() % 1;
    trans->translate(glm::vec3(x, y, z));
    return trans;
}

/**
 * Creates a Floor Transformation
 * @return TransformationSP
 */
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


/**
 * Creates an object which will be used as a Object binded to the cam
 * @return
 */
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

/**
 * Creats a Sphere
 * @return
 */
const TransformationSP SceneObjetFactory::getKugel() {
    auto x = torusFactory.createSphere(.1f, 100, 100);
    auto x1 = Shape::create();
    x1->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getWhite())
            ->addCore(x);
    TransformationSP trans = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(1, 1, 1),
                                                  glm::vec3(0.f, 1.f, 0.f), 180.);
    trans->addChild(x1);

    trans->scale(glm::vec3(.1, .1, .1));
    return trans;
}

/**
 * Creates a Lightposition
 * @return
 */

/**
 * Creates a Tower with a 3D brick wall
 * @return
 */
const TransformationSP &SceneObjetFactory::getTurm() {

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

/**
 *
 * @return SceneObjetFactory *
 */
SceneObjetFactory *SceneObjetFactory::getInstance(ViewerSP viewerSp) {
    if (instance == nullptr) {
        instance = new SceneObjetFactory(viewerSp);
    }
    return instance;
}

/**
 * Constructor
 */
SceneObjetFactory::SceneObjetFactory(ViewerSP viewer) {
    this->viewer = viewer;
    shaderFactory = ShaderFactory::getInstance();
    matFactory = MatFactory::getInstance();
    textureFactory = TexturFactory::getInstance();
}

const TransformAnimationSP SceneObjetFactory::createFlugzeugGruppe() {


    auto group = Group::create();
    group->addChild(getFlugzeugAnimated());
    auto groupTrans = Transformation::create();
    auto TransAni = TransformAnimation::create();
    TransAni->setUpdateFunc(
            [groupTrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
               // groupTrans->translate(glm::vec3(-.001, 0, 0));
            });

    groupTrans->addChild(group);
    groupTrans->translate(glm::vec3(0, 1, 0));
    groupTrans->rotate(90.0,glm::vec3(0, 0.1, 0));
    TransAni->addChild(groupTrans);
    viewer->addAnimation(TransAni);
    return TransAni;
}

const TransformAnimationSP SceneObjetFactory::getFlugzeugAnimated() {
    auto flug1 = getFlugzeug();
    auto flug2 = getFlugzeug();
    auto flug3 = getFlugzeug();

    auto flug4 = getFlugzeug();
    auto flug5 = getFlugzeug();
    auto flug6 = getFlugzeug();

    flug1->translate(glm::vec3(0, 0, 0));
    flug2->translate(glm::vec3(5, -.3, -3));
    flug3->translate(glm::vec3(-5, -.3, -3));

    flug4->translate(glm::vec3(-3, -3, 4));
    flug5->translate(glm::vec3(3, -.3, 4));
    //flug6->translate(glm::vec3(-5, -.3, -3));

    auto TransAni = TransformAnimation::create();

    TransAni->setUpdateFunc(
            [flug1, flug2, flug3](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {


            }
    );

    TransAni->addChild(flug1);
    TransAni->addChild(flug2);
    TransAni->addChild(flug3);
    TransAni->addChild(flug4);
    TransAni->addChild(flug5);
    viewer->addAnimation(TransAni);
    return TransAni;

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




