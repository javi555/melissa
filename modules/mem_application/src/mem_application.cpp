
#include "queen_bee_svc.h"
#include "robo_bee_svc.h"
#include "yaml-cpp/yaml.h"
#include <iostream>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include <klepsydra/high_performance/event_loop_middleware_provider.h>

int main(int argc, char **argv) {

  if (argc != 3) {
    spdlog::error("MemApp: Pass number of iterations and rb_config.yaml path");
    return 0;
  }
  int iters = std::stoi(argv[1]);
  std::string yamlFile = argv[2];

  kpsr::YamlEnvironment yamlEnv(yamlFile);

  int imgWidth;
  int imgHeight;
  int poolSize;
  int roboBeePrefix;
  int imgCvType;
  std::string fileImagesPath;


  yamlEnv.getPropertyString("file_images_path", fileImagesPath);
  yamlEnv.getPropertyInt("img_width", imgWidth);
  yamlEnv.getPropertyInt("img_height", imgHeight);
  yamlEnv.getPropertyInt("pool_size", poolSize);
  yamlEnv.getPropertyInt("robo_bee_prefix", roboBeePrefix);
  yamlEnv.getPropertyInt("img_cv_type", imgCvType);

  spdlog::info("MemApp: loaded fileImagesPath: {}", fileImagesPath);
  spdlog::info("MemApp: loaded width: {}", imgWidth);
  spdlog::info("MemApp: loaded height: {}", imgHeight);
  spdlog::info("MemApp: loaded pooSize: {}", poolSize);
  spdlog::info("MemApp: loaded roboBeePrefix: {}", roboBeePrefix);
  spdlog::info("MemApp: loaded imgCvType: {}", imgCvType);

  kpsr::high_performance::EventLoopMiddlewareProvider<16> eventloop(nullptr);

  eventloop.start();

  kpsr::vision_ocv::ImageDataFactory factory(imgWidth, imgHeight, imgCvType, "frame");

  kpsr::Subscriber<kpsr::vision_ocv::ImageData> *imageDataSubscriber =
      eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("ImageData");
  kpsr::Publisher<kpsr::vision_ocv::ImageData> *imageDataPublisher =
      eventloop.getPublisher<kpsr::vision_ocv::ImageData>(
          "ImageData", poolSize, factory.initializerFunction, factory.eventClonerFunction);

  kpsr::Subscriber<mls::Waypoint> *waypointSubscriber =
      eventloop.getSubscriber<mls::Waypoint>("Waypoint");
  kpsr::Publisher<mls::Waypoint> *waypointPublisher =
      eventloop.getPublisher<mls::Waypoint>("Waypoint", 0, nullptr, nullptr);

  std::vector<mls::WpPublisherConfig> waypointPublishers;
  waypointPublishers.push_back(mls::WpPublisherConfig(0, waypointPublisher));

  mls::RoboBeeSvc roboBeeSvc(&yamlEnv, imageDataPublisher, waypointSubscriber,
                             imgWidth, imgHeight, fileImagesPath, true, roboBeePrefix);
  mls::QueenBeeSvc queenBeeSvc(&yamlEnv, imageDataSubscriber, waypointPublishers);

  roboBeeSvc.startup();
  spdlog::info("MemApp: RoboBee Service started");
  queenBeeSvc.startup();
  spdlog::info("MemApp: QueenBee Service started");

  for (int i = 0; i < iters; i++) {
    roboBeeSvc.runOnce();
    queenBeeSvc.runOnce();
  }

  eventloop.getSubscriber<kpsr::vision_ocv::ImageData>("img")->removeListener(
      "ImageData");
  eventloop.getSubscriber<mls::Waypoint>("wp")->removeListener("Waypoint");
  eventloop.stop();

  return 0;
} // end main()