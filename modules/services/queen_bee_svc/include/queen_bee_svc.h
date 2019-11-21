#ifndef QUEENBEESVC_H
#define QUEENBEESVC_H

#include <iostream>
#include <stdio.h>
#include <vector>

#include <klepsydra/core/environment.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/vision_ocv/image_event_data.h>

namespace mls {
class QueenBeeSvc : public kpsr::Service {
public:
  QueenBeeSvc(
      kpsr::Environment *environment,
      kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber);

  ~QueenBeeSvc(){};

  void sendWaypoint();
  void processImg();

protected:
  void start() override;
  void stop() override;
  void execute() override;

private:
  void create();
  void onImgReceived(const kpsr::vision_ocv::ImageData &img);

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *_imageDataSubscriber;
};

} // namespace mls
#endif // QUEENBEESVC