#ifndef QUEENBEE_H
#define QUEENBEE_H

#include <stdio.h>
#include <iostream>
#include <vector>

namespace mls {

class QueenBee
{
  public:
    QueenBee(std::string name);

    void onImgReceived();
    void sendWaypoint();
    void processImg();

  private:
    std::string _name;
};

} // namespace mls
#endif //QUEENBEE