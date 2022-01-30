// this file was created for 
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/flow-monitor-module.h"
#include <string>
#include <iostream>
using namespace ns3;
using namespace std;
int experimenttype=0; 
  //0:bandwidth step change and uniformity;1:progation delay step change
void   Bandwidthchange(string s)
{
Config::Set("/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/DataRate", StringValue(s) );
}
void Delaychange(string s)
{
Config::Set("/ChannelList/0/$ns3::PointToPointChannel/Delay", StringValue(s) );
}
void Change(string s1,string s2)
{
if(experimenttype==0) 
Bandwidthchange(s1);
else   
Delaychange(s2); 
}
// Trace congestion window
static void CwndTracer (Ptr<OutputStreamWrapper> stream, uint32_t oldval, uint32_t newval)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " " << newval / 1448.0 << std::endl;
}

void TraceCwnd (uint32_t nodeId, uint32_t socketId,string dir)
{
  AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (dir + "/cwnd.dat");
  Config::ConnectWithoutContext ("/NodeList/" + std::to_string (nodeId) + "/$ns3::TcpL4Protocol/SocketList/" + std::to_string (socketId) + "/CongestionWindow", MakeBoundCallback (&CwndTracer, stream));
}
int main (int argc, char *argv [])
{
 // Naming the output directory using local system time
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer, sizeof (buffer), "%d-%m-%Y-%I-%M-%S", timeinfo);
  std::string currentTime (buffer);
  std::string tcpTypeId = "TcpBbr";
  std::string queueDisc = "FifoQueueDisc";
  std::string preDelay="90ms",postDelay="30ms";
  std::string preBdw="3Mbps",postBdw="22Mbps";
  std::string packetbuffer="20p";
  uint32_t delAckCount = 2;
  string filename;
  string trace_file;
  bool enablePcap = true;
  std::string dir = "test-results/"+ currentTime + "/";
  Time stopTime = Seconds (200); //
  CommandLine cmd (__FILE__);
  cmd.AddValue ("experimenttype", "0:bandwidth step change and uniformity 1:progation delay step change", experimenttype);
  cmd.AddValue ("trace_file", "xxxx.txt", trace_file);
  cmd.AddValue ("filename", "xxxx.pcap", filename);
  if (experimenttype==0)
   {
       cmd.AddValue("delay","Delay: xx ms(eg. 30ms)",preDelay);
       cmd.AddValue("preBdw","initial bandwidth: xx Mbps(eg. 30Mbps)",preBdw);
       cmd.AddValue("postBdw","changed bandwidth: xx Mbps(eg. 50Mbps)",postBdw);
   }
   else
   {
       cmd.AddValue("bandwidth","bandwidth: xx Mbps(eg. 30Mbps)",preBdw);
       cmd.AddValue("preDelay","initial delay: xx ms(eg. 130ms)",preDelay);
       cmd.AddValue("postDelay","changed delay: xx ms(eg. 50ms)",postDelay);
   }
    printf("shit");
  cmd.AddValue ("tcpTypeId", "Transport protocol to use: TcpNewReno, TcpBbr", tcpTypeId); //How to use cubic or copa?
  cmd.AddValue ("delAckCount", "Delayed ACK count", delAckCount);
  cmd.AddValue ("stopTime", "Stop time for applications / simulation time will be stopTime + 1", stopTime);
  cmd.AddValue ("packetbuffer", "Stop time for applications / simulation time will be stopTime + 1", packetbuffer);
  cmd.Parse (argc, argv);
  
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::" + tcpTypeId));
  Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (6194304));
  Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (6291456));
  Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue (10));
  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (delAckCount));
  Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (1448));
  Config::SetDefault ("ns3::DropTailQueue<Packet>::MaxSize", QueueSizeValue (QueueSize (packetbuffer)));

  
  NodeContainer nodes;
  nodes.Create(2);

  // Create the point-to-point link helpers
  PointToPointHelper bottleneckLink;
  bottleneckLink.SetDeviceAttribute  ("DataRate", StringValue ("10Mbps"));
  bottleneckLink.SetChannelAttribute ("Delay", StringValue ("30ms"));
  NetDeviceContainer s1r2 = bottleneckLink.Install (nodes);
 
  InternetStackHelper Internet;
  Internet.Install(nodes.Get(0));
  Internet.Install(nodes.Get(1));

  // Assign IP addresses
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer i1i2 = ipv4.Assign (s1r2);
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  // Select sender side port
  uint16_t port = 50001;

  // Install application on the sender
  BulkSendHelper source ("ns3::TcpSocketFactory", InetSocketAddress (i1i2.GetAddress(1), port));
  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (nodes.Get (0));
  Simulator::Schedule (Seconds (0.2), &TraceCwnd,0, 0,dir);
  sourceApps.Start (Seconds (0.0));
  sourceApps.Stop (stopTime);

  // Install application on the receiver
  PacketSinkHelper sink ("ns3::TcpSocketFactory", InetSocketAddress (i1i2.GetAddress(0), port));
  ApplicationContainer sinkApps = sink.Install (nodes.Get (1));
  sinkApps.Start (Seconds (0.0));
  sinkApps.Stop (stopTime);
  printf("orzz");

  // Create a new directory to store the output of the program
  std::string dirToSave = "mkdir -p " + dir;
  if (system (dirToSave.c_str ()) == -1)
    {
      exit (1);
    }
 printf("orzz1");
  // Generate PCAP traces if it is enabled
  if (enablePcap)
    {
      if (system ((dirToSave + "/pcap/").c_str ()) == -1)
        {
          exit (1);
        }
      bottleneckLink.EnablePcapAll (dir + "/pcap/bbr", true);
    }
 printf("orzz2");
  Simulator::Schedule (Seconds(100) , &Change,postBdw,postDelay);
  // Check for dropped packets using Flow Monitor
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
  Simulator::Stop (stopTime + TimeStep (1));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


