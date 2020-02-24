#ifndef ZMQMIDDLEWAREFACILITY_H
#define ZMQMIDDLEWAREFACILITY_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/vision_ocv/image_data_factory.h>
#include <klepsydra/socket_interface/hp_socket_admin_container_provider.h>
#include <klepsydra/socket_interface/hp_socket_system_container_provider.h>

#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/serialization/json_cereal_mapper.h>
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>
#include <klepsydra/core/yaml_environment.h>

#include "waypoint_zmq_serializer.h"
#include "waypoint.h"
#include "queen_bee_svc.h"
#include "config.h"

class ZmqMiddlewareFacility
{
public:
    ZmqMiddlewareFacility(zmq::context_t &context, std::string rbImgUrl, std::string rbWpUrl, std::string imgTopic, std::string wpTopic,
                          kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> &eventloop, int poolSize,
                          kpsr::vision_ocv::ImageDataFactory &factory, kpsr::Container *container, int numPublishers);

    ~ZmqMiddlewareFacility(){};

    void stop();

    std::string _rbImgUrl;
    std::string _rbWpUrl;
    std::string _imgTopic;
    std::string _wpTopic;
    kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> &_eventloop;
    int _poolSize;
    int _numPublishers;
    kpsr::vision_ocv::ImageDataFactory &_factory;

    zmq::socket_t _subscriber;
    std::vector<std::shared_ptr<zmq::socket_t>> _publishers;
    kpsr::Subscriber<kpsr::vision_ocv::ImageData> *_imageDataSubscriber;
    kpsr::Publisher<kpsr::vision_ocv::ImageData> *_imageDataPublisher;
    kpsr::zmq_mdlw::FromZmqChannel<Base> *_binaryFromZMQProvider;
    std::vector<mls::WpPublisherConfig> _toZMQPublishers;
    kpsr::zmq_mdlw::FromZmqMiddlewareProvider _fromZmqMiddlewareProvider;
    std::vector<std::shared_ptr<kpsr::zmq_mdlw::ToZMQMiddlewareProvider>> _toZMQMiddlewareProviders;
};
#endif // ZMQMIDDLEWAREFACILITY_H