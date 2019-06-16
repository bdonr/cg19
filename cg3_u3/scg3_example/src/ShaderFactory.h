//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_SHADERFACTORY_H
#define CG3_U3_SHADERFACTORY_H

#include "scg3.h"

using namespace scg;

class ShaderFactory {

private:
    ShaderCoreSP phongwithTexture;
    ShaderCoreSP phongwithoutTexture;
    ShaderCoreSP phongbumb;
    ShaderCoreSP color;
    ShaderCoreFactory shaderFactory;
    static ShaderFactory* instance;
public:
    const ShaderCoreSP &getPhong(bool texturmode);

    const ShaderCoreSP &getPhongBumb();

    const ShaderCoreSP getSkyBox();
    static ShaderFactory* getInstance();
private:
    const ShaderCoreSP create(const std::string &name, bool texturemode);

    const ShaderCoreSP create(const std::string &name1, const std::string &name2);
    ShaderFactory();

public:
    virtual ~ShaderFactory();
};


#endif //CG3_U3_SHADERFACTORY_H
