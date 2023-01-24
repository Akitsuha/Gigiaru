#ifndef SStepper_h
#define SStepper_h

// library interface description
class SStepper
{
  public:
    // constructors:
    SStepper(int number_of_steps, int motor_pin_1, int motor_pin_2);
    SStepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4);
    SStepper(int number_of_steps, int motor_pin_1, int motor_pin_2,
                                 int motor_pin_3, int motor_pin_4,
                                 int motor_pin_5);


    int version(void);
    
    void sleep();
    void set_motion(int steps_to_move);

    void stepMotor(int this_step);

    long step_number=0;// which step the motor is on
    float max_speed_spms=0.5;//モータの最高速度(step/ms)


  private:
    int number_of_steps;      // total number of steps this motor can take
    int pin_count;            // how many pins are in use.       


    // motor pin numbers:
    int motor_pin_1;
    int motor_pin_2;
    int motor_pin_3;
    int motor_pin_4;
    int motor_pin_5;          // Only 5 phase motor
};



#endif

