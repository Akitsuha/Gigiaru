#ifndef Phygiology_h
#define Phygiology_h

class Phygiology
{
private:
    /* data */
public:

};

class Hungry:Phygiology
{
private:
     u_int8_t hungry=128;
public:
    u_int8_t get_hungry(){
        return hungry;
    }

};


#endif