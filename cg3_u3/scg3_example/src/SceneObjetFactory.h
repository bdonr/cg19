//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_SCENEOBJETFACTORY_H
#define CG3_U3_SCENEOBJETFACTORY_H

#include "scg3.h"
#include "EnvoirementController.h"
#include "MatFactory.h"
#include "TexturFactory.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"
#include "TorusFactory.h"
#include "TransfomationExt.h"

using namespace scg;

class SceneObjetFactory {

public:

private:
    static SceneObjetFactory *instance;

    SceneObjetFactory(ViewerSP viewer);

    TransformationSP camObject;
    TransformationSP bulletTrans;
    TransformationSP floorTrans;
    TransformationSP videofloorTrans;

    TransformationSP turmTrans;

    ShapeSP skybox;
    ShapeSP videoSkybox;

    GroupSP group;
    std::vector<TransformationSP> transformations;
    std::vector<TransformationSP> zielscheiben;
    ShaderFactory *shaderFactory;
    MatFactory *matFactory;
    TexturFactory *textureFactory;
    ViewerSP viewer;
    TorusFactory torusFactory;

public:

    const LightSP &getLinks();

    const LightSP &getRechts();

    const TransformationExtSP getFlugzeug();

    const ShapeSP getHimmel();

    const ShapeSP getVideoHimmel();


    const TransformationSP &getFloor();

    const TransformationSP &getVideoFloor();

    const TransformationSP &getCamObject();

    const TransformationSP getKugel();

    const LightSP &getVideoSonne();

    const TransformAnimationSP createFlugzeugGruppe();

    const TransformationSP &getTurm();

    const LightSP &getSonne();


    const std::vector<TransformationSP> &getZielscheiben();

    static SceneObjetFactory *getInstance(ViewerSP viewer);

    const TransformationSP getStern();

    TransformationSP &createBullet();

private:

    const TransformAnimationSP createTorrusseTrans();

    const TransformationExtSP
    createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);


    const ShapeSP getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
                           const GeometryCoreSP &core);


    TransformationSP &createRandompos(TransformationSP &trans);


    void
    createShapesWithDifferentColors(int i, const GeometryCoreSP &torusCore, std::vector<ShapeSP> &torusShape) const;

    const TransformAnimationSP getAnimatedStern();

    TransformAnimationSP
    createTorusAnimation(const GeometryCoreSP &torusCore, const std::vector<ShapeSP> &torusShape, int j, float i);


    const TransformAnimationSP getFlugzeugAnimated();


};


#endif //CG3_U3_SCENEOBJETFACTORY_H
