
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
using namespace std;

class Vessel
{
public:
    /* data */
    int m_arrivalTime;
    int m_processingTime;
    int m_index;
    int m_size;
    int m_weight = 1;

public:
    Vessel(int index, int size, int arrivalTime, int processingTime, int weight)
        : m_index(index), m_size(size), m_arrivalTime(arrivalTime), m_processingTime(processingTime), m_weight(weight)
    {
    };
};

struct Berth
{
    int startPoint;
    int endPoint;
    vector<Vessel*> vessels;
};


struct Schedule
{
    int position = 0;
    int mooringTime = 0;
};

int berthLength= 0;
vector<int> breakPoints;
vector<Berth> berthChilds;
vector<Vessel*> vessels;
map<Vessel*, Schedule> vesselsSchedule;

void init()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
}

void getInput()
{
    string line = "";
    while (line != "% Berth length")
    {
        getline(cin,line);
    }
    cin >> berthLength;

    while (line != "% Berth breaks")
    {
        getline(cin,line);
    }

    getline(cin,line);
    while (!line.empty())
    {
        int breakPoint;
        breakPoint = stoi(line);
        breakPoints.push_back(breakPoint);
        breakPoint = 0;
        getline(cin,line);
    }

    while (line != "% Vessel index, size, arrival time, processing time, weight (=1 as default)")
    {
        getline(cin,line);
    }

    getline(cin,line);
    while (!line.empty())
    {
        int idx, size, aT, pT, w;
        w = 1;
        stringstream vesselData(line);
        vesselData >> idx >> size >> aT >> pT >> w;
        vessels.push_back(new Vessel(idx, size, aT, pT, w));
        getline(cin,line);
    }
    /*
    //=========================================================
    // verify parse data
    cout <<"Berth length"<< berthLength << endl;
    cout << "break:" << endl;
    for (auto b : breakPoints){
        cout << b << endl;
    }
    cout << "vessels:" << endl;
    for (auto v: vessels){
        cout << v->m_index << " " << v->m_size << " " << v->m_arrivalTime << " " << v->m_processingTime << " " << v->m_weight << endl;
    }
    //===================================================
    */
}

void printOutput(){
    cout << "% Vessel index, mooring time $u_i$, starting berth position occupied $v_i$" << endl;
    for (auto v : vessels){
        cout << v->m_index << "   ";
        cout << vesselsSchedule[v].mooringTime << "   ";
        cout << vesselsSchedule[v].position << endl;
    }
}

bool checkScheduleValid(Vessel v, Schedule s){
    int x1 = s.mooringTime;
    int x2 = s.mooringTime + v.m_processingTime;
    int y1 = s.position;
    int y2 = s.position + v.m_size;
    if (s.mooringTime < v.m_arrivalTime) return false;
    for (auto vesselSchedule : vesselsSchedule){
        Vessel vi = *vesselSchedule.first;
        Schedule si = vesselSchedule.second;
        int xi1 = si.mooringTime;
        int xi2 = si.mooringTime + vi.m_processingTime;
        int yi1 = si.position;
        int yi2 = si.position + vi.m_size;
        if (!(xi1 >= x2 || x1 >= xi2 || yi1 >= y2 || y1 >= yi2)){
            return false;
        }
    }

    for (auto b : berthChilds){
        if ((y1 < b.endPoint) && (b.endPoint < y2)){
            return false;
        }
    }

    return true;
}

int main ()
{
    init();
    getInput();
    vector<Vessel*> vesselsPriority(vessels);
    sort(vesselsPriority.begin(),vesselsPriority.end(), [](Vessel* v1, Vessel* v2){ 
        return((v1->m_arrivalTime + v1->m_processingTime)*v2->m_weight < (v2->m_arrivalTime + v2->m_processingTime)*v1->m_weight);});
    
    
    Berth berth;
    for (int i = 0; i < breakPoints.size(); i++){
        berth.startPoint = (i == 0) ? 0 : breakPoints[i-1];
        berth.endPoint = breakPoints[i];
        berthChilds.push_back(berth);
    }
    berth.startPoint = breakPoints.back();
    berth.endPoint = berthLength;
    berthChilds.push_back(berth);
    sort(berthChilds.begin(), berthChilds.end(), [](Berth b1, Berth b2){ return((b1.endPoint - b1.startPoint) < (b2.endPoint - b2.startPoint));});
    /*
    //===============
    // verify sort
    cout << "vessels priority:" << endl;
    for(auto v : vesselsPriority){
        cout << v->m_index <<endl;
    }
    //==============
    */

    for (auto vessel : vesselsPriority){
        Schedule schedule;
        //Todo here ===>>>>>
        
        vector<Schedule> S;
        for (auto b : berthChilds){
            if (b.vessels.empty()){
                Schedule s1;
                s1.position = b.startPoint;
                s1.mooringTime = vessel->m_arrivalTime;
                if (checkScheduleValid(*vessel, s1)){
                    S.push_back(s1);
                    break;
                }
            } else{
                for (auto v_last : b.vessels){
                    if (vesselsSchedule[v_last].mooringTime + v_last->m_processingTime < vessel->m_arrivalTime) continue;
                    Schedule s;
                    s.position = b.startPoint;
                    s.mooringTime = vesselsSchedule[v_last].mooringTime + v_last->m_processingTime;
                    if (checkScheduleValid(*vessel, s)){
                        S.push_back(s);
                    }
                }

                Schedule s1;
                s1.position = b.startPoint;
                s1.mooringTime = vessel->m_arrivalTime;
                if (checkScheduleValid(*vessel, s1)){
                    S.push_back(s1);
                }

                for (auto v_p : b.vessels){       
                    if (vesselsSchedule[v_p].mooringTime + v_p->m_processingTime < vessel->m_arrivalTime) continue;
                    for (auto v_last : b.vessels){
                        if (vesselsSchedule[v_last].mooringTime + v_last->m_processingTime < vessel->m_arrivalTime) continue;
                        Schedule s;
                        s.position = vesselsSchedule[v_p].position + v_p->m_size;
                        s.mooringTime = vesselsSchedule[v_last].mooringTime + v_last->m_processingTime;
                        if (checkScheduleValid(*vessel, s)){
                            S.push_back(s);
                        }
                    }
                    Schedule s2;
                    s2.position = vesselsSchedule[v_p].position + v_p->m_size;
                    s2.mooringTime = vessel->m_arrivalTime;
                    if (checkScheduleValid(*vessel, s2)){
                        S.push_back(s2);
                    }

                }
            }
        }

        // get best schedule from S
        vector<Schedule> s_potential;
        s_potential.push_back(S.front());
        int late_time_min = S.front().mooringTime - vessel->m_arrivalTime;
        for (auto s : S)
        {
            int lateTime = s.mooringTime - vessel->m_arrivalTime;
            if (lateTime < late_time_min) {
                s_potential.clear();
                late_time_min = lateTime;
                s_potential.push_back(s);
            } else if(lateTime == late_time_min){
                s_potential.push_back(s);
            }
        }
        schedule = s_potential.front();
        int position_min = s_potential.front().position;
        for (auto s : s_potential)
        {
            if (s.position < position_min) {
                position_min = s.position;
                schedule = s;
            }
        }

        for (auto &b : berthChilds){
            if ((b.startPoint <= schedule.position) && (schedule.position < b.endPoint)) {
                b.vessels.push_back(vessel);
                break;
            }
        }
        vesselsSchedule.insert({vessel, schedule});
    }

    printOutput();
}
