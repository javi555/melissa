
#include <zmq_middleware_facility.h>

ZmqMiddlewareFacility::ZmqMiddlewareFacility(zmq::context_t &context, std::string rbUrl, std::string topic, kpsr::high_performance::EventLoopMiddlewareProvider<16> &eventloop, int poolSize)
{
    zmq::socket_t subscriber(context, ZMQ_SUB);

    subscriber.connect(rbUrl);

    subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

    kpsr::zmq_mdlw::FromZmqMiddlewareProvider _fromZmqMiddlewareProvider;
    _binaryFromZMQProvider = _fromZmqMiddlewareProvider
                                 .getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(subscriber, 10);

    kpsr::vision_ocv::ImageDataFactory factory(320, 240, 10, "body");

    _binaryFromZMQProvider->start();
    imageDataSubscriber =
        eventloop.getSubscriber<kpsr::vision_ocv::ImageData>(topic);
    imageDataPublisher =
        eventloop.getPublisher<kpsr::vision_ocv::ImageData>(topic, poolSize, factory.initializerFunction, factory.eventClonerFunction);

    _binaryFromZMQProvider->registerToTopic(topic, imageDataPublisher);
}

void ZmqMiddlewareFacility::stop(std::string topic)
{
    _binaryFromZMQProvider->stop();
    _binaryFromZMQProvider->unregisterFromTopic(topic);
}