#ifndef ZMQMIDDLEWAREFACILITY_H
#define ZMQMIDDLEWAREFACILITY_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>
#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/vision_ocv/image_data_factory.h>

#include <klepsydra/serialization/binary_cereal_mapper.h>
#include <klepsydra/serialization/json_cereal_mapper.h>
#include <klepsydra/zmq_vision_ocv/image_event_data_zmq_mapper.h>

class ZmqMiddlewareFacility {
public:
    ZmqMiddlewareFacility(zmq::context_t &context, std::string rbUrl, std::string topic, kpsr::high_performance::EventLoopMiddlewareProvider<16> &eventloop, int poolSize);

    ~ZmqMiddlewareFacility(){};

    void stop();

    kpsr::Subscriber<kpsr::vision_ocv::ImageData> *_imageDataSubscriber;
    kpsr::Publisher<kpsr::vision_ocv::ImageData> *_imageDataPublisher;
    kpsr::zmq_mdlw::FromZmqChannel<Base> *_binaryFromZMQProvider;
    kpsr::zmq_mdlw::FromZmqMiddlewareProvider _fromZmqMiddlewareProvider;
    
    zmq::socket_t _subscriber;
    kpsr::vision_ocv::ImageDataFactory _factory;

    zmq::context_t &_context;
    std::string _rbUrl;
    std::string _topic;
    int _poolSize;
    kpsr::high_performance::EventLoopMiddlewareProvider<16> &_eventloop;
};
#endif // ZMQMIDDLEWAREFACILITY_H