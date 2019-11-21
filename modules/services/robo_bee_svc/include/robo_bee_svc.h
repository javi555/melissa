#ifndef ROBOBEESVC_H
#define ROBOBEESVC_H

#include <iostream>
#include <stdio.h>
#include <vector>

#include <klepsydra/core/environment.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/geometry/pose_event_data.h>
#include <klepsydra/vision_ocv/image_event_data.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace mls {
class RoboBeeSvc : public kpsr::Service {
public:
  RoboBeeSvc(kpsr::Environment *environment,
             kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher,
             int witdh, int height);

  ~RoboBeeSvc(){};

  void capture();
  void sendImg();
  void goToWaypoint();
  void startCapturing();
  void stopCapturing();
  bool isCapturing();

protected:
  void start() override;
  void stop() override;
  void execute() override;

private:
  void create();
  void onPoseReceived();

  kpsr::Publisher<kpsr::vision_ocv::ImageData> *_imageDataPublisher;
  kpsr::Subscriber<kpsr::geometry::PoseEventData> *_poseDataSubscriber;
  std::string _name;
  bool _capturing;
  int _currentImgId;
  std::string fileImagesPath;
  float _imgWidth;
  float _imgHeigh;
  bool _restartIfNoMoreImages;
};

} // namespace mls
#endif // ROBOBEESVC