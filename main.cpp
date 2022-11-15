
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

class Berth
{
private:
    int m_length;
    vector<int> m_breakPoints;
public:
    Berth(int length, vector<int> breakPoints)
    : m_length(length), m_breakPoints(breakPoints)
    {
    };
};

struct Schedule
{
    int position = 0;
    int mooringTime = 0;
};

int berthLength= 0;
vector<int> breakPoints;
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

int main ()
{
    init();
    getInput();
    vector<Vessel*> vesselsPriority(vessels);
    sort(vesselsPriority.begin(),vesselsPriority.end(), [](Vessel* v1, Vessel* v2){ 
        return((v1->m_arrivalTime + v1->m_processingTime)*v2->m_weight < (v2->m_arrivalTime + v2->m_processingTime)*v1->m_weight);});
    
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

        // int aT = vessel->m_arrivalTime;
        // int pT = vessel->m_processingTime;
        // int s = vessel->m_size;

        


        vesselsSchedule.insert({vessel, schedule});
    }

    printOutput();
}

