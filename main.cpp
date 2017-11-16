
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include "Order.h"
#include "Cashier.h"
#include "Barista.h"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>

using namespace std;

void read(char* input,int &n,int &m,vector<Order> &orders){
    ifstream infile(input);
    infile>>n>>m;
    Order o;
    for(int i=0; i<m; i++){
        setprecision(100);
        infile>>o.arrivalTime;
        infile>>o.orderTime;
        infile>>o.brewTime;
        infile>>o.price;
        o.currentTime=o.arrivalTime;
        o.id=i;
        o.status=0;
        orders.push_back(o);
    }
}

int main(int argc, char*argv[]){


    vector<Order> orders;
    vector<Cashier> cashiers;
    vector<Barista> baristas;
    priority_queue<Order,vector<Order>,compareArrivalTime> cq;
    priority_queue<Order,vector<Order>,comparePrice> bq;
    priority_queue<Order,vector<Order>,compareTime> timeLine;
    int n,m;
    int maxCqLength=0;
    int maxBqLength=0;
    double totalRunningTime=0;
    read(argv[1],n,m,orders);

    cashiers.resize(n);
    baristas.resize(n/3);

    for(int i=0;i<m;i++){

        timeLine.push(orders[i]);
    }
cout<<orders[4].arrivalTime<<endl;
    cout<<orders[4].orderTime<<endl;
    cout<<orders[4].brewTime<<endl;
    cout<<orders[4].price<<endl;
    cout<<orders[5].arrivalTime<<endl;


    while(!timeLine.empty()){
        int i;
        for(i=0;i<m;i++){
            Order o=timeLine.top();
            timeLine.pop();
            if(o.status==0){
                int a;
                bool isAvailable=false;
                for(a=0;a<n;a++){
                    if(cashiers[a].available){
                        isAvailable=true;
                        cashiers[a].busyTime+=o.orderTime;
                        cashiers[a].available=false;
                        o.currentTime+=o.orderTime;
                        cashiers[a].orderID=o.id;
                        o.status=2;
                        timeLine.push(o);
                        for(int j=0;j<m;j++){
                            if(orders[j].id==o.id){
                                orders[j]=o;
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){
                    o.status=1;
                    cq.push(o);
                    if(cq.size()>maxCqLength)
                        maxCqLength=cq.size();
                }
            }
            else if(o.status==2){
                Cashier currentCashier;
                int c;
                for(c=0;c<n;c++){
                    if(cashiers[c].orderID==o.id)
                        currentCashier=cashiers[c];
                }
                currentCashier.available= true;
                if(!cq.empty()){
                    Order ox=cq.top();
                    cq.pop();
                    currentCashier.orderID=ox.id;
                    currentCashier.available=false;
                    currentCashier.busyTime+=ox.orderTime;
                    ox.status=2;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.orderTime;
                    timeLine.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders[j].id==ox.id){
                            orders[j]=ox;
                        }
                    }
                }
                int b;
                bool isAvailable=false;
                for(b=0;b<n/3;b++){
                    if(baristas[b].available){
                        isAvailable=true;
                        baristas[b].orderID=o.id;
                        baristas[b].available=false;
                        baristas[b].busyTime+=o.brewTime;
                        o.currentTime+=o.brewTime;
                        o.status=4;
                        timeLine.push(o);
                        for(int j=0;j<m;j++){
                            if(orders[j].id==o.id){
                                orders[j]=o;
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){
                    o.status=3;
                    bq.push(o);
                    if(bq.size()>maxBqLength)
                        maxBqLength=bq.size();

                }
            }
            else if(o.status==4){
                Barista currentBarista;
                for(int b=0;b<n/3;b++){
                    if(baristas[b].orderID==o.id)
                        currentBarista=baristas[b];
                }
                if(!bq.empty()){
                    Order ox=bq.top();
                    bq.pop();
                    currentBarista.orderID=ox.id;
                    currentBarista.available=false;
                    currentBarista.busyTime+=ox.brewTime;
                    ox.status=4;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.brewTime;
                    timeLine.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders[j].id==ox.id){
                            orders[j]=ox;
                        }
                    }
                }
                o.status=5;
                totalRunningTime=o.currentTime;
            }
        }
    }

    ofstream myfile(argv[2]);
   /* int totalTime=totalRunningTime;
    int digits=0;
    while(totalTime!=0){
        totalTime=totalTime/10;
        digits++;
    }*/
    myfile<< totalRunningTime;
    myfile<<endl;
    myfile<< maxCqLength;
    myfile<<endl;
    myfile<< maxBqLength;
    myfile<<endl;
    for(int x=0;x<n;x++){
        myfile<<cashiers[x].busyTime/totalRunningTime<<endl;
    }
    for(int x=0;x<n/3;x++){
        myfile<<baristas[x].busyTime/totalRunningTime<<endl;
    }
    for(int x=0;x<m;x++){
        myfile<<orders[x].currentTime-orders[x].arrivalTime<<endl;
    }

cout<<""<<endl;

    vector<int> lengths;
    vector<Barista> baristas2;
    baristas2.resize(n/3);
    vector<Order> orders2;
    vector<Cashier> cashiers2;
    cashiers2.resize(n);
    priority_queue<Order,vector<Order>,compareArrivalTime> cq2;
    priority_queue<Order,vector<Order>,compareTime> timeLine2;

    for(int i=0;i<m;i++){
        timeLine2.push(orders2[i]);
    }

    int maxCqLength2=0;
    int maxBqLength2=0;
    double totalRunningTime2;
    while(!timeLine2.empty()){

        int i;
        for(i=0;i<m;i++){
            Order o=timeLine2.top();
            timeLine2.pop();
            if(o.status==0){
                int a;
                bool isAvailable=false;
                for(a=0;a<n;a++){
                    if(cashiers2[a].available){
                        isAvailable=true;
                        cashiers2[a].busyTime+=o.orderTime;
                        cashiers2[a].available=false;
                        o.currentTime+=o.orderTime;
                        cashiers2[a].orderID=o.id;
                        o.status=2;
                        timeLine2.push(o);
                        for(int j=0;j<m;j++){
                            if(orders2[j].id==o.id){
                                orders2[j]=o;
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){
                    o.status=1;
                    cq2.push(o);
                    if(cq2.size()>maxCqLength2)
                        maxCqLength2=cq2.size();
                }
            }
            else if(o.status==2){
                Cashier currentCashier;
                int c;
                int cashierLocation;
                for(c=0;c<n;c++){
                    if(cashiers2[c].orderID==o.id){
                        currentCashier=cashiers2[c];
                        cashierLocation=c;
                    }
                }
                currentCashier.available= true;
                if(!cq2.empty()){
                    Order ox=cq2.top();
                    cq2.pop();
                    currentCashier.orderID=ox.id;
                    currentCashier.available=false;
                    currentCashier.busyTime+=ox.orderTime;
                    ox.status=2;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.orderTime;
                    timeLine2.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders2[j].id==ox.id){
                            orders2[j]=ox;
                        }
                    }
                }
                int b;
                bool isAvailable=false;

                for(b=0;b<n/3;b++){
                    if(baristas2[b].available && b==cashierLocation/3){
                        isAvailable=true;
                        baristas2[b].orderID=o.id;
                        baristas2[b].available=false;
                        baristas2[b].busyTime+=o.brewTime;
                        o.currentTime+=o.brewTime;
                        o.status=4;
                        timeLine2.push(o);
                        for(int j=0;j<m;j++){
                            if(orders2[j].id==o.id){
                                orders2[j]=o;
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){
                    o.status=3;
                    baristas2[cashierLocation/3].bq.push(o);
                    lengths.push_back(baristas2[cashierLocation/3].bq.size());

                }
            }
            else if(o.status==4){
                Barista currentBarista;
                int baristaLocation;
                for(int b=0;b<n/3;b++){
                    if(baristas2[b].orderID==o.id){
                        currentBarista=baristas2[b];
                        baristaLocation=b;
                    }
                }
               if(!baristas2[baristaLocation].bq.empty()){
                    Order ox=baristas2[baristaLocation].bq.top();
                    baristas2[baristaLocation].bq.pop();
                    currentBarista.orderID=ox.id;
                    currentBarista.available=false;
                    currentBarista.busyTime+=ox.brewTime;
                    ox.status=4;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.brewTime;
                    timeLine.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders2[j].id==ox.id){
                            orders2[j]=ox;
                        }
                    }
                }
                o.status=5;
                totalRunningTime2=o.currentTime;
            }
        }
    }

    myfile<< totalRunningTime2;
    myfile<<endl;
    myfile<< maxCqLength2;
    myfile<<endl;
    myfile<< maxBqLength2;
    myfile<<endl;
    for(int x=0;x<n;x++){
        myfile<<cashiers2[x].busyTime/totalRunningTime2<<endl;
    }
    for(int x=0;x<n/3;x++){
        myfile<<baristas2[x].busyTime/totalRunningTime2<<endl;
    }
    for(int x=0;x<m;x++){
        myfile<<orders2[x].currentTime-orders2[x].arrivalTime<<endl;
    }






























    return 0;
}