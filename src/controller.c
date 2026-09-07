#include "controller.h"
#include <stdio.h>


void controller_print(Controller *controller)
{
    printf("A:%d B:%d SEL:%d START:%d UP:%d DOWN:%d LEFT:%d RIGHT:%d | shift_reg:%02X strobe:%d\n",
           controller->a, controller->b, controller->select, controller->start,
           controller->up, controller->down, controller->left, controller->right,
           controller->shift_register, controller->strobe);
}



uint8_t controller_read(Controller *controller){

    if(controller->strobe){
        return controller->shift_register & 1;
    }
    else {
        uint8_t value = controller->shift_register & 1;
        controller->shift_register >>= 1;
        return value;

    }

}

void controller_write(Controller *controller, uint8_t value){
    if(value & 1){
        controller->strobe = true;
        printf("strobe updated");
    } else{
        controller->strobe = false;
    }
    
}

void controller_update(Controller *controller)
{

    controller->a = IsKeyDown(KEY_Z);
    controller->b = IsKeyDown(KEY_X);
    controller->select = IsKeyDown(KEY_A);
    controller->start = IsKeyDown(KEY_S);
    controller->up = IsKeyDown(KEY_UP);
    controller->down = IsKeyDown(KEY_DOWN);
    controller->left = IsKeyDown(KEY_LEFT);
    controller->right = IsKeyDown(KEY_RIGHT);


    //controller_print(controller);

    if (controller->strobe)
    {

        controller->shift_register = ((controller->a << A_BIT_POS) |
                                      (controller->b << B_BIT_POS) |
                                      (controller->select << SELECT_BIT_POS) |
                                      (controller->start << START_BIT_POS) |
                                      (controller->up << UP_BIT_POS) |
                                      (controller->left << LEFT_BIT_POS) |
                                      (controller->down << DOWN_BIT_POS) |
                                      (controller->right << RIGHT_BIT_POS));
    }
}
