#include "robo_bee_svc.h"

namespace mls {

RoboBeeSvc::RoboBeeSvc(
    kpsr::Environment *environment,
    kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher,
    kpsr::Subscriber<mls::Waypoint> *waypointSubscriber, int witdh, int height,
    std::string imageDirname, bool restartIfNoMoreImages)
    : Service(environment, "robo_bee_service"),
      _imageDataPublisher(imageDataPublisher),
      _waypointSubscriber(waypointSubscriber), _imgWidth(witdh),
      _imgHeigh(height), _imageDirname(imageDirname), _lastWpSeq(0),
      _lastImageSeq(0), _restartIfNoMoreImages(restartIfNoMoreImages) {}

void RoboBeeSvc::start() {
  _waypointSubscriber->registerListener(
      "WpRBSvc",
      std::bind(&RoboBeeSvc::onWaypointReceived, this, std::placeholders::_1));

  if (fileNameList == nullptr) {
    index = 2;
    fileNameList = new std::vector<std::string>();
    FileUtils::getSortedListOfFilesInDir(_imageDirname, fileNameList);
    sz = fileNameList->size();
    spdlog::info("RoboBeeSvc: Read a list of {} images in folder: {}", sz - 2,
                 _imageDirname);
  }
}
void RoboBeeSvc::stop() { fileNameList = nullptr; }

void RoboBeeSvc::execute() {

  _image.frameId = "frame";
  fullScaleImage = getImage();
  _image.seq = ++_lastImageSeq;
  resize(fullScaleImage, _image.img, cvSize(_imgWidth, _imgHeigh));
  _imageDataPublisher->publish(_image);
  spdlog::info("RoboBeeSvc: Published Image");
}

bool RoboBeeSvc::hasMoreImages() { return (index < (sz)); }

cv::Mat RoboBeeSvc::getImage() {
  if (hasMoreImages()) {
    std::string image_path;
    image_path.append(_imageDirname);
    image_path.append("/");
    image_path.append((*fileNameList)[index++]);
    _fileImage = cv::imread(image_path);
    if (!_fileImage.data) {
      spdlog::warn("Could not open or find the image in{}", image_path);
    }

    return _fileImage;

  } else {
    if (_restartIfNoMoreImages) {
      this->stop();
      this->start();
      return getImage();
    }
  }
}

void RoboBeeSvc::onWaypointReceived(const mls::Waypoint &wp) {
  if (_lastWpSeq < wp.seq) {
    _lastWpSeq = wp.seq;
    _waypoint = wp;
    spdlog::info("RoboBeeSvc: new waypoint received");
  } else {
    spdlog::info("RoboBeeSvc: obsolete sequence number ");
  }
}

void RoboBeeSvc::goToWaypoint() {
  // fly to defined wp
}

} // namespace mls