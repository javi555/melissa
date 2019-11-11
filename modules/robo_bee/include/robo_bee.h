#ifndef ROBOBEE_H
#define ROBOBEE_H

#include <stdio.h>
#include <iostream>
#include <vector>

namespace mls
{
class RoboBee
{
  public:
    RoboBee(std::string name);
    //kpsr::Subscriber<kpsr::geometry::PoseEventData> *poseSubscriber


    //-> destructors
    void capture();
    void sendImg();
    void onPoseReceived();
    void startCapturing();
    void stopCapturing();
    bool isCapturing();
  private:
    std::string _name;
    bool _capturing;
    int _currentImgId;
    //CV::Mat _img;
    std::string fileImagesPath;
    float _imgWidth;
    float _imgHeigh;
    bool _restartIfNoMoreImages;
    //kpsr::Subscriber<kpsr::geometry::PoseEventData> * _poseSubscriber;
};

} // namespace mls
#endif //ROBOBEE