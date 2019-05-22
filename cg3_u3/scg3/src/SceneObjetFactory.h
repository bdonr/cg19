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
using namespace scg;
class SceneObjetFactory {
    static TransformationSP himmelTrans;
    static TransformationSP jetTrans;
    static TransformationSP camObject;
    static TransformationSP floorTrans;
    static GroupSP torus;
    static LightSP sonne;
    static GeometryCoreFactory geometryFactory;
    static GroupSP group;
    static std::vector<TransformationSP> transformations;
    static std::vector<TransformationSP> torusseTrans;
public:
    static const TransformationSP &getFlugzeug();

    static const TransformationSP &getHimmel();

    static const TransformationSP &getFloor();

    static const TransformationSP &getCamObject();
    static const TransformationSP getKugel();

    static const  GroupSP& getTorus();
    static const LightSP &getSonne();

    static const GroupSP &getGroup();


    const std::vector<TransformationSP> &getFlugzeuge() const;

    void setFlugzeuge(const std::vector<TransformationSP> &flugzeuge);

private:

    static const TransformationSP createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);
    static const std::vector<TransformationSP>& getSterne(int menge,const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);
    static const ShapeSP getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
             const GeometryCoreSP &core);
    std::vector<TransformationSP> flugzeuge;

   static void createTorrusseTrans(const ShapeSP &torusShape);
};


#endif //CG3_U3_SCENEOBJETFACTORY_H
