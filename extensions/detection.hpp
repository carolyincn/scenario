// detection.hpp

#ifndef DETECTION_APP_H_
#define DETECTION_APP_H_

#include "ns3/ndnSIM/apps/ndn-app.hpp"
#include "ns3/string.h"
#include "ns3/ptr.h"

using namespace std;

namespace ns3 {
namespace ndn {
class DetectionApp : public ndn::App {
public:
  // register NS-3 type "DetectionApp"
  static TypeId
  GetTypeId();

  // (overridden from ndn::App) Processing upon start of the application
  virtual void
  StartApplication();

  // (overridden from ndn::App) Processing when application is stopped
  virtual void
  StopApplication();

  struct Info {
    string prefix;
    mutable int num;
    bool operator < (const Info &a) const{
      return a.prefix < prefix;
    }
  };

  // // (overridden from ndn::App) Callback that will be called when Interest arrives
  // virtual void
  // OnInterest(std::shared_ptr<const ndn::Interest> interest);

  // // (overridden from ndn::App) Callback that will be called when Data arrives
  // virtual void
  // OnData(std::shared_ptr<const ndn::Data> contentObject);

private:
  void
  GetPit();

  void
  Detect();

  


};
} // namespace ndn
} // namespace ns3

#endif // DETECTION_APP_H_