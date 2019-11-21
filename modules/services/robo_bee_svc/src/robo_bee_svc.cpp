#include "robo_bee_svc.h"

namespace mls {

RoboBeeSvc::RoboBeeSvc(
    kpsr::Environment *environment,
    kpsr::Publisher<kpsr::vision_ocv::ImageData> *
                   imageDataPublisher,
                   int witdh, int height)
    :
    Service(environment, "robo_bee_service")
    , _imageDataPublisher(imageDataPublisher)
    , _imgWidth(witdh)
    , _imgHeigh(height)
    , _capturing(false){}

void RoboBeeSvc::start() {}
void RoboBeeSvc::stop() {}
void RoboBeeSvc::execute() {
    std::cout << "execute" << std::endl;
    kpsr::vision_ocv::ImageData image;
    _imageDataPublisher->publish(image);
}

void RoboBeeSvc::capture()
{
     _currentImgId++;
     // _img=image
}
void RoboBeeSvc::sendImg()
{
    //publish img to QueenBee via ZMQ
    std::cout << "Publish Image, resolution: " << _imgWidth << " x " << _imgHeigh;
}
void RoboBeeSvc::onPoseReceived()
{
    //Received pose from QueenBee and update waypoint destination
}
void RoboBeeSvc::goToWaypoint()
{
    //fly to defined wp
}

void RoboBeeSvc::startCapturing()
{
    _capturing = true;
}
void RoboBeeSvc::stopCapturing()
{
    _capturing = false;
}
bool RoboBeeSvc::isCapturing()
{
    return _capturing;
}

} // namespace mls
