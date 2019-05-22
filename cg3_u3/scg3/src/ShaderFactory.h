//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_SHADERFACTORY_H
#define CG3_U3_SHADERFACTORY_H

#include "../scg3/scg3.h"
using namespace scg;

class ShaderFactory {

private:
    static ShaderCoreSP phongwith;
    static ShaderCoreSP phongwithout;
    static ShaderCoreSP gauradwith;
    static ShaderCoreSP gauradwithout;
    static ShaderCoreSP phongreverse;
    static ShaderCoreSP color;
    static ShaderCoreSP skybox;


public:
    static const ShaderCoreSP &getPhong(bool texturmode);

    static const ShaderCoreSP &getGaurad(bool texturmode);

    static const ShaderCoreSP &getPhongreverse(bool texturmode);

    static const ShaderCoreSP &getColor();

    static const ShaderCoreSP &getSkybox(bool texturmode);

private:
    static const ShaderCoreSP create(const std::string& name,bool texturemode);
    static ShaderCoreFactory shaderFactory;
};


#endif //CG3_U3_SHADERFACTORY_H
