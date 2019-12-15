#include "queen_bee_svc.h"

namespace mls {

QueenBeeSvc::QueenBeeSvc(
    kpsr::Environment *environment,
    kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber,
    kpsr::Publisher<mls::Waypoint> *waypointPublisher)
    : Service(environment, "queen_bee_service"),
      _imageDataSubscriber(imageDataSubscriber),
      _waypointPublisher(waypointPublisher),
      _receivedImage(false) {}

void QueenBeeSvc::start() {
  _imageDataSubscriber->registerListener(
      "ImgDataQBSvc",
      std::bind(&QueenBeeSvc::onImgReceived, this, std::placeholders::_1));
}
void QueenBeeSvc::stop() {}
void QueenBeeSvc::execute() {}

void QueenBeeSvc::processImg(const kpsr::vision_ocv::ImageData &img) {
  cv::Mat gray;
  cv::cvtColor(img.img, gray, cv::COLOR_BGR2GRAY);
  medianBlur(gray, gray, 5);

  HoughCircles(gray, _circles, cv::HOUGH_GRADIENT, 1, gray.rows / 16, 100, 30,
               1, 30);
  spdlog::info("number of circles: {}", _circles.size());
  _receivedImage = false;
  return;
}

void QueenBeeSvc::onImgReceived(const kpsr::vision_ocv::ImageData &img) {
  spdlog::info("QueenBeeSvc: image received");
  processImg(img);
  mls::Waypoint waypoint = calculateWaypointForImage(img);
  _waypointPublisher->publish(waypoint);
  spdlog::info("QueenBeeSvc: Published waypoint");
  _receivedImage = true;
}

mls::Waypoint
QueenBeeSvc::calculateWaypointForImage(const kpsr::vision_ocv::ImageData &img) {
  mls::Waypoint wp(img.seq, 1, 2, 3);
  return wp;
}

} // namespace mls
