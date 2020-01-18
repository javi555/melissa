#include <iostream>
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include "zmq_middleware_facility.h"
#include "config.h"
#include "queen_bee_svc.h"

int main(int argc, char **argv)
{

  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  std::string imageTopic;
  std::string wpTopic;
  int poolSize;
  int imgWidth;
  int imgHeight;
  std::string rbImgUrl;
  std::string rbWpUrl;
  std::string containerAdminName;

  yamlEnv.getPropertyString("rb_img_url", rbImgUrl);
  yamlEnv.getPropertyString("rb_wp_url", rbWpUrl);
  yamlEnv.getPropertyString("image_topic", imageTopic);
  yamlEnv.getPropertyString("waypoint_topic", wpTopic);
  yamlEnv.getPropertyInt("img_height", imgHeight);
  yamlEnv.getPropertyInt("img_width", imgWidth);
  yamlEnv.getPropertyInt("pool_size", poolSize);
  yamlEnv.getPropertyString("container_admin_name", containerAdminName);

  kpsr::high_performance::EventLoopMiddlewareProvider<CONTAINER_SIZE> adminEventLoop(nullptr);
  int adminPort;
  yamlEnv.getPropertyInt("server_admin_port", adminPort);
  kpsr::admin::socket_mdlw::EventLoopSocketAdminContainerProvider<CONTAINER_SIZE> adminProvider(adminPort,
                                                                                                adminEventLoop,
                                                                                                &yamlEnv,
                                                                                                containerAdminName);

  int systemPort;
  yamlEnv.getPropertyInt("server_system_port", systemPort);
  kpsr::admin::socket_mdlw::EventLoopSocketSystemContainerProvider<CONTAINER_SIZE> systemProvider(systemPort,
                                                                                                  adminEventLoop,
                                                                                                  &yamlEnv,
                                                                                                  containerAdminName);
  adminEventLoop.start();
  adminProvider.start();
  systemProvider.start();

  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(&adminProvider.getContainer());
  eventloop.start();

  spdlog::info("EventLoopSize: {}", EVENT_LOOP_SIZE);
  spdlog::info("rb_img_url: {}", rbImgUrl);
  spdlog::info("rb_wp_url: {}", rbWpUrl);
  spdlog::info("image_topic: {}", imageTopic);
  spdlog::info("waypoint_topic: {}", wpTopic);
  spdlog::info("pool_size: {}", poolSize);
  spdlog::info("server_admin_port: {}", adminPort);
  spdlog::info("server_system_port {}", systemPort);

  kpsr::vision_ocv::ImageDataFactory _factory(imgWidth, imgHeight, 0, "frame");

  zmq::context_t context(1);
  ZmqMiddlewareFacility zmf(context, rbImgUrl, rbWpUrl, imageTopic, wpTopic, eventloop, poolSize, _factory, &adminProvider.getContainer());

  mls::QueenBeeSvc queenBeeSvc(&yamlEnv, zmf._imageDataSubscriber, zmf._toZMQPublisher);
  adminProvider.getContainer().attach(&queenBeeSvc);   
  queenBeeSvc.startup();
  spdlog::info("QBApp: QueenBee Service started");

  while (true)
  {
    queenBeeSvc.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  queenBeeSvc.shutdown();

  systemProvider.stop();
  adminProvider.stop();
  adminEventLoop.stop();

  zmf.stop();

  return 0;
} // end main()