//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"

TransformationSP SceneObjetFactory::floorTrans;
TransformationSP SceneObjetFactory::himmelTrans;
TransformationSP SceneObjetFactory::jetTrans;
TransformationSP SceneObjetFactory::camObject;
GroupSP SceneObjetFactory::torus;
LightSP SceneObjetFactory::sonne;
GroupSP SceneObjetFactory::group;

 std::vector<TransformationSP> SceneObjetFactory::transformations;

std::vector<TransformationSP> SceneObjetFactory::torusseTrans;

GeometryCoreFactory SceneObjetFactory::geometryFactory;

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

const TransformationSP
SceneObjetFactory::createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate,
                                        float degree) {
    auto trans = Transformation::create();
    trans->translate(translate);
    trans->rotate(degree, rotate);
    trans->scale(scale);
    return trans;
}

const std::vector<TransformationSP> &
SceneObjetFactory::getSterne(int menge, const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate,
                                float degree) {
    if(transformations.size()==0) {
        for (int i = 0; i < menge; i++) {
            std::cout <<i<< "blka" << std::endl;
            transformations.push_back(getKugel());
        }
    }
    return transformations;
}

const TransformationSP &SceneObjetFactory::getFlugzeug() {
    if (jetTrans == nullptr) {
        auto jetcore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
        ShapeSP jet = getShape(ShaderFactory::getPhong(true), MatFactory::getWhite(), TexturHelper::getBrick(),
                               jetcore);
        jetTrans = createTransformation(glm::vec3(4.3f, 0.2f, 0.3f), glm::vec3(0.05, 0.05, 0.05),
                                        glm::vec3(0.f, 1.f, 0.f), -90.f);
        jetTrans->addChild(jet);
    }
    return jetTrans;
}


const TransformationSP &SceneObjetFactory::getHimmel() {
    if (himmelTrans == nullptr) {
        auto himmelCore = geometryFactory.createSphere(20, 101, 110);
        ShapeSP himmel = getShape(ShaderFactory::getPhong(true), MatFactory::getTag(), TexturHelper::getHimmel(),
                                  himmelCore);
        himmelTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(1, 0, 0),
                                           4.f);
        himmelTrans->addChild(himmel);
    }
    return himmelTrans;
}
const GroupSP& SceneObjetFactory::getTorus() {
    if (torus == nullptr) {
        torus = Group::create();
        torus->addCore(ShaderFactory::getPhong(false))->addCore(MatFactory::getWhite());
        auto torusCore = geometryFactory.createTorus(3,.7,3,3);
        ShapeSP torusShape = Shape::create();
        torusShape->addCore(torusCore);
        createTorrusseTrans(torusShape);
    }
    return torus;
}

void SceneObjetFactory::createTorrusseTrans(const ShapeSP &torusShape) {
    if(torusseTrans.size()==0) {
        int j = 0;
        for (float i = 1.0f; i >= 0.0; i = i - 0.3) {
            torusseTrans.push_back(
                    createTransformation(glm::vec3(0, 0, 0), glm::vec3(1-i, 1-i, 1-i), glm::vec3(1, 0, 0),0.f));
            torusseTrans.at(j)->addChild(torusShape);
            torus->addChild(torusseTrans.at(j));
            j++;
        }
    }
}


const TransformationSP &SceneObjetFactory::getFloor() {
    if (floorTrans == nullptr) {
        auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/table-mountain.obj");
        auto floor = Shape::create();
        floor->addCore(ShaderFactory::getPhong(true))->addCore(MatFactory::getWhite())->addCore(
                        TexturHelper::getStadt())
                ->addCore(floorCore);
        floorTrans = createTransformation(glm::vec3(0, 0, 0), glm::vec3(.012, .012, .012), glm::vec3(1, 0, 0),
                                          0.f);
        floorTrans->addChild(floor);
    }
    return floorTrans;
}

const std::vector<TransformationSP> &SceneObjetFactory::getFlugzeuge() const {

}

const TransformationSP &SceneObjetFactory::getCamObject() {
    if (camObject == nullptr) {
        auto floorCore = geometryFactory.createModelFromOBJFile("../scg3/models/jet.obj");
        auto floor = Shape::create();
        floor->addCore(ShaderFactory::getPhong(true))->addCore(MatFactory::getWhite())->addCore(
                        TexturHelper::getBrick())
                ->addCore(floorCore);
        camObject = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(0.05, 0.05, 0.05),
                                         glm::vec3(0.f, 1.f, 0.f), 180.);
        camObject->addChild(floor);
    }
    return camObject;
}

const TransformationSP SceneObjetFactory::getKugel() {
    auto x = geometryFactory.createSphere(.1f, 100, 100);
    auto x1 = Shape::create();
    x1->addCore(ShaderFactory::getPhong(false))->addCore(MatFactory::getWhite())
            ->addCore(x);
    TransformationSP trans = createTransformation(glm::vec3(0.f, -0.1f, -0.3f), glm::vec3(1, 1, 1),glm::vec3(0.f, 1.f, 0.f), 180.);
    trans->addChild(x1);
    return trans;
}

const LightSP &SceneObjetFactory::getSonne() {
    if (sonne == nullptr) {
        sonne = Light::create();
        sonne->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))->setAmbient(
                        glm::vec4(.4, .4, .4, 1))->setSpot(glm::normalize(glm::vec4(1, 1, 1, 1)), 180.f, .1f)
                ->setPosition(glm::vec4(0.f, 36.f, 0, 1.f))
                ->init();
    }
    return sonne;
}

const GroupSP &SceneObjetFactory::getGroup() {
    if (group == nullptr) {
        group = Group::create();
        group->addCore(ShaderFactory::getPhong(true))->addCore(MatFactory::getStadt())->addCore(
                TexturHelper::getBrick());
getSterne(100, glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.1, 0.1, 0.1),
                                                           glm::vec3(0.1, 0.1, 0.1), 0);

            for (int i = 0; i < transformations.size()-1; i++) {
                group->addChild(transformations.at(i));

        }

    }
    return group;
}


