#ifndef Plot_h
#define Plot_h

#include <map>
#include <memory>
#include <vector>
#include <algorithm>


using std::vector;
using std::shared_ptr;

typedef size_t motion_t;

struct Frame{
    motion_t duration;

    Frame(motion_t duration):duration(duration){}

    virtual float get_to(float from)=0;
};

struct Frame_Abs:public Frame{
    float to;

    Frame_Abs(motion_t duration,float to):Frame(duration),to(to){}

    float get_to(float from){
        return to;
    }
};

struct Frame_Rel:public Frame{
    float dif;

    Frame_Rel(motion_t duration,float dif):Frame(duration),dif(dif){}

    float get_to(float from){
        return from+dif;
    }
};


typedef std::shared_ptr<Frame> Frame_ptr;
Frame_ptr make_frame_ptr(Frame* frame){return std::shared_ptr<Frame>(frame);}

typedef std::pair<motion_t,Frame_ptr> Frame_typed;

class Plot
{
private:
    std::map<int,vector<Frame_typed>> frames={};
    std::map<int,motion_t> durations={};
public:
    typedef shared_ptr<Plot> _Plot_ptr;
    Plot(const std::map<int,vector<Frame_typed>>& frames){
        this->frames=frames;
        for(auto itr_fs=frames.begin();itr_fs!=frames.end();itr_fs++){
            int id=itr_fs->first;
            if(itr_fs->second.size()==0){
                durations[id]=0;
            }
            else{
                auto end_frame=itr_fs->second.back();
                durations[id]=end_frame.first+end_frame.second->duration;
            }
        }
    }
    Plot(int id,const vector<Frame_typed>& frame){
        this->frames[id]=frame;
        if(frame.size()==0){
            durations[id]=0;
        }
        else{
            auto end_frame=frame.back();
            durations[id]=end_frame.first+end_frame.second->duration;
        }
    }
    Plot(const std::map<int,vector<Frame_ptr>>& frames){

        for(auto itr_fs=frames.begin();itr_fs!=frames.end();itr_fs++){
            int id=itr_fs->first;

            this->frames[id]={};

            motion_t start=0;
            for(auto itr_f=itr_fs->second.begin();itr_f!=itr_fs->second.end();itr_f++){
                this->frames[id].push_back({start,*(itr_f)});
                start += (*(itr_f))->duration;
            }

            if(itr_fs->second.size()==0){
                durations[id]=0;
            }
            else{
                auto end_frame=this->frames[id].back();
                durations[id]=end_frame.first+end_frame.second->duration;
            }
        }
    }
    Plot(int id,const vector<Frame_ptr>& frame){

        this->frames[id]={};

        motion_t start=0;
        for(auto itr_f=frame.begin();itr_f!=frame.end();itr_f++){
            this->frames[id].push_back({start,*(itr_f)});
            start += (*(itr_f))->duration;
        }

        if(frame.size()==0){
            durations[id]=0;
        }
        else{
            auto end_frame=this->frames[id].back();
            durations[id]=end_frame.first+end_frame.second->duration;
        }
    }
    Plot(const vector<_Plot_ptr>& plots){
        for(auto itr_p=plots.begin();itr_p!=plots.end();itr_p++){
            auto each_frames=(*itr_p)->get_frame();
            motion_t all_duration=0;
            if(durations.size()>0){
                all_duration=this->get_duration();
            }
            for(auto itr_fs=each_frames.begin();itr_fs!=each_frames.end();itr_fs++){
                int id=itr_fs->first;
                auto itr_newfs=this->frames.find(id);
                if(itr_newfs==this->frames.end()){
                    this->frames[id]={};
                }
                for(auto itr_f=itr_fs->second.begin();itr_f!=itr_fs->second.end();itr_f++){
                    motion_t frame_start=all_duration+itr_f->first;
                    this->frames[id].push_back({frame_start,itr_f->second});
                }
                auto end_frame=this->frames[id].back();
                this->durations[id]=end_frame.first+end_frame.second->duration;
            }
        }
    }

    const std::map<int,vector<Frame_typed>>& get_frame(){
        return frames;
    }

    motion_t get_duration(){
        return std::max_element(this->durations.begin(),this->durations.end(),[](std::pair<int,motion_t> const &x, std::pair<int,motion_t> const &y){return x.second<y.second;})->second;
    }

    motion_t get_duration(int act_id){
        return durations[act_id];
    }



    int add_act(int act_id,const vector<Frame_typed>& frames){
        auto itr=this->frames.find(act_id);
        if(itr!=this->frames.end()){
            return 0;
        }
        this->frames[act_id]=frames;
        
        auto end_frame=this->frames[act_id].back();
        this->durations[act_id]=end_frame.first+end_frame.second->duration;

        return 1;
    }
    int add_act_map(const std::map<int,vector<Frame_typed>>& frames){
        int ret=1;
        for(auto itr_fs=frames.begin();itr_fs!=frames.end();itr_fs++){
            ret*=this->add_act(itr_fs->first,itr_fs->second);
        }

        return ret;
    }
    int add_frames(const std::map<int,vector<Frame_typed>>& adder){
        motion_t pre_all_duration=get_duration();
        for(auto itr_map=adder.begin();itr_map!=adder.end();itr_map++){
            int act_id=itr_map->first;

            for(auto itr_fr=itr_map->second.begin();itr_fr!=itr_map->second.end();itr_fr++){
                frames[act_id].push_back({pre_all_duration+itr_fr->first,itr_fr->second});
            }
            auto end_frame=frames[act_id].back();
            durations[act_id]=end_frame.first+end_frame.second->duration;
        }
        return 1;
    }
    int add_frame_eachend(int act_id,const vector<Frame_typed>& frames){
        auto this_frame=this->frames[act_id];
        int pre_duration=durations[act_id];
        for(auto itr=frames.begin();itr!=frames.end();itr++){
            this_frame.push_back({pre_duration+itr->first,itr->second});
        }
        auto end_frame=this_frame.back();
        this->durations[act_id]=end_frame.first+end_frame.second->duration;
        return 1;
    }
    int add_frame_allend(int act_id,const vector<Frame_typed>& frames){
        motion_t pre_all_duration=std::max_element(this->durations.begin(),this->durations.end(),[](std::pair<int,motion_t> const &x, std::pair<int,motion_t> const &y){return x.second<y.second;})->second;
        for(auto itr=frames.begin();itr!=frames.end();itr++){
            this->frames[act_id].push_back({pre_all_duration+itr->first,itr->second});
        }
        auto end_frame=this->frames[act_id].back();
        this->durations[act_id]=end_frame.first+end_frame.second->duration;
        return 1;
    }
};

class Plot_ptr:public Plot::_Plot_ptr{
public:
    //コンストラクタ
    Plot_ptr(Plot* plot):std::shared_ptr<Plot>(plot){}
    Plot_ptr(const std::map<int,vector<Frame_typed>>& frames):std::shared_ptr<Plot>(new Plot(frames)){}
    Plot_ptr(const std::map<int,vector<Frame_ptr>>& frames):std::shared_ptr<Plot>(new Plot(frames)){}
    Plot_ptr(const vector<Plot::_Plot_ptr>& plots):std::shared_ptr<Plot>(new Plot(plots)){}
    Plot_ptr(int id,const vector<Frame_typed>& frame):std::shared_ptr<Plot>(new Plot(id,frame)){}
    Plot_ptr(int id,const vector<Frame_ptr>& frame):std::shared_ptr<Plot>(new Plot(id,frame)){}

    //演算子オーバーロード
    Plot_ptr operator +(const Plot_ptr& b)const
    {
        return Plot_ptr(new Plot(vector<Plot::_Plot_ptr>{(*this),b}));
    }
    Plot_ptr operator +=(const Plot_ptr& b)
    {
        (*this)->add_frames(b->get_frame());
        return *this;
    }
    Plot_ptr operator *(const Plot_ptr& b)const
    {
        Plot* plot=new Plot(std::map<int,vector<Frame_typed>>((*this)->get_frame()));
        plot->add_act_map(b->get_frame());
        return Plot_ptr(plot);
    }
    /// @brief コンストラクタで棄てられるPlotをたくさん複製してるのであんまり良くない。
    /// @param n 繰り返し
    /// @return 
    Plot_ptr operator *(const size_t& n)const
    {
        return Plot_ptr(new Plot(vector<Plot::_Plot_ptr>(n,*this)));
    }
};

Plot_ptr make_plotptr(Plot* plot){
    return Plot_ptr(plot);
}




#endif