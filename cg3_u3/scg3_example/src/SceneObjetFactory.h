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
/**
 * This class Creates SubTrees of Transformations
 * It creates Transformations with its Groups Shapes and Cores
 *  Feel free to add more Transformations as you need .
 *  They can be easily added to your scene.
 *  Just by adding them whith scene->addChild(your-TransFormation);
 */
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
    TransformAnimationSP jetAniGroup;
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


    /**
     * Animated Jet
     * @return
     */
    const TransformationExtSP getJet();

    /**
     * Shape for the Sky
     * @return
     */
    const ShapeSP getSky();

    /**
     * Shape for the Video Sky
     * @return
     */
    const ShapeSP getVideoSky();


    /**
     * Transformation with Subtree for the Floor
     * with Shape and Cores
     * @return
     */
    const TransformationSP &getFloor();

    /**
  * Transformation with Subtree for the Floor
  * with Shape and Cores
  * @return
  */
    const TransformationSP &getVideoFloor();

    /**
  * Transformation with Subtree for CamObject
  * with Shape and Cores
  * @return
  */
    const TransformationSP &getCamObject();

    /**
     * Transformation with Subtree for JetGroup
     * with Subtree for the Floor
     * @return
     */
    const TransformAnimationSP createJetGroup();

    /**
     * Transformation with Subtree for JetGroup2
     * with Shape and Cores
     * @return
     */
    const TransformAnimationSP createJetGroup2();


    /**
     * Transformation with Subtree for getTower
     * with Shape and Cores
     * @return
     */
    const TransformationSP &getTower();


    /**
     * Array of Transformations with Subtree for Targets
     * with Shape and Cores
     * @return
     */
    const std::vector<TransformationSP> &getTargets();

    /**
     *
     * @param viewer
     * @return
     */
    static SceneObjetFactory *getInstance(ViewerSP viewer);

    /**
     * Transformation with Subtree for getTower
     * with Shape and Cores
     * @return
     */
    const TransformationSP getStar();

    TransformationSP &createBullet();

private:

    /**
     * HelperFunction to Create a Group Of animated Tori
     * @return
     */
    const TransformAnimationSP createToriWithAnimation();

    /**
     * HelperFunction to create An Animated Tranformation
     * with given Parameter
     * @param translate
     * @param scale
     * @param rotate
     * @param degree
     * @return
     */
    const TransformationExtSP
    createTransformation(const glm::vec3 &translate, const glm::vec3 &scale, const glm::vec3 &rotate, float degree);


    /**
     * HelperFuncttion to get a
     * @param shade
     * @param mat
     * @param textur
     * @param core
     * @return
     */
    const ShapeSP getShape(const ShaderCoreSP &shade, const MaterialCoreSP &mat, const Texture2DCoreSP &textur,
                           const GeometryCoreSP &core);


    /**
     * Calculates randompos for an given Transformation
     * @param trans
     * @return
     */
    TransformationSP &createRandompos(TransformationSP &trans);


    /**
     * HelperFunction to create different Color in an given array
     * the integer i is for the position which item of array has to change
     * @param i
     * @param torusCore
     * @param torusShape
     */
    void
    createShapesWithDifferentColors(int i, const GeometryCoreSP &torusCore, std::vector<ShapeSP> &torusShape) const;


    /**
     * Creates an rotating Star Torus
     * @return
     */
    const TransformAnimationSP getAnimatedStern();


    /**
     * Animates Tori
     * @param torusCore
     * @param torusShape
     * @param j
     * @param i
     * @return
     */
    TransformAnimationSP
    createTorusAnimation(const GeometryCoreSP &torusCore, const std::vector<ShapeSP> &torusShape, int j, float i);


    /**
     * Creates some Jets Which are Animated
     * @return
     */
    const TransformAnimationSP getFlugzeugAnimated();


};


#endif //CG3_U3_SCENEOBJETFACTORY_H
