//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_SCENEOBJETFACTORY_H
#define CG3_U3_SCENEOBJETFACTORY_H

#include "../scg3/scg3.h"
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturFactory.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"

using namespace scg;

class SceneObjetFactory {

public:

private:
    static SceneObjetFactory *instance;

    SceneObjetFactory(ViewerSP viewer);

    TransformationSP himmelTrans;
    TransformationSP jetTrans;
    TransformationSP camObject;
    TransformationSP floorTrans;
    TransformationSP turmTrans;
    LightSP sonne;
    LightSP links;
    LightSP rechts;
    GroupSP frontlichter;
    GeometryCoreFactory geometryFactory;
    GroupSP group;
    std::vector<TransformationSP> transformations;
    std::vector<TransformationSP> zielscheiben;
    ShaderFactory *shaderFactory;
    MatFactory *matFactory;
    TexturFactory *textureFactory;
    ViewerSP viewer;
public:

    const LightSP &getLinks();

    const LightSP &getRechts();

    const TransformationSP &getFlugzeug();

    const TransformationSP &getHimmel();

    const TransformationSP &getFloor();

    const TransformationSP &getCamObject();

    const TransformationSP getKugel();

    const TransformationSP &getTorus();

    const TransformationSP &getTurm();

    const LightSP &getSonne();

    const GroupSP &getFrontLichter();

    const GroupSP &getGroup();

    const std::vector<TransformationSP> &getZielscheiben();

    static SceneObjetFactory *getInstance(ViewerSP viewer);
    const TransformationSP getStern();
private:

    const TransformAnimationSP createTorrusseTrans();

    const TransformationSP
    createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);

    const std::vector<TransformationSP> &
    getSterne(int menge, const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);

    const ShapeSP getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
                           const GeometryCoreSP &core);

    std::vector<TransformationSP> flugzeuge;

    TransformationSP &createRandompos(TransformationSP &trans);

    void createShapesWithDifferentColors(const GeometryCoreSP &torusCore, std::vector<ShapeSP> &torusShape) const;



    void createShapesWithDifferentColors(int i, const GeometryCoreSP &torusCore, std::vector<ShapeSP> &torusShape) const;

    const TransformAnimationSP getAnimatedStern();

    TransformAnimationSP
    createTorusAnimation(const GeometryCoreSP &torusCore, const std::vector<ShapeSP> &torusShape, int j, float i);
};


#endif //CG3_U3_SCENEOBJETFACTORY_H
