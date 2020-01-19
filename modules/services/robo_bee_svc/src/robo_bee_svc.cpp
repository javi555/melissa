#include "robo_bee_svc.h"

namespace mls
{
RoboBeeSvc::RoboBeeSvc(
    kpsr::Environment *environment,
    kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher,
    kpsr::Subscriber<mls::Waypoint> *waypointSubscriber, int witdh, int height,
    std::string imageDirname, bool restartIfNoMoreImages, int prefix)
    : Service(environment, "robo_bee_service"),
      _imageDataPublisher(imageDataPublisher),
      _waypointSubscriber(waypointSubscriber), _prefix(prefix),
      _imgWidth(witdh), _imgHeigh(height), _imageDirname(imageDirname),
      _lastWpSeq(0), _lastImageSeq(prefix),
      _restartIfNoMoreImages(restartIfNoMoreImages) {}

void RoboBeeSvc::start()
{
  _waypointSubscriber->registerListener(
      "WpRBSvc",
      std::bind(&RoboBeeSvc::onWaypointReceived, this, std::placeholders::_1));
  if (_fileNameList == nullptr)
  {
    _index = 2;
    _fileNameList = new std::vector<std::string>();
    FileUtils::getSortedListOfFilesInDir(_imageDirname, _fileNameList);
    _sz = _fileNameList->size();
    spdlog::debug("RoboBeeSvc: Read a list of {} images in folder: {}", _sz - 2,
                  _imageDirname);
  }
}

void RoboBeeSvc::stop()
{
  _waypointSubscriber->removeListener("WpRBSvc");
  _fileNameList->clear();
  _fileNameList = nullptr;
}

void RoboBeeSvc::execute()
{
  _image.frameId = "frame";
  _image.seq = ++_lastImageSeq;
  getImage(_image.img);

  if (_image.img.cols != _imgWidth || _image.img.rows != _imgHeigh)
  {
    spdlog::warn("RoboBeeSvc: Width or heigh of the image should be: {} {}", _imgWidth, _imgHeigh);
    return;
  }
  _imageDataPublisher->publish(_image);
  spdlog::info("RoboBeeSvc: Published Image {}", _image.seq);
}

bool RoboBeeSvc::hasMoreImages() { return (_index < (_sz)); }

void RoboBeeSvc::getImage(cv::Mat &img)
{
  if (hasMoreImages())
  {
    std::string image_path;
    image_path.append(_imageDirname);
    image_path.append("/");
    image_path.append((*_fileNameList)[_index++]);
    img = cv::imread(image_path);
    if (!img.data)
    {
      spdlog::warn("Could not open or find the image in{}", image_path);
    }
    return;
  }
  else
  {
    if (_restartIfNoMoreImages)
    {
      this->stop();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      this->start();
      getImage(img);
      return;
    }
  }
}

void RoboBeeSvc::onWaypointReceived(const mls::Waypoint &wp)
{
  if (_lastWpSeq < wp.seq)
  {
    _lastWpSeq = wp.seq;
    _waypoint = wp;
    spdlog::info("RoboBeeSvc: new waypoint received {} {}", _lastWpSeq, wp.seq);
    goToWaypoint(_waypoint);
  }
  else
  {
    spdlog::info("RoboBeeSvc: obsolete sequence number {} {}", _lastWpSeq, wp.seq);
  }
}

void RoboBeeSvc::goToWaypoint(const mls::Waypoint &waypoint)
{
  // fly to waypoint.x waypoint.y waypoint.z

}

} // namespace mls
