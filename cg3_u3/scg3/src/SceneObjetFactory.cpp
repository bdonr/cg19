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
const TransformationSP
SceneObjetFactory::createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate,
                                        float degree) {
    auto trans = Transformation::create();
    trans->translate(translate);
    trans->rotate(degree, rotate);
    trans->scale(scale);
    return trans;
}

/**
 * Create a Group of Transformations which represent some Stars
 * @param menge
 * @param translate
 * @param scale
 * @param rotate
 * @param degree
 * @return
 */
const std::vector<TransformationSP> &
SceneObjetFactory::getSterne(int menge, const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate,
                             float degree) {
    if (transformations.size() == 0) {
        for (int i = 0; i < menge; i++) {
            transformations.push_back(getKugel());
        }
    }
    return transformations;
}


/**
 * Create a jet
 * @return TransformationSP
 */
const TransformationSP &SceneObjetFactory::getFlugzeug() {
    if (jetTrans == nullptr) {
        auto jetcore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
        ShapeSP jet = getShape(shaderFactory->getPhong(true), matFactory->getWhite(), textureFactory->getBrick(),
                               jetcore);
        jetTrans = createTransformation(glm::vec3(4.3f, 0.2f, 0.3f), glm::vec3(0.05, 0.05, 0.05),
                                        glm::vec3(0.f, 1.f, 0.f), -90.f);
        jetTrans->addChild(jet);
    }
    return jetTrans;
}

/**
 * Create a Heavensphere
 * @return TransformationSP
 */
const TransformationSP &SceneObjetFactory::getHimmel() {
    if (himmelTrans == nullptr) {
        auto himmelCore = geometryFactory.createSphere(30, 101, 110);
        ShapeSP himmel = getShape(shaderFactory->getPhongreverse(true), matFactory->getTag(),
                                  textureFactory->getHimmel(),
                                  himmelCore);
        himmelTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
                                           20.f);
        himmelTrans->addChild(himmel);
    }
    return himmelTrans;
}


/**
 * Creates a Group of Torus with its  TransformationSP
 * @param TransformationSP
 */
const TransformAnimationSP SceneObjetFactory::createTorrusseTrans() {
    auto torussGroup = Group::create();

    //unterschiedliche formen 3 eck bishin zu vieleck
    auto torusCore = geometryFactory.createTorus(0.5, .1, 50, 40);
    std::vector<ShapeSP> torusShape;
    int j = 0;
    for (float i = 1.0f; i >= 0.0; i = i - 0.1) {
        createShapesWithDifferentColors(j, torusCore, torusShape);
        TransformAnimationSP TransAni = createTorusAnimation(torusCore, torusShape, j, i);
        if (j == 9) {
            std::cout << "stern" << std::endl;
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
    auto torusCore = geometryFactory.createTorus(0, .9, 3, 50);
    TransformationSP toursTrans = Transformation::create();
    auto torusShape = Shape::create();
    torusShape->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getGold())->addCore(torusCore);
    toursTrans->addChild(torusShape);
    toursTrans->rotate(90,glm::vec3(1,0,0));
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
    float x = rand() % 15 -15;
    float y = rand() % 15 ;
    float z = rand() % 15 -15;
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
        auto floorCore = geometryFactory.createCuboid(glm::vec3(50, .1, 50));
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

/**
 * Creates an object which will be used as a Object binded to the cam
 * @return
 */
const TransformationSP &SceneObjetFactory::getCamObject() {
    if (camObject == nullptr) {
        auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
        auto floor = Shape::create();
        floor->addCore(shaderFactory->getPhong(true))->addCore(matFactory->getWhite())->addCore(
                        textureFactory->getBrick())
                ->addCore(floorCore);
        camObject = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(0.05, 0.05, 0.05),
                                         glm::vec3(0.f, 1.f, 0.f), 180.);
        camObject->addChild(floor);
    }
    return camObject;
}

/**
 * Creats a Sphere
 * @return
 */
const TransformationSP SceneObjetFactory::getKugel() {
    auto x = geometryFactory.createSphere(.1f, 100, 100);
    auto x1 = Shape::create();
    x1->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getWhite())
            ->addCore(x);
    TransformationSP trans = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(1, 1, 1),
                                                  glm::vec3(0.f, 1.f, 0.f), 180.);
    trans->addChild(x1);

    trans->scale(glm::vec3(.1,.1,.1));
    return trans;
}

/**
 * Creates a Lightposition
 * @return
 */
const LightSP &SceneObjetFactory::getSonne() {
    if (sonne == nullptr) {
        sonne = Light::create();
        sonne->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))->setAmbient(
                        glm::vec4(.1, .1, .1, 1))->setSpot(glm::vec3(0, 1, 1), 100, 1)
                ->setPosition(glm::vec4(0.f, 20.f, 0, 1.f))
                ->init();
    }
    return sonne;
}

/**
 * Creates two Lights in a group
 * @return
 */
const GroupSP &SceneObjetFactory::getFrontLichter() {
    if (frontlichter == nullptr) {
        frontlichter->addChild(getRechts());
        frontlichter->addChild(getLinks());
    }
    return frontlichter;
}


const GroupSP &SceneObjetFactory::getGroup() {
    if (group == nullptr) {
        group = Group::create();
        group->addCore(shaderFactory->getPhong(true))->addCore(matFactory->getStadt())->addCore(
                textureFactory->getBrick());
        getSterne(100, glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.1, 0.1, 0.1),
                  glm::vec3(0.1, 0.1, 0.1), 0);

        for (int i = 0; i < transformations.size() - 1; i++) {
            group->addChild(transformations.at(i));

        }

    }
    return group;
}

const LightSP &SceneObjetFactory::getLinks() {

    if (links == nullptr) {
        links = Light::create();

        links->setSpecular(glm::vec4(1.f, 0.f, .0f, 1.f))->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))->setAmbient(
                        glm::vec4(1, .4, .4, 1))
                ->setPosition(glm::vec4(0,0,-1,1))
                ->init();
    }
    LightPositionSP x = LightPosition::create(links);

    return links;
}

const LightSP &SceneObjetFactory::getRechts() {
    if (rechts == nullptr) {
        rechts = Light::create();
        glm::vec4 k = glm::vec4(0,0,1,1);
        rechts->setSpecular(glm::vec4(0.f, .8f, .0f, 1.f))->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))->setAmbient(
                        glm::vec4(.4, 4, .4, 1))

                ->setPosition(k)
                ->init();
    }
    return rechts;
}

/**
 * Creates a Tower with a 3D brick wall
 * @return
 */
const TransformationSP &SceneObjetFactory::getTurm() {

    if (turmTrans == nullptr) {
        GroupSP group = Group::create();

        auto turmCore = geometryFactory.createConicalFrustum(.3, .3, 1, 10, 10, true);
        auto turmShape = Shape::create();
        turmShape->addCore(shaderFactory->getPhongBumb())->addCore(matFactory->getWhite())->addCore(
                textureFactory->getMauer())->addCore(turmCore);

        auto bodyTrans = Transformation::create();
        bodyTrans->addChild(turmShape);
        group->addChild(bodyTrans);


        TransformationSP kugelTrans = Transformation::create();
        auto kugelCore = geometryFactory.createSphere(2, 100, 100);
        ShapeSP kugelShape = Shape::create();
        kugelShape->addCore(shaderFactory->getPhongBumb())->addCore(matFactory->getBlack())->addCore(
                textureFactory->getMauer())->addCore(kugelCore);
        kugelTrans->addChild(kugelShape);
        kugelTrans->translate(glm::vec3(0,0,-.5555));
        kugelTrans->scale(glm::vec3(.1,.1,.1));

        group->addChild(kugelTrans);

        TransformationSP kugelTrans2 = Transformation::create();
        ShapeSP kugelShape2 = Shape::create();
        kugelShape2->addCore(shaderFactory->getPhong(false))->addCore(matFactory->getBlack())->addCore(
                textureFactory->getMauer())->addCore(kugelCore);
        kugelTrans2->addChild(kugelShape2);
        kugelTrans2->translate(glm::vec3(0,0,-.8));
        kugelTrans2->scale(glm::vec3(.04,.04,.04));

        group->addChild(getLinks());
        group->addChild(getRechts());
        getRechts()->addChild(kugelTrans2);
        getLinks()->addChild(kugelTrans2);

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
