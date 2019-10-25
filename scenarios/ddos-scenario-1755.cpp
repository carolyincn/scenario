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
#include "ns3/rng-seed-manager.h"
#include "ns3/random-variable-stream.h"

using namespace std;
namespace ns3 {
// ofstream outfile;

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
  topologyReader.SetFileName("/home/yin/Desktop/ndnSIM/scenario/scenarios/topo-1755.txt");
  topologyReader.Read();
  
  ndn::StackHelper ndnHelper;
  ndnHelper.setCsSize(1);
  ndnHelper.Install(topologyReader.GetNodes());
  ndn::GlobalRoutingHelper grouter;
  grouter.Install(topologyReader.GetNodes());

  NodeContainer leaves;
  NodeContainer gw;
  NodeContainer bb;

  int leafnum = 0;
  int gwnum = 0;
  int bbnum = 0;

  for_each (NodeList::Begin (), NodeList::End (), [&] (Ptr<Node> node) {
    if (Names::FindName (node).compare (0, 5, "leaf-")==0)
      {
        leaves.Add (node);
        // cout << Names::FindName (node) << endl;
        leafnum ++;
      }
    else if (Names::FindName (node).compare (0, 3, "gw-")==0)
      {
        gw.Add (node);
        // cout << Names::FindName (node) << endl;
        gwnum ++;
      }
    else if (Names::FindName (node).compare (0, 3, "bb-")==0)
      {
        bb.Add (node);
        // cout << Names::FindName (node) << endl;
        bbnum ++;
      }
  });

  NodeContainer producerNodes;
  NodeContainer evilNodes;
  NodeContainer goodNodes;

  set< Ptr<Node> > producers;
  set< Ptr<Node> > evils;
  set< Ptr<Node> > angels;

  int badCount = 10;
  int prodCount = 10;
  while (evils.size () < badCount){
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    Ptr<Node> node = leaves.Get (rand->GetInteger(0, leaves.GetN ()));

    if (evils.find (node) != evils.end ())
      continue;
    evils.insert (node);
    // cout << Names::FindName(node) << endl;
    string name = Names::FindName(node);
    Names::Rename (name, "evil-"+name);
  }

  for_each(leaves.Begin(), leaves.End(), [&] (Ptr<Node> node){
    if (evils.find(node) == evils.end()){
      angels.insert(node);
      string name = Names::FindName(node);
      Names::Rename (name, "good-"+name);
    }
  });

  while (producers.size () < prodCount){
    Ptr<Node> node = 0;
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    node = gw.Get (rand->GetInteger(0, leaves.GetN ()));
    producers.insert (node);
    string name = Names::FindName(node);
    Names::Rename (name, "prod-"+name);
  }


  //ofstream os (meta_file.c_str(), ios_base::out | ios_base::trunc);
  for_each (evils.begin (), evils.end (), [&] (Ptr<Node> node){
    string name = Names::FindName (node);
    cout << name << " ";
    evilNodes.Add (node);
  });
  cout << endl;

  for_each (angels.begin (), angels.end (), [&] (Ptr<Node> node){
    string name = Names::FindName (node);
    cout << name << " ";
    goodNodes.Add (node);
  });
  cout << endl;

  for_each (producers.begin (), producers.end (), [&] (Ptr<Node> node){
    string name = Names::FindName (node);
    cout << name << " ";
    producerNodes.Add (node);
  });
  cout << endl;

  ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  grouter.AddOrigins ("/p", producerNodes);
  grouter.CalculateRoutes ();

  // evil traffic
  ndn::AppHelper evilAppHelper("ns3::ndn::ConsumerCbr"); // ConsumerZipfMandelbrot
  // evilAppHelper.SetPrefix("/evil");
  evilAppHelper.SetAttribute("Frequency", StringValue("50"));
  for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++){
    ApplicationContainer evilApp;
    evilAppHelper.SetPrefix ("/p/evil/"+Names::FindName (*node));
    evilApp.Add (evilAppHelper.Install (*node));
    evilApp.Start (Seconds (10.0));
    evilApp.Stop (Seconds (20.0));
  }

  // normal traffic
  ndn::AppHelper goodAppHelper("ns3::ndn::ConsumerZipfMandelbrot"); // ConsumerZipfMandelbrot
  goodAppHelper.SetAttribute("Frequency", StringValue("10"));
  for (NodeContainer::Iterator node = goodNodes.Begin (); node != goodNodes.End (); node++){
    ApplicationContainer goodApp;
    goodAppHelper.SetPrefix ("/p/good/"+Names::FindName (*node));
    // goodAppHelper.SetAttribute ("AvgGap", TimeValue (Seconds (1.100 / maxNonCongestionShare)));
    goodApp.Add (goodAppHelper.Install (*node));
    // UniformVariable rand (0, 1);
    // goodApp.Start (Seconds (0.0) + Time::FromDouble (rand.GetValue (), Time::S));
  }

  ndn::AppHelper producerApp("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerApp.SetPrefix("/p/good");
  producerApp.SetAttribute("PayloadSize", StringValue("1024"));
  producerApp.Install(producerNodes); // last node  

  ndn::AppHelper detectionApp("DetectionApp");
  detectionApp.Install(gw.Get(0)); // last node 

  Simulator::Stop(Seconds(20.0));
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