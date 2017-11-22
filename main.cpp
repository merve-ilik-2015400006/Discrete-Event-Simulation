#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include "Order.h"
#include "Cashier.h"
#include "Barista.h"
#include <iomanip>
#include <iterator>
#include <algorithm>
using namespace std;
//gets the input from the given file and initializes the data to the orders vector
void read(char* input,int &n,int &m,vector<Order> &orders){
    ifstream infile(input);
    infile>>n>>m;
    Order o;
    for(int i=0; i<m; i++){
        infile>>o.arrivalTime;
        infile>>o.orderTime;
        infile>>o.brewTime;
        infile>>o.price;
        o.currentTime=o.arrivalTime;
        o.number=i;
        o.status=0;
        orders.push_back(o);
    }
}

int main(int argc, char*argv[]){

    vector<Order> orders;
    vector<Cashier> cashiers;
    vector<Barista> baristas;
    priority_queue<Order,vector<Order>,compareTime> cq;  //we sort orders according to their current time
    priority_queue<Order,vector<Order>,comparePrice> bq; //we sort orders according to their price
    priority_queue<Order,vector<Order>,compareTime> timeLine; //we sort orders according to their operation turn
    int n; //number of cashiers
    int m; //number of orders
    int maxCqLength=0;
    int maxBqLength=0;
    double totalRunningTime=0; //time when the last order leaves the coffee shop
    read(argv[1],n,m,orders);

    cashiers.resize(n);
    baristas.resize(n/3);

    for(int i=0;i<m;i++){

        timeLine.push(orders[i]); //pushing all orders to timeLine
    }



    while(!timeLine.empty()){
        int i;
        for(i=0;i<m;i++){
            Order o=timeLine.top();
            timeLine.pop();

            if(o.status==0){ //if order is just arrived to coffee shop
                int a;
                bool isAvailable=false;
                for(a=0;a<n;a++){
                    if(cashiers[a].available){ //searching for an available cashier
                        isAvailable=true;
                        cashiers[a].busyTime+=o.orderTime;
                        cashiers[a].available=false;
                        o.currentTime+=o.orderTime;
                        cashiers[a].orderNo=o.number;
                        o.status=2;  //now order is at a cashier
                        timeLine.push(o);
                        for(int j=0;j<m;j++){
                            if(orders[j].number==o.number){
                                orders[j]=o;     //updating order's info at the vector
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){ //if there is no available cashier order will wait in cq
                    o.status=1;
                    cq.push(o);
                    if(cq.size()>maxCqLength) //if new cq's length is bigger than previous it is the new maxCqLength
                        maxCqLength=cq.size();
                }
            }
            else if(o.status==2){  //if order is at cashier
                int currentCashier;
                int c;
                for(c=0;c<n;c++){
                    if(cashiers[c].orderNo==o.number) //finding which cashier it is at
                        currentCashier=c;
                }
                cashiers[currentCashier].available= true; //that cashier is done with that order and goes available
                if(!cq.empty()){                          //for the next order if it exists
                    Order ox=cq.top();
                    cq.pop();
                    cashiers[currentCashier].orderNo=ox.number;
                    cashiers[currentCashier].available=false;
                    cashiers[currentCashier].busyTime+=ox.orderTime;
                    ox.status=2;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.orderTime;
                    timeLine.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders[j].number==ox.number){
                            orders[j]=ox;      //updating order's info at the vector
                        }
                    }
                }
                int b;
                bool isAvailable=false;
                for(b=0;b<n/3;b++){
                    if(baristas[b].available){ //searching for an available barista
                        isAvailable=true;
                        baristas[b].orderNo=o.number;
                        baristas[b].available=false;
                        baristas[b].busyTime+=o.brewTime;
                        o.currentTime+=o.brewTime;
                        o.status=4;   //now order is at a barista
                        timeLine.push(o);
                        for(int j=0;j<m;j++){
                            if(orders[j].number==o.number){
                                orders[j]=o;   //updating order's info at the vector
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){ //no available barista
                    o.status=3;   //now order is at barista queue
                    bq.push(o);
                    if(bq.size()>maxBqLength)
                        maxBqLength=bq.size(); //if new bq's length is bigger than previous it is the new maxBqLength
                }
            }
            else if(o.status==4){  //if order is at barista
                int currentBarista;
                for(int b=0;b<n/3;b++){
                    if(baristas[b].orderNo==o.number){ //finding which barista it is at
                        currentBarista=b;

                    }
                }
                baristas[currentBarista].available=true; //this barista is available for the orders at bq
                if(!bq.empty()){     //if there exist an orders at bq
                    Order ox=bq.top();
                    bq.pop();
                    baristas[currentBarista].orderNo=ox.number;
                    baristas[currentBarista].available=false;
                    baristas[currentBarista].busyTime+=ox.brewTime;
                    ox.status=4; //now it is at a barista
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.brewTime;
                    timeLine.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders[j].number==ox.number){
                            orders[j]=ox;    //updating order's info at the vector
                        }
                    }
                }
                o.status=5; //process completed
                totalRunningTime=o.currentTime;
            }
        }
    }

    ofstream myfile(argv[2]);
    myfile<< fixed<<setprecision(2)<<totalRunningTime;
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
        myfile<<orders[x].currentTime-orders[x].arrivalTime<<endl; //prints utilization time of orders
    }

    myfile<<" "<<endl;

    //model 2 starts here
    vector<Barista> baristas2;
    baristas2.resize(n/3);
    vector<Order> orders2;
    vector<Cashier> cashiers2;
    cashiers2.resize(n);
    read(argv[1],n,m,orders2);

    priority_queue<Order,vector<Order>,compareTime> cq2;
    priority_queue<Order,vector<Order>,compareTime> timeLine2;

    for(int i=0;i<m;i++){

        timeLine2.push(orders2[i]);
    }

    int maxCqLength2=0;
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
                        cashiers2[a].orderNo=o.number;
                        o.status=2;
                        timeLine2.push(o);
                        for(int j=0;j<m;j++){
                            if(orders2[j].number==o.number){
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
                int c;
                int cashierLocation;
                for(c=0;c<n;c++){
                    if(cashiers2[c].orderNo==o.number){
                        cashierLocation=c;
                    }
                }
                cashiers2[cashierLocation].available= true;
                if(!cq2.empty()){
                    Order ox=cq2.top();
                    cq2.pop();
                    cashiers2[cashierLocation].orderNo=ox.number;
                    cashiers2[cashierLocation].available=false;
                    cashiers2[cashierLocation].busyTime+=ox.orderTime;
                    ox.status=2;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.orderTime;
                    timeLine2.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders2[j].number==ox.number){
                            orders2[j]=ox;
                        }
                    }
                }
                int b;
                bool isAvailable=false;

                for(b=0;b<n/3;b++){
                    if(baristas2[b].available && b==cashierLocation/3){ //each cashier will have a specific barista
                        isAvailable=true;
                        baristas2[b].orderNo=o.number;
                        baristas2[b].available=false;
                        baristas2[b].busyTime+=o.brewTime;
                        o.currentTime+=o.brewTime;
                        o.status=4;
                        timeLine2.push(o);
                        for(int j=0;j<m;j++){
                            if(orders2[j].number==o.number){
                                orders2[j]=o;
                            }
                        }
                        break;
                    }
                }
                if(!isAvailable){
                    o.status=3;
                    baristas2[cashierLocation/3].bq.push(o);
                    baristas2[cashierLocation/3].lengths.push_back(baristas2[cashierLocation/3].bq.size());

                }
            }
            else if(o.status==4){
                int baristaLocation;
                for(int b=0;b<n/3;b++){
                    if(baristas2[b].orderNo==o.number){
                        baristaLocation=b;
                    }
                }
                if(!baristas2[baristaLocation].bq.empty()){
                    Order ox=baristas2[baristaLocation].bq.top();
                    baristas2[baristaLocation].bq.pop();
                    baristas2[baristaLocation].orderNo=ox.number;
                    baristas2[baristaLocation].available=false;
                    baristas2[baristaLocation].busyTime+=ox.brewTime;
                    ox.status=4;
                    ox.currentTime=max(o.currentTime,ox.currentTime)+ox.brewTime;
                    timeLine2.push(ox);
                    for(int j=0;j<m;j++){
                        if(orders2[j].number==ox.number){
                            orders2[j]=ox;
                        }
                    }
                }
                o.status=5;
                totalRunningTime2=o.currentTime;
            }
        }
    }

    myfile<< fixed<<setprecision(2)<<totalRunningTime2;
    myfile<<endl;
    myfile<< maxCqLength2;
    myfile<<endl;

    for(int i=0;i<n/3;i++){
        int max=0;
        for (vector<int>::iterator it =baristas2[i].lengths.begin();it!=baristas2[i].lengths.end();++it){
            if(*it>max)
                max=*it;
        }
        myfile<<max<<endl;
    }
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