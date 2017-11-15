//
// Created by Merve İlik on 14/11/2017.
//

#ifndef PROJECT2_MERVE_ILIK_2015400006_ORDER_H
#define PROJECT2_MERVE_ILIK_2015400006_ORDER_H
using namespace std;
class Order{
public:
    double arrivalTime;
    double orderTime;
    double brewTime;
    double price;
    int status;
    double currentTime;
    int id;
    Order();

};

struct comparePrice{
public:
    bool operator()(Order const &o1, Order const &o2){
        return o1.price>o2.price;
    }
     };

struct compareArrivalTime{
public:
    bool operator()(Order const &o1, Order const &o2){
        return o1.arrivalTime>o2.arrivalTime;
    }
};

struct compareTime{
public:
    bool operator()(Order const &o1, Order const &o2){
        return o1.currentTime>o2.currentTime;
    }
};

#endif //PROJECT2_MERVE_ILIK_2015400006_ORDER_H
