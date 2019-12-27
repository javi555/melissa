
#include <zmq_middleware_facility.h>

ZmqMiddlewareFacility::ZmqMiddlewareFacility(zmq::context_t &context,
                                             std::string rbUrl, std::string topic,
                                             kpsr::high_performance::EventLoopMiddlewareProvider<16> &eventloop,
                                             int poolSize, kpsr::vision_ocv::ImageDataFactory &factory)
    : _rbUrl(rbUrl),
      _topic(topic),
      _eventloop(eventloop),
      _poolSize(poolSize), _imageDataSubscriber(nullptr),
      _imageDataPublisher(nullptr),
      _binaryFromZMQProvider(nullptr), _fromZmqMiddlewareProvider(),
      _subscriber(context, ZMQ_SUB),
      _factory(factory)
{
    _subscriber.connect(_rbUrl);

    _subscriber.setsockopt(ZMQ_SUBSCRIBE, _topic.c_str(), _topic.size());

    _binaryFromZMQProvider = _fromZmqMiddlewareProvider
                                 .getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(_subscriber, _poolSize);

    _binaryFromZMQProvider->start();
    _imageDataSubscriber =
        eventloop.getSubscriber<kpsr::vision_ocv::ImageData>(_topic);
    _imageDataPublisher =
        eventloop.getPublisher<kpsr::vision_ocv::ImageData>(_topic, _poolSize, _factory.initializerFunction, _factory.eventClonerFunction);

    _binaryFromZMQProvider->registerToTopic(_topic, _imageDataPublisher);
}

void ZmqMiddlewareFacility::stop()
{
    _binaryFromZMQProvider->stop();
    _binaryFromZMQProvider->unregisterFromTopic(_topic);
}