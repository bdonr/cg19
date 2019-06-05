//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_SHADERFACTORY_H
#define CG3_U3_SHADERFACTORY_H

#include "../scg3/scg3.h"

using namespace scg;

class ShaderFactory {

private:
    ShaderCoreSP phongwith;
    ShaderCoreSP phongwithout;
    ShaderCoreSP gauradwith;
    ShaderCoreSP gauradwithout;
    ShaderCoreSP phongreverse;
    ShaderCoreSP phongreversewithout;
    ShaderCoreSP phongbumb;
    ShaderCoreSP color;
    ShaderCoreSP skybox;
    ShaderCoreFactory shaderFactory;
    static ShaderFactory* instance;
public:
    const ShaderCoreSP &getPhong(bool texturmode);

    const ShaderCoreSP &getGaurad(bool texturmode);

    const ShaderCoreSP &getPhongreverse(bool texturmode);

    const ShaderCoreSP &getPhongBumb();

    const ShaderCoreSP &getColor();

    const ShaderCoreSP &getSkybox(bool texturmode);
    const ShaderCoreSP getSkyBox();
    static ShaderFactory* getInstance();
private:
    const ShaderCoreSP create(const std::string &name, bool texturemode);

    const ShaderCoreSP create(const std::string &name1, const std::string &name2);
    ShaderFactory();


};


#endif //CG3_U3_SHADERFACTORY_H
