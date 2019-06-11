//
// Created by don on 11.06.19.
//

#ifndef CG3_U3_FLOORKEYBOARDCONTROLLER_H
#define CG3_U3_FLOORKEYBOARDCONTROLLER_H


#include "scg_glew.h"
#include "CameraController.h"
#include "scg_internals.h"
namespace scg {

/**
 * \brief A camera controller using keyboard input.
 *
 * See implementation of constructor KeyboardController::KeyboardController(),
 * which prints a list of control actions.
 */
    class FloorKeyboardController: public CameraController {
    private:
        bool enable;
    public:
        NodeSP bullet;

        /**
         * Constructor with given camera transformation.
         */
        FloorKeyboardController(CameraSP camera);

        /**
         * Destructor.
         */
        virtual ~FloorKeyboardController();

        /**
         * Create shared pointer with given camera transformation.
         */
        static FloorKeyboardControllerSP create(CameraSP& camera);

        /**
         * Check input devices, called by Viewer::startMainLoop().
         *
         * \param viewState view state managed by Viewer, may be modified by controller
         */
        virtual void checkInput(ViewState* viewState);

        virtual void setBullet(NodeSP N);

        bool isEnable() const;

        void setEnable(bool enable);
    };

}




#endif //CG3_U3_FLOORKEYBOARDCONTROLLER_H