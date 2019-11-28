#include "queen_bee_svc.h"

namespace mls {

QueenBeeSvc::QueenBeeSvc(
    kpsr::Environment *environment,
    kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber,
    kpsr::Publisher<mls::Waypoint> *waypointPublisher)
    : Service(environment, "queen_bee_service"),
      _imageDataSubscriber(imageDataSubscriber),
      _waypointPublisher(waypointPublisher) {}

void QueenBeeSvc::start() {
    _imageDataSubscriber->registerListener(
      "ImgDataQBSvc",
      std::bind(&QueenBeeSvc::onImgReceived, this,
                std::placeholders::_1));
}
void QueenBeeSvc::stop() {}
void QueenBeeSvc::execute() {}

void QueenBeeSvc::processImg() {
  /* Apply Hough transformation on image
     Write resultedImg on disk */
}
void QueenBeeSvc::onImgReceived(const kpsr::vision_ocv::ImageData &img) {
  spdlog::info("QueenBeeSvc: image received");
  mls::Waypoint waypoint = calculateWaypointForImage(img);
  waypoint.seq = img.seq;
  _waypointPublisher->publish(waypoint);
  spdlog::info("QueenBeeSvc: Published waypoint");
}

mls::Waypoint QueenBeeSvc::calculateWaypointForImage(const kpsr::vision_ocv::ImageData) {
  mls::Waypoint wp(0000, 1, 2, 3);
  return wp;
}

} // namespace mls
