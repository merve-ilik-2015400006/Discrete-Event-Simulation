//
// Created by Merve İlik on 14/11/2017.
//

#ifndef PROJECT2_MERVE_ILIK_2015400006_BARISTA_H
#define PROJECT2_MERVE_ILIK_2015400006_BARISTA_H
using namespace std;

#include "Order.h"
#include <vector>
#include <queue>
class Barista{
public:
    double busyTime;
    bool available;
    int orderNo;
    priority_queue<Order,vector<Order>,comparePrice> bq;
    Barista();
    vector<int> lengths;
};
#endif //PROJECT2_MERVE_ILIK_2015400006_BARISTA_H
