
#include <zmq_middleware_facility.h>

ZmqMiddlewareFacility::ZmqMiddlewareFacility(zmq::context_t &context,
                                             std::string rbImgUrl, std::string rbWpUrl, std::string imgTopic, std::string wpTopic,
                                             kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> &eventloop,
                                             int poolSize, kpsr::vision_ocv::ImageDataFactory &factory,
                                             kpsr::Container *container)
    : _rbImgUrl(rbImgUrl)
    , _rbWpUrl(rbWpUrl)
    , _imgTopic(imgTopic)
    , _wpTopic(wpTopic)
    , _eventloop(eventloop)
    , _poolSize(poolSize)
    , _factory(factory)
    , _imageDataSubscriber(nullptr)
    , _imageDataPublisher(nullptr)
    , _waypointSubscriber(nullptr)
    , _waypointPublisher(nullptr)
    , _binaryFromZMQProvider(nullptr)
    , _toZMQPublisher(nullptr)
    , _subscriber(context, ZMQ_SUB)
    , _publisher(context, ZMQ_PUB)
    , _fromZmqMiddlewareProvider()
    , _toZMQMiddlewareProvider(container, _publisher)
{
  _subscriber.connect(_rbImgUrl);

  _subscriber.setsockopt(ZMQ_SUBSCRIBE, imgTopic.c_str(), imgTopic.size());

  _binaryFromZMQProvider = _fromZmqMiddlewareProvider
                               .getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(_subscriber, _poolSize);

  _binaryFromZMQProvider->start();
  _imageDataSubscriber =
      _eventloop.getSubscriber<kpsr::vision_ocv::ImageData>(_imgTopic);
  _imageDataPublisher =
      _eventloop.getPublisher<kpsr::vision_ocv::ImageData>(_imgTopic, _poolSize, _factory.initializerFunction, _factory.eventClonerFunction);

  _binaryFromZMQProvider->registerToTopic(_imgTopic, _imageDataPublisher);

  _publisher.bind(_rbWpUrl);

  _toZMQPublisher = _toZMQMiddlewareProvider
                        .getBinaryToMiddlewareChannel<mls::Waypoint>(_wpTopic, 10);
}

void ZmqMiddlewareFacility::stop()
{
  _binaryFromZMQProvider->stop();
  _binaryFromZMQProvider->unregisterFromTopic(_imgTopic);
}