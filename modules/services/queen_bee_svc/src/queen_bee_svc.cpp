#include "queen_bee_svc.h"

namespace mls
{

QueenBeeSvc::QueenBeeSvc(
    kpsr::Environment *environment,
    kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber,
    std::vector<mls::WpPublisherConfig> waypointPublishers)
    : Service(environment, "queen_bee_service"),
      _imageDataSubscriber(imageDataSubscriber),
      _waypointPublishers(waypointPublishers) {}

void QueenBeeSvc::start()
{
  _imageDataSubscriber->registerListener(
      "ImgDataQBSvc",
      std::bind(&QueenBeeSvc::onImgReceived, this, std::placeholders::_1));
}
void QueenBeeSvc::stop()
{
  _imageDataSubscriber->removeListener("ImgDataQBSvc");
}
void QueenBeeSvc::execute() {}

void QueenBeeSvc::processImg(const kpsr::vision_ocv::ImageData &img)
{
  cv::Mat gray;
  cv::cvtColor(img.img, gray, cv::COLOR_BGR2GRAY);
  medianBlur(gray, gray, 5);

  HoughCircles(gray, _circles, cv::HOUGH_GRADIENT, 1, gray.rows / 16, 100, 30,
               1, 30);
  spdlog::info("number of circles: {}", _circles.size());
  return;
}

void QueenBeeSvc::onImgReceived(const kpsr::vision_ocv::ImageData &img)
{
  spdlog::info("QueenBeeSvc: image received");
  processImg(img);
  mls::Waypoint waypoint = calculateWaypointForImage(img);
  for (int i = 0; i < _waypointPublishers.size(); i++)
  {
    spdlog::info("QueenBeeSvc: min {}, max {}, imgSeq {}", _waypointPublishers[i]._minIndex, _waypointPublishers[i]._maxIndex, img.seq);
    if ((_waypointPublishers[i]._minIndex < img.seq) && ( img.seq < _waypointPublishers[i]._maxIndex)){
      _waypointPublishers[i]._waypointPublisher->publish(waypoint);
      spdlog::info("QueenBeeSvc: Publisher_{} waypoint {}", i, waypoint.seq);
      break;
    }  
  }
}

mls::Waypoint
QueenBeeSvc::calculateWaypointForImage(const kpsr::vision_ocv::ImageData &img)
{
  mls::Waypoint wp(img.seq, 1, 2, 3);
  return wp;
}

} // namespace mls
