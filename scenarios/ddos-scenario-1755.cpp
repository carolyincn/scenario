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
#include <fstream>
using namespace std;
namespace ns3 {
// ofstream outfile;

void PrintTime (Time next)
{
  std::cout << "=== " << Simulator::Now ().ToDouble (Time::S) << "s ===" << std::endl;
  Simulator::Schedule (next, PrintTime, next);
}

int
main(int argc, char* argv[])
{
  // setting default parameters for PointToPoint links and channels
  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::QueueBase::MaxPackets", StringValue("20"));
  // Config::SetDefault("ns3::QueueBase::MaxSize", StringValue("20p"));

  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);

  // Creating nodes

  AnnotatedTopologyReader topologyReader("", 1);
  // topologyReader.SetFileName("/home/yin/Desktop/ndn/ndnSIM/scenario/scenarios/topo-1755.txt");
  topologyReader.SetFileName("/home/yin/Desktop/ndn/ndnSIM/scenario/scenarios/topo-tree.txt");
  // topologyReader.SetFileName("/home/yin/Desktop/ndnSIM/scenario/scenarios/topo-small.txt");
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

  int badCount = 7;
  int prodCount = bbnum;
  while (evils.size () < badCount){
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    Ptr<Node> node = leaves.Get (rand->GetInteger(0, leafnum - 1));

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
      Names::Rename (name, "good-" + name);
    }
  });

  while (producers.size () < prodCount){
    Ptr<Node> node = 0;
    Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
    node = bb.Get (rand->GetInteger(0, bbnum - 1));
    if (producers.find (node) != producers.end ())
      continue;
    producers.insert (node);
    string name = Names::FindName(node);
    Names::Rename (name, "prod-" + name);
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
// 
  // ndn::StrategyChoiceHelper::Install(leaves, "/", "/localhost/nfd/strategy/best-route");
  // ndn::StrategyChoiceHelper::Install(bb, "/", "/localhost/nfd/strategy/best-route");
  // // ndn::StrategyChoiceHelper::Install(gw, "/", "/localhost/nfd/strategy/cusum-strategy");
  // ndn::StrategyChoiceHelper::Install(gw, "/", "/localhost/nfd/strategy/best-route");

  grouter.AddOrigins ("/p", producerNodes);
  grouter.CalculateRoutes ();

  // evil traffic
  ndn::AppHelper evilAppHelper("ns3::ndn::ConsumerCbr"); // ConsumerZipfMandelbrot
  evilAppHelper.SetAttribute("Frequency", StringValue("200"));
  evilAppHelper.SetAttribute("Randomize", StringValue("uniform"));
  for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++){
    ApplicationContainer evilApp;
    evilAppHelper.SetPrefix ("/p/evil/");
    evilApp.Add (evilAppHelper.Install (*node));
    evilApp.Start (Seconds (120.0));
    evilApp.Stop (Seconds (240.0));
  }

  // ndn::AppHelper evilAppHelper1("ns3::ndn::ConsumerCbr"); // ConsumerZipfMandelbrot
  // evilAppHelper1.SetAttribute("Frequency", StringValue("20"));
  // evilAppHelper1.SetAttribute("Randomize", StringValue("uniform"));
  // for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++){
  //   ApplicationContainer evilApp;
  //   evilAppHelper1.SetPrefix ("/p/even/");
  //   evilApp.Add (evilAppHelper1.Install (*node));
  //   evilApp.Start (Seconds (120.0));
  //   evilApp.Stop (Seconds (240.0));
  // }
  // for(int i = 120; i < 240; i ++){
  //   Ptr<NormalRandomVariable> rand = CreateObject<NormalRandomVariable>();
  //   evilAppHelper.SetAttribute("Frequency", StringValue(to_string(rand->GetInteger(20, 5, 50))));
  //   for (NodeContainer::Iterator node = evilNodes.Begin (); node != evilNodes.End (); node++){
  //     ApplicationContainer evilApp;
  //     evilAppHelper.SetPrefix ("/p/evil");
  //     evilApp.Add (evilAppHelper.Install (*node));
  //     evilApp.Start (Seconds (i));
  //     evilApp.Stop (Seconds (i + 1));
  //   }
  // }

  ndn::AppHelper goodAppHelper("ns3::ndn::ConsumerZipfMandelbrot"); // ConsumerZipfMandelbrot
  goodAppHelper.SetAttribute("Frequency", StringValue("10"));
  for (NodeContainer::Iterator node = leaves.Begin (); node != leaves.End (); node++){
    ApplicationContainer goodApp;
    goodAppHelper.SetPrefix ("/p/good/");
    goodApp.Add (goodAppHelper.Install (*node));
  }
  
  // for(int i = 0; i < 150; i ++){
  //   Ptr<ExponentialRandomVariable> rand = CreateObject<ExponentialRandomVariable>();
  //   cout << rand->GetInteger(9,40) + 1 << ",";
  // }
  // cout << endl;
  // for(int i = 0; i < 300; i ++){
  //   // Ptr<NormalRandomVariable> rand = CreateObject<NormalRandomVariable>();
  //   // goodAppHelper.SetAttribute("Frequency", StringValue(to_string(rand->GetInteger(10, 5, 50))));
  //   Ptr<ExponentialRandomVariable> rand = CreateObject<ExponentialRandomVariable>();
  //   goodAppHelper.SetAttribute("Frequency", StringValue(to_string(rand->GetInteger(20, 50) + 1)));
  //   // goodAppHelper.SetAttribute("Frequency", StringValue("10"));
  //   for (NodeContainer::Iterator node = leaves.Begin (); node != leaves.End (); node++){
  //     ApplicationContainer goodApp;
  //     goodAppHelper.SetPrefix ("/p/good/rand");
  //     goodApp.Add (goodAppHelper.Install (*node));
  //     goodApp.Start (Seconds (i));
  //     goodApp.Stop (Seconds (i + 1));
  //   }
  // }

  // ndn::AppHelper randAppHelper("ns3::ndn::ConsumerZipfMandelbrot"); // ConsumerZipfMandelbrot
  // randAppHelper.SetAttribute("Frequency", StringValue("50"));
  // randAppHelper.SetPrefix ("/p/good/rand");
  // for(int i = 0; i < 150; i ++){
  //   Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  //   ApplicationContainer goodApp;
  //   goodApp.Add (randAppHelper.Install (leaves.Get(rand->GetInteger(0, 15))));
  //   goodApp.Start (Seconds (i));
  //   goodApp.Stop (Seconds (i + 1));
  // }
  

  ndn::AppHelper producerApp("ns3::ndn::Producer");
  // Producer will reply to all requests starting with /prefix
  producerApp.SetPrefix("/p/good");
  producerApp.SetAttribute("PayloadSize", StringValue("1024"));
  producerApp.Install(producerNodes); // last node  

  // ndn::AppHelper GiniDetectionApp("GiniDetectionApp");
  // GiniDetectionApp.Install(gw); // last node 

  Simulator::Stop(Seconds(300.0));
  ndn::L3RateTracer::InstallAll("results/rate.txt", Seconds(1.0));
  ofstream mycout("results/index.txt", ios::out);
  mycout << "Time Node entropy gini t05 enrate ginirate t05rate apa cusum m_cusum" << endl;
  mycout.close();

  // Simulator::Schedule (Seconds (1.0), PrintTime, Seconds (1.0));

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
