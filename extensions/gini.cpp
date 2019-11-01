// custom-app.cpp

#include "gini.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/string.h"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"
#include "ns3/names.h"
// #inlcude <fstream>
NS_LOG_COMPONENT_DEFINE("GiniDetectionApp");
using namespace std;
namespace ns3 {
namespace ndn {
NS_OBJECT_ENSURE_REGISTERED(GiniDetectionApp);

// register NS-3 type
TypeId
GiniDetectionApp::GetTypeId()
{
  static TypeId tid = TypeId("GiniDetectionApp").SetParent<ndn::App>().AddConstructor<GiniDetectionApp>();
  return tid;
}

// Processing upon start of the application
void
GiniDetectionApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();
  mycout.open("results/gini.txt", ios::app);

  // Add entry to FIB for `/prefix/sub`
  // ndn::FibHelper::AddRoute(GetNode(), "/prefix/sub", m_face, 0);

  // Schedule send of first interest
  // Simulator::Schedule(Seconds(0.0), &GiniDetectionApp::GetPit, this);
  // Simulator::Schedule(Seconds(1.0), &GiniDetectionApp::CalcGini, this);
}

// Processing when application is stopped
void
GiniDetectionApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
  mycout.close();
}

// void
// GiniDetectionApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
// {
//   ndn::App::OnInterest(interest);
//   NS_LOG_DEBUG("Do nothing for incoming interest for" << interest->getName());
// }



void
GiniDetectionApp::GetPit()
{
  auto forwarder = GetNode()->GetObject<ndn::L3Protocol>()->getForwarder();
  auto begin = forwarder->getPit().begin();
  auto end = forwarder->getPit().end();
  size += forwarder->getPit().size();
  count ++;
  // NS_LOG_DEBUG("===== size of PIT is " << size << " =====");
  // set<Info> prefix;
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

  if(count == 1){
    double gini = 1.0;
    for(auto iter = prefix.begin(); iter != prefix.end(); iter ++){
      NS_LOG_DEBUG((*iter).prefix << " " << (*iter).num);
      gini -= double((*iter).num * (*iter).num) / (size * size);
    }
    prefix.clear();
    size = 0;
    count = 0;
    mycout << Simulator::Now().ToInteger (Time::S) << " " << ns3::Names::FindName (GetNode()) << " " << gini << endl;
    // NS_LOG_DEBUG(gini);
  }
  
  Simulator::Schedule(Seconds(1.0), &GiniDetectionApp::GetPit, this);

}


} // namespace ndn
} // namespace ns3