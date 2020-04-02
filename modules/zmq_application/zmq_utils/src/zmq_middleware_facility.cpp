
#include <zmq_middleware_facility.h>

ZmqMiddlewareFacility::ZmqMiddlewareFacility(zmq::context_t &context,
                                             std::string rbImgUrl, std::string rbWpUrl, std::string imgTopic, std::string wpTopic,
                                             kpsr::high_performance::EventLoopMiddlewareProvider<EVENT_LOOP_SIZE> &eventloop,
                                             int poolSize, kpsr::vision_ocv::ImageDataFactory &factory,
                                             kpsr::Container *container, int numPublishers)
    : _rbImgUrl(rbImgUrl)
    , _rbWpUrl(rbWpUrl)
    , _imgTopic(imgTopic)
    , _wpTopic(wpTopic)
    , _eventloop(eventloop)
    , _poolSize(poolSize)
    , _factory(factory)
    , _numPublishers(numPublishers)
    , _imageDataSubscriber(nullptr)
    , _imageDataPublisher(nullptr)
    , _binaryFromZMQProvider(nullptr)
    , _subscriber(context, ZMQ_SUB)
    , _fromZmqMiddlewareProvider()
    , _publishers(_numPublishers)
    , _toZMQPublishers(_numPublishers)
    , _toZMQMiddlewareProviders(_numPublishers)
{

  _subscriber.bind(_rbImgUrl);

  _subscriber.setsockopt(ZMQ_SUBSCRIBE, imgTopic.c_str(), imgTopic.size());

  _binaryFromZMQProvider = _fromZmqMiddlewareProvider
                               .getBinaryFromMiddlewareChannel<kpsr::vision_ocv::ImageData>(_subscriber, _poolSize);

  _binaryFromZMQProvider->start();
  _imageDataSubscriber =
      _eventloop.getSubscriber<kpsr::vision_ocv::ImageData>(_imgTopic);
  _imageDataPublisher =
      _eventloop.getPublisher<kpsr::vision_ocv::ImageData>(_imgTopic, _poolSize, _factory.initializerFunction, _factory.eventClonerFunction);

  _binaryFromZMQProvider->registerToTopic(_imgTopic, _imageDataPublisher);

  for (int i = 0; i < _publishers.size(); i++)
  {
    std::string fullWpUrl = _rbWpUrl+std::to_string(9002+(10*i));
    spdlog::info("fullWpUrl: {}", fullWpUrl);
    _publishers[i] = std::make_shared<zmq::socket_t>(context, ZMQ_PUB);
    _publishers[i].get()->bind(fullWpUrl);
    _toZMQMiddlewareProviders[i] = std::make_shared<kpsr::zmq_mdlw::ToZMQMiddlewareProvider>(container, *_publishers[i].get());
    _toZMQPublishers[i]._minIndex = 100000*(i+1);
    _toZMQPublishers[i]._maxIndex = 100000*(i+2)-1;
    _toZMQPublishers[i]._waypointPublisher = _toZMQMiddlewareProviders[i]->getBinaryToMiddlewareChannel<mls::Waypoint>(_wpTopic, 10);

  }
}

void ZmqMiddlewareFacility::stop()
{
  _binaryFromZMQProvider->stop();
  _binaryFromZMQProvider->unregisterFromTopic(_imgTopic);
}