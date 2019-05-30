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

SceneObjetFactory* SceneObjetFactory::instance;


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
        ShapeSP himmel = getShape(shaderFactory->getPhongreverse(true), matFactory->getTag(), textureFactory->getHimmel(),
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
const TransformationSP SceneObjetFactory::createTorrusseTrans() {
    auto torussGroup = Group::create();
    auto torusCore = geometryFactory.createTorus(0.5, .1, rand()%13+3,40);
    ShapeSP torusShape = Shape::create();
    torusShape->addCore(ShaderFactory::getInstance()->getPhong(false))->addCore(matFactory->getWhite());
    torusShape->addCore(torusCore);
    for (float i = 1.0f; i >= 0.0; i = i - 0.3) {
        TransformationSP toursTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(i, i, i),
                                                           glm::vec3(1, 0, 0), 0.f);
        toursTrans->addChild(torusShape);
        torussGroup->addChild(toursTrans);
    }
    TransformationSP grouptrans = Transformation::create();
    grouptrans->addChild(torussGroup);
    return grouptrans;
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
            y->scale(glm::vec3(.4, .4, .4));
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
    float x = rand() % 10 + 0.1;
    float y = rand() % 10 + 0.1;
    float z = rand() % 10 + 0.1;
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
        auto floorCore = geometryFactory.createCuboid(glm::vec3(50,.1,50));
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
                ->setPosition(glm::vec4(0.f, 36.f, 0, 1.f))
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
        glm::vec4 k = glm::vec4(getCamObject()->getMatrix()[3][0],getCamObject()->getMatrix()[3][0],getCamObject()->getMatrix()[3][0],1.);
        links->setSpecular(glm::vec4(.8f, .8f, .8f, 1.f))->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))->setAmbient(
                        glm::vec4(.4, .4, .4, 1))
                ->setPosition(k)
                ->init();
        links->setSpot(glm::vec3(getCamObject()->getMatrix()[3][0], getCamObject()->getMatrix()[3][0], getCamObject()->getMatrix()[3][0]+.2), 10, 10);
        links->addChild(getFloor());
    }
    return links;
}

const LightSP &SceneObjetFactory::getRechts() {
    if (rechts == nullptr) {
        rechts = Light::create();
        glm::vec4 k = glm::vec4(getCamObject()->getMatrix()[3][0],getCamObject()->getMatrix()[3][0],getCamObject()->getMatrix()[3][0],1.);
        rechts->setSpecular(glm::vec4(.8f, .8f, .8f, 1.f))->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))->setAmbient(
                        glm::vec4(.4, .4, .4, 1))

                ->setPosition(k)
                ->init();
        rechts->setSpot(glm::vec3(getCamObject()->getMatrix()[3][0], getCamObject()->getMatrix()[3][0], getCamObject()->getMatrix()[3][0]+.2), 10, 10);
        rechts->addChild(getFloor());
    }
    return rechts;
}

/**
 * Creates a Tower with a 3D brick wall
 * @return
 */
const TransformationSP& SceneObjetFactory::getTurm(){
    if(turmTrans == nullptr) {
        auto turmCore = geometryFactory.createConicalFrustum(.3,.3, 1, 10, 10, true);
        auto turmShape = Shape::create();
        turmShape->addCore(shaderFactory->getPhongBumb())->addCore(matFactory->getWhite())->addCore(
                textureFactory->getMauer())->addCore(turmCore);
        turmTrans = Transformation::create();

        turmTrans->rotate(90.f,glm::vec3(1,0,0));
        turmTrans->translate(glm::vec3(0,0,-.1));
        turmTrans->scale(glm::vec3(.1,.1,.1));
        turmTrans->addChild(turmShape);
    }
    return turmTrans;

}

/**
 *
 * @return SceneObjetFactory *
 */
 SceneObjetFactory * SceneObjetFactory::getInstance(){
    if(instance==nullptr){
        instance=new SceneObjetFactory();
    }
     return instance;
}

/**
 * Constructor
 */
SceneObjetFactory::SceneObjetFactory(){
    shaderFactory= ShaderFactory::getInstance();
    matFactory = MatFactory::getInstance();
    textureFactory = TexturFactory::getInstance();
}
