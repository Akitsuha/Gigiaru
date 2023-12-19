#ifndef MyFunction_h
#define  MyFunction_h

template <typename T>
T unsigned_LPF(T& value, const T& new_data,const float& LPF_rate)
{
    T dif=0;
    if(new_data>value)
    {
        dif=new_data-value;
        value+=dif*LPF_rate;
    }
    else if(new_data<value){
        dif=value-new_data;
        value-=dif*LPF_rate;
    }
    return dif;
}

#endif