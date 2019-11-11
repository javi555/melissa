#include "robo_bee.h"

namespace mls {

RoboBee::RoboBee(std::string name)
: _name(name)
, _capturing(false)
//YAMLConfigurationParams
{}

void RoboBee::capture()
{
     _currentImgId++;
     // _img=image
}
void RoboBee::sendImg()
{
    //publish img to QueenBee via ZMQ
}
void RoboBee::onPoseReceived()
{
    //Received pose from QueenBee and update waypoint destination
}


void RoboBee::startCapturing()
{
    _capturing = true;
}
void RoboBee::stopCapturing()
{
    _capturing = false;
}
bool RoboBee::isCapturing()
{
    return _capturing;
}

} // namespace mls
