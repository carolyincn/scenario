// custom-app.cpp

#include "detection.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/string.h"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

NS_LOG_COMPONENT_DEFINE("DetectionApp");
using namespace std;
namespace ns3 {
namespace ndn {
NS_OBJECT_ENSURE_REGISTERED(DetectionApp);

// register NS-3 type
TypeId
DetectionApp::GetTypeId()
{
  static TypeId tid = TypeId("DetectionApp").SetParent<ndn::App>().AddConstructor<DetectionApp>();
  return tid;
}

// Processing upon start of the application
void
DetectionApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/prefix/sub`
  // ndn::FibHelper::AddRoute(GetNode(), "/prefix/sub", m_face, 0);

  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.0), &DetectionApp::GetPit, this);
}

// Processing when application is stopped
void
DetectionApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
DetectionApp::GetPit()
{
  auto forwarder = GetNode()->GetObject<ndn::L3Protocol>()->getForwarder();
  auto begin = forwarder->getPit().begin();
  auto end = forwarder->getPit().end();
  int size = forwarder->getPit().size();
  // NS_LOG_DEBUG("===== size of PIT is " << size << " =====");
  set<Info> prefix;
  // set<int> num;
  for (auto iter = begin; iter != end; iter ++){
    auto interest = iter->getInterest().toUri().substr(0, 21);
    Info t = {interest};
    auto it = prefix.find(t);
    if(it == prefix.end()){
      Info temp = {interest, 1};
      prefix.insert(temp);
      // NS_LOG_DEBUG(interest);
    } else{
      (*it).num ++;
    }
    // NS_LOG_DEBUG("+++ in record start +++");
    // for (auto it = iter->in_begin(); it != iter->in_end(); it ++)
    //   NS_LOG_DEBUG(it->getInterest().toUri());
    // NS_LOG_DEBUG("+++ in record end +++");
  }
  double gini = 1.0;
  for(auto iter = prefix.begin(); iter != prefix.end(); iter ++){
    // NS_LOG_DEBUG((*iter).prefix << " " << (*iter).num);
    gini -= double((*iter).num * (*iter).num) / (size * size);
  }
  NS_LOG_DEBUG(gini);
  Simulator::Schedule(Seconds(1.0), &DetectionApp::GetPit, this);

}


} // namespace ndn
} // namespace ns3