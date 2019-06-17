//
// Created by don on 09.06.19.
//

#ifndef CG3_U3_LIGHTFACTORY_H
#define CG3_U3_LIGHTFACTORY_H

#include "scg3.h"
using namespace scg;

class LightFactory {
private:
    LightSP primary;
    LightSP left;
    LightSP right;
    LightSP videosun;

    static LightFactory *instance;

public:
    /**
     * Right LightSource
     * @return
     */
    const LightSP& getRightLight();
    /**
     * Left LightSource
     * @return
     */
    const LightSP& getLeftLight();
    /**
     * Primary LightSource for the Game
     * @return
     */
    const LightSP& getPrimaryLight();

    /**
     * LightSource for the VideoScene
     * @return
     */
    const LightSP &getVideoLight();

private:
public:
    LightFactory();

    static LightFactory *getInstance();
};


#endif //CG3_U3_LIGHTFACTORY_H
