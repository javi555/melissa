#include <iostream>
#include "robo_bee_svc.h"
#include "waypoint_zmq_serializer.h"
#include "waypoint.h"
#include <klepsydra/core/yaml_environment.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/serialization/json_cereal_mapper.h>
#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>
#include "config.h"

int main(int argc, char **argv)
{
  std::string yamlFile = argv[1];
  kpsr::YamlEnvironment yamlEnv(yamlFile);

  int imgWidth;
  int imgHeight;
  int roboBeePrefix;
  std::string imageTopic;
  std::string wpTopic;
  std::string fileImagesPath;
  int poolSize;
  std::string qbImgUrl;
  std::string qbWpUrl;
  std::string containerAdminName;

  yamlEnv.getPropertyString("qb_img_url", qbImgUrl);
  yamlEnv.getPropertyString("qb_wp_url", qbWpUrl);
  yamlEnv.getPropertyInt("img_width", imgWidth);
  yamlEnv.getPropertyInt("img_height", imgHeight);
  yamlEnv.getPropertyInt("robo_bee_prefix", roboBeePrefix);
  yamlEnv.getPropertyString("image_topic", imageTopic);
  yamlEnv.getPropertyString("waypoint_topic", wpTopic);
  yamlEnv.getPropertyString("file_images_path", fileImagesPath);
  yamlEnv.getPropertyInt("pool_size", poolSize);
  yamlEnv.getPropertyString("container_admin_name", containerAdminName);

  kpsr::high_performance::EventLoopMiddlewareProvider<128> adminEventLoop(nullptr);
  int adminPort;
  yamlEnv.getPropertyInt("server_admin_port", adminPort);
  kpsr::admin::socket_mdlw::EventLoopSocketAdminContainerProvider<128> adminProvider(adminPort,
                                                                                     adminEventLoop,
                                                                                     &yamlEnv,
                                                                                     containerAdminName);

  int systemPort;
  yamlEnv.getPropertyInt("server_system_port", systemPort);
  kpsr::admin::socket_mdlw::EventLoopSocketSystemContainerProvider<128> systemProvider(systemPort,
                                                                                       adminEventLoop,
                                                                                       &yamlEnv,
                                                                                       containerAdminName);
  adminEventLoop.start();
  adminProvider.start();
  systemProvider.start();

  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloop(&adminProvider.getContainer());
  eventloop.start();

  spdlog::info("qb_img_url: {}", qbImgUrl);
  spdlog::info("qb_wp_url: {}", qbWpUrl);
  spdlog::info("RBApp: width: {}", imgWidth);
  spdlog::info("RBApp: height: {}", imgHeight);
  spdlog::info("RBApp: robo_bee_prefix: {}", roboBeePrefix);
  spdlog::info("image_topic: {}", imageTopic);
  spdlog::info("waypoint_topic: {}", wpTopic);
  spdlog::info("file_images_path: {}", fileImagesPath);
  spdlog::info("pool_size: {}", poolSize);
  spdlog::info("server_admin_port: {}", adminPort);
  spdlog::info("server_system_port {}", systemPort);

  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  publisher.connect(qbImgUrl);

  kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZMQMiddlewareProvider(&adminProvider.getContainer(), publisher);

  kpsr::Publisher<kpsr::vision_ocv::ImageData> *toZMQPublisher = toZMQMiddlewareProvider
                                                                     .getBinaryToMiddlewareChannel<kpsr::vision_ocv::ImageData>(imageTopic, 10);

  zmq::context_t contextWP(2);
  zmq::socket_t subscriber(contextWP, ZMQ_SUB);
  kpsr::zmq_mdlw::FromZmqChannel<Base> *binaryFromZMQProvider;
  kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
  kpsr::Subscriber<mls::Waypoint> *waypointSubscriber;
  kpsr::Publisher<mls::Waypoint> *waypointPublisher;

  kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> eventloopWP(&adminProvider.getContainer());
  eventloopWP.start();

  subscriber.connect(qbWpUrl);

  subscriber.setsockopt(ZMQ_SUBSCRIBE, wpTopic.c_str(), wpTopic.size());

  binaryFromZMQProvider = fromZmqMiddlewareProvider
                              .getBinaryFromMiddlewareChannel<mls::Waypoint>(subscriber, poolSize);

  binaryFromZMQProvider->start();
  waypointSubscriber =
      eventloopWP.getSubscriber<mls::Waypoint>(wpTopic);
  waypointPublisher =
      eventloopWP.getPublisher<mls::Waypoint>(wpTopic, poolSize, nullptr, nullptr);

  binaryFromZMQProvider->registerToTopic(wpTopic, waypointPublisher);

  mls::RoboBeeSvc roboBeeSvc(&yamlEnv, toZMQPublisher, waypointSubscriber,
                             imgWidth, imgHeight, fileImagesPath, true, roboBeePrefix);
  adminProvider.getContainer().attach(&roboBeeSvc);
  roboBeeSvc.startup();
  spdlog::info("RBApp: RoboBee Service started");

  while (true)
  {
    roboBeeSvc.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  roboBeeSvc.shutdown();

  return 0;
} // end main()