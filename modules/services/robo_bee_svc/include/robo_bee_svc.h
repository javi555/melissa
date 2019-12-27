#ifndef ROBOBEESVC_H
#define ROBOBEESVC_H

#include <iostream>
#include <stdio.h>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"
#include <opencv2/opencv.hpp>

#include <klepsydra/core/environment.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/vision_ocv/image_event_data.h>

#include "waypoint.h"
#include <file_utils.h>

namespace mls {
class RoboBeeSvc : public kpsr::Service {
public:
  RoboBeeSvc(kpsr::Environment *environment,
             kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher,
             kpsr::Subscriber<mls::Waypoint> *waypointSubscriber, int witdh,
             int height, std::string imageDirname, bool restartIfNoMoreImages,
             int prefix);

  ~RoboBeeSvc(){};

  void goToWaypoint(const mls::Waypoint &waypoint);

  mls::Waypoint _waypoint;

protected:
  void start() override;
  void stop() override;
  void execute() override;

private:
  void create();
  void onWaypointReceived(const mls::Waypoint &wp);
  bool hasMoreImages();
  void getImage(cv::Mat &img);
  int _prefix;
  kpsr::vision_ocv::ImageData _image;
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *_imageDataPublisher;
  kpsr::Subscriber<mls::Waypoint> *_waypointSubscriber;
  std::string _fileImagesPath;
  int _imgWidth;
  int _imgHeigh;
  int _lastImageSeq;
  int _lastWpSeq;
  std::string _imageDirname;
  bool _restartIfNoMoreImages;
  std::vector<std::string> *_fileNameList = nullptr;
  unsigned _index;
  std::vector<int>::size_type _sz;
};

} // namespace mls
#endif // ROBOBEESVC