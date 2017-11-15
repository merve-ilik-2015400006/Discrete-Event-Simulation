
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include "Order.h"
#include "Cashier.h"
#include "Barista.h"
using namespace std;

void read(char* input,int &n,int &m,vector<Order> &orders){
    ifstream infile(input);
    infile>>n>>m;
    Order o;
    for(int i=0; i<m; i++){
        infile>>o.arrivalTime;
        infile>>o.orderTime;
        infile>>o.brewTime;
        infile>>o.price;
        o.arrivalTime=o.currentTime;
        o.id=i;
        o.status=0;
        orders.push_back(o);

    }
}


int main(int argc, char*argv[]){


    vector<Order> orders;
    vector<Cashier> cashiers;
    vector<Barista> baristas;
    int n,m;
    int maxCqLength=0;
    int maxBqLength=0;
    double totalRunningTime=0;
    read(argv[1],n,m,orders);
cout<<m;
cout<<n;
    cashiers.resize(n);
    baristas.resize(n/3);

    priority_queue<Order,vector<Order>,compareArrivalTime> cq;
    priority_queue<Order,vector<Order>,comparePrice> bq;
    priority_queue<Order,vector<Order>,compareTime> timeLine;

    for(int i=0;i<m;i++){

        timeLine.push(orders[i]);
    }
cout<< orders[1].arrivalTime;
    while(!timeLine.empty()){
        int i;
        for(i=0;i<m;i++){
            Order o=timeLine.top();
            timeLine.pop();
            if(o.status==0){
                int a;
                for(a=0;a<n;a++){
                    if(cashiers[a].available){
                        cashiers[a].busyTime+=o.orderTime;
                        cashiers[a].available=false;
                        o.currentTime+=o.orderTime;
                        cashiers[a].orderID=o.id;
                        o.status=2;
                        timeLine.push(o);
                        break;
                    }
                }
                if(a==n-1){      //timea bir şey yapıyor muyuz burada?
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
                    ox.currentTime+=ox.orderTime;
                    timeLine.push(ox);
                }
                int b;
                for(b=0;b<n/3;b++){
                    if(baristas[b].available){

                        baristas[b].orderID=o.id;
                        baristas[b].available=false;
                        baristas[b].busyTime+=o.brewTime;
                        o.currentTime+=o.brewTime;
                        o.status=4;
                        timeLine.push(o);
                        break;
                    }
                }
                if(b==n/3-1){   //timea bir şey yapıyor muyuz burada?
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
                    ox.currentTime+=ox.brewTime;
                    timeLine.push(ox);
                }
                o.status=5;
                totalRunningTime=o.currentTime;
            }
        }

    }

    ofstream myfile(argv[2]);
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


   /* priority_queue baristaArr[n/3];
    for(int i=1;i<=n/3;i++){

    }*/




















    return 0;
}