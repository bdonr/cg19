//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_SCENEOBJETFACTORY_H
#define CG3_U3_SCENEOBJETFACTORY_H
#include "../scg3/scg3.h"
#include "EnvoirementHelper.h"
#include "MatFactory.h"
#include "TexturHelper.h"
#include "ShaderFactory.h"
#include "SceneObjetFactory.h"

using namespace scg;
class SceneObjetFactory {
    static TransformationSP himmelTrans;
    static TransformationSP jetTrans;
    static TransformationSP camObject;
    static TransformationSP floorTrans;
    static LightSP sonne;
    static LightSP mond;
    static GeometryCoreFactory geometryFactory;
    static GroupSP group;
    static std::vector<TransformationSP> transformations;
    static std::vector<TransformationSP> zielscheiben;
public:
    static const TransformationSP &getFlugzeug();

    static const TransformationSP &getHimmel();

    static const TransformationSP &getFloor();

    static const TransformationSP &getCamObject();
    static const TransformationSP getKugel();

    static const  TransformationSP& getTorus();
    static const LightSP &getSonne();
    static const LightSP& getMond();
    static const GroupSP &getGroup();
    static const std::vector<TransformationSP> &getZielscheiben();


private:

    static const TransformationSP createTorrusseTrans();
    static const TransformationSP createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);
    static const std::vector<TransformationSP>& getSterne(int menge,const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);
    static const ShapeSP getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
             const GeometryCoreSP &core);
    std::vector<TransformationSP> flugzeuge;
    static TransformationSP& createRandompos(TransformationSP& trans);
};


#endif //CG3_U3_SCENEOBJETFACTORY_H
