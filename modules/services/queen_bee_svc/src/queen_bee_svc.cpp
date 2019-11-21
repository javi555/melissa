#include "queen_bee_svc.h"

namespace mls {

QueenBeeSvc::QueenBeeSvc(
    kpsr::Environment *environment,
    kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber)
    : Service(environment, "queen_bee_service"),
      _imageDataSubscriber(imageDataSubscriber) {}

void QueenBeeSvc::start() {
    _imageDataSubscriber->registerListener(
      "ImgDataQBSvc",
      std::bind(&QueenBeeSvc::onImgReceived, this,
                std::placeholders::_1));
}
void QueenBeeSvc::stop() {}
void QueenBeeSvc::execute() { std::cout << "sth" << std::endl; }

void QueenBeeSvc::sendWaypoint() {}
void QueenBeeSvc::processImg() {
  /* Apply Hough transformation on image
     Write resultedImg on disk */
}
void QueenBeeSvc::onImgReceived(const kpsr::vision_ocv::ImageData &img) {
  std::cout << "Image received with id: " << img.seq;
}
} // namespace mls
