// ndn-ddos-scenario.cpp

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"
#include "ns3/node.h"
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-scenario-helper.hpp"
#include "ns3/ndnSIM/model/ndn-l3-protocol.hpp"
#include "ns3/names.h"
#include "ns3/net-device.h"

namespace ns3 {
// ofstream outfile;
void PrintTime (Time next, Ptr<Node> node)
{
  std::cout << "=== " << Simulator::Now ().ToDouble (Time::S) << "s ===" << std::endl;

  nfd::pit::Pit::const_iterator begin = node->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().begin();
  nfd::pit::Pit::const_iterator end = node->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().end();
  std::cout << node->GetObject<ndn::L3Protocol>()->getForwarder()->getPit().size() << std::endl;
  for (nfd::pit::Pit::const_iterator iter = begin; iter != end; iter ++)
    std::cout << iter->getInterest().toUri() << std::endl;
  Simulator::Schedule (next, PrintTime, next, node);
}
int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));


  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes
  // Ptr<Node> node = CreateObject<Node>();


  // Creating nodes

  AnnotatedTopologyReader topologyReader("", 1);
  topologyReader.SetFileName("/home/yin/Desktop/ndnSIM/scenario/scenarios/topo-large.txt");
  topologyReader.Read();

  // NodeContainer nodes;
  // nodes.Create(2);

  // Connecting nodes using two links
  // PointToPointHelper p2p;
  // p2p.Install(nodes.Get(0), nodes.Get(1));
  // p2p.Install(nodes.Get(1), nodes.Get(2));
  

  // ndnHelper.setCsSize(1);
  // ndnHelper.Install(c1);

  // Choosing forwarding strategy
  // ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/multicast");
  
  // Ptr<Node> c1 = Names::Find<Node>("C1");
  // Ptr<Node> c2 = Names::Find<Node>("C2");
  Ptr<Node> p1 = Names::Find<Node>("P1");
  Ptr<Node> p2 = Names::Find<Node>("P2");
  Ptr<Node> r1 = Names::Find<Node>("R1");
  Ptr<Node> r2 = Names::Find<Node>("R2");
  Ptr<Node> r3 = Names::Find<Node>("R3");
  Ptr<Node> r4 = Names::Find<Node>("R4");
  Ptr<Node> r5 = Names::Find<Node>("R5");
  Ptr<Node> r6 = Names::Find<Node>("R6");
  Ptr<Node> r7 = Names::Find<Node>("R7");
  Ptr<Node> h1 = Names::Find<Node>("H1");
  Ptr<Node> h2 = Names::Find<Node>("H2");
  Ptr<Node> h3 = Names::Find<Node>("H3");
  Ptr<Node> h4 = Names::Find<Node>("H4");
  Ptr<Node> h5 = Names::Find<Node>("H5");
  Ptr<Node> h6 = Names::Find<Node>("H6");





  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  // ndnHelper.SetDefaultRoutes(false);

 //  NodeContainer allOtherNodes;
	// for (NodeList::Iterator i = NodeList::Begin(); i != NodeList::End(); ++i) {
	// 	if (*i != c1) {
	// 		allOtherNodes.Add(*i);
 //  	}
	// }
	// ndnHelper1.Install(allOtherNodes);
  ndnHelper.setCsSize(1);
  // ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/my-strategy");
  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");


  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper2;
  ndnGlobalRoutingHelper2.Install(r1);
  ndnGlobalRoutingHelper2.Install(r2);
  ndnGlobalRoutingHelper2.Install(r3);
  ndnGlobalRoutingHelper2.Install(r4);
  ndnGlobalRoutingHelper2.Install(r5);
  ndnGlobalRoutingHelper2.Install(r6);
  ndnGlobalRoutingHelper2.Install(r7);
  ndnGlobalRoutingHelper2.Install(h1);
  ndnGlobalRoutingHelper2.Install(h2);
  ndnGlobalRoutingHelper2.Install(h3);
  ndnGlobalRoutingHelper2.Install(h4);
  ndnGlobalRoutingHelper2.Install(h5);
  ndnGlobalRoutingHelper2.Install(h6);
  ndnGlobalRoutingHelper2.Install(p1);
  ndnGlobalRoutingHelper2.Install(p2);
  ndnGlobalRoutingHelper2.AddOrigin("/p1", p1);
  ndnGlobalRoutingHelper2.AddOrigin("/p2", p2);
  ndnGlobalRoutingHelper2.CalculateRoutes();

  
  // normal
  ndn::AppHelper h1_app("ns3::ndn::ConsumerZipfMandelbrot"); // ConsumerZipfMandelbrot
  h1_app.SetPrefix("/p1/good");
  h1_app.SetAttribute("Frequency", StringValue("50"));
  h1_app.Install(h1);
  // h1_app.Install(h2);
  h1_app.Install(h3);
  // h1_app.Install(h4);
  h1_app.Install(h5);
  // h1_app.Install(h6);

  // ndn::AppHelper h3_app("ns3::ndn::ConsumerZipfMandelbrot");
  // h3_app.SetPrefix("/p2/good");
  // h3_app.SetAttribute("Frequency", StringValue("5"));
  // h3_app.Install(h3);
  // h1_app.Install(h1);
  // h1_app.Install(h5);

  // evil
  ApplicationContainer evilApps;

  ndn::AppHelper h2_app("ns3::ndn::ConsumerCbr");
  h2_app.SetPrefix("/p1/evil");
  h2_app.SetAttribute("Frequency", StringValue("100"));
  // h2_app.Install(h2);
  evilApps = h2_app.Install(h2);

  evilApps.Start(Seconds(20.0));
  evilApps.Stop(Seconds(40.0));

  ndn::AppHelper h5_app("ns3::ndn::ConsumerCbr");
  h5_app.SetPrefix("/p1/evil");
  h5_app.SetAttribute("Frequency", StringValue("100"));
  // h5_app.Install(h5);
  evilApps = h5_app.Install(h4);
  evilApps.Start(Seconds(20.0));
  evilApps.Stop(Seconds(40.0));

  ndn::AppHelper h6_app("ns3::ndn::ConsumerCbr");
  h6_app.SetPrefix("/p1/evil");
  h6_app.SetAttribute("Frequency", StringValue("100"));
  evilApps = h6_app.Install(h6);

  evilApps.Start(Seconds(20.0));
  evilApps.Stop(Seconds(40.0));
  // // 
  ndn::AppHelper p1_app("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  p1_app.SetPrefix("/p1/good");
  p1_app.SetAttribute("PayloadSize", StringValue("1024"));
  p1_app.Install(p1); // last node  

  ndn::AppHelper p2_app("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  p1_app.SetPrefix("/p2/good");
  p1_app.SetAttribute("PayloadSize", StringValue("1024"));
  p1_app.Install(p2); // last node  

  ndn::AppHelper r1_app("DetectionApp");
  // Producer will reply to all requests starting with /prefix
  // p1_app.SetPrefix("/p2/good");
  // p1_app.SetAttribute("PayloadSize", StringValue("1024"));
  r1_app.Install(r1); // last node 


  Simulator::Stop(Seconds(50.0));
  ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(1.0));
  // Simulator::Schedule (Seconds (1.0), PrintTime, Seconds (0.1), r1);

  Simulator::Run();
  Simulator::Destroy();
  // outfile.close();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}
