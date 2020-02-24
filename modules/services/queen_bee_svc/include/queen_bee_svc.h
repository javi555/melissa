#ifndef QUEENBEESVC_H
#define QUEENBEESVC_H

#include <iostream>
#include <stdio.h>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <opencv2/core/matx.hpp>
#include <opencv2/opencv.hpp>

#include <klepsydra/core/environment.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/vision_ocv/image_event_data.h>

#include "waypoint.h"

namespace mls
{
struct WpPublisherConfig
{
  int _minIndex;
  int _maxIndex;
  kpsr::Publisher<Waypoint> *_waypointPublisher;
  WpPublisherConfig(){}
  WpPublisherConfig(int index, kpsr::Publisher<Waypoint> *waypointPublisher)
  : _minIndex(index*100000)
  , _maxIndex((index+1)*100000)
  , _waypointPublisher(waypointPublisher)
  {}
};
class QueenBeeSvc : public kpsr::Service
{
public:
  QueenBeeSvc(
      kpsr::Environment *environment,
      kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber,
      std::vector<mls::WpPublisherConfig> waypointPublishers);

  ~QueenBeeSvc(){};

  mls::Waypoint
  calculateWaypointForImage(const kpsr::vision_ocv::ImageData &img);
  void processImg(const kpsr::vision_ocv::ImageData &img);

  std::vector<cv::Vec3f> _circles;

protected:
  void start() override;
  void stop() override;
  void execute() override;

private:
  void create();
  void onImgReceived(const kpsr::vision_ocv::ImageData &img);

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *_imageDataSubscriber;
  std::vector<mls::WpPublisherConfig> _waypointPublishers;
};

} // namespace mls
#endif // QUEENBEESVC