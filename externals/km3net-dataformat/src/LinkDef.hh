#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all function;


#pragma link C++ class KM3NETDAQ::JDAQAbstractPreamble-;
#pragma link C++ class KM3NETDAQ::JDAQPreamble-;
#pragma link C++ class KM3NETDAQ::JDAQUTCExtended+;
#pragma link C++ class KM3NETDAQ::JDAQChronometer+;
#pragma link C++ class KM3NETDAQ::JDAQHeader+;
#pragma link C++ class KM3NETDAQ::JDAQFrameStatus+;
#pragma link C++ class KM3NETDAQ::JDAQModuleIdentifier-;
#pragma link C++ class KM3NETDAQ::JDAQPMTIdentifier-;

#pragma link C++ class KM3NETDAQ::JDAQHit-;
#pragma link C++ class KM3NETDAQ::JDAQFrame+;
#pragma link C++ class KM3NETDAQ::JDAQSuperFrameHeader+;
#pragma link C++ class KM3NETDAQ::JDAQSuperFrame+;

#pragma link C++ class KM3NETDAQ::JDAQTimesliceHeader+;
#pragma link C++ class KM3NETDAQ::JDAQTimeslice+;
#pragma link C++ class KM3NETDAQ::JDAQTimesliceL0+;
#pragma link C++ class KM3NETDAQ::JDAQTimesliceL1+;
#pragma link C++ class KM3NETDAQ::JDAQTimesliceL2+;
#pragma link C++ class KM3NETDAQ::JDAQTimesliceSN+;

#pragma link C++ class KM3NETDAQ::JDAQSummaryFrame-;
#pragma link C++ class KM3NETDAQ::JDAQRate+;
#pragma link C++ class KM3NETDAQ::JDAQSummarysliceHeader+;
#pragma link C++ class KM3NETDAQ::JDAQSummaryslice+;

#pragma link C++ class KM3NETDAQ::JDAQKeyHit-;
#pragma link C++ class KM3NETDAQ::JDAQTriggeredHit-;
#pragma link C++ class KM3NETDAQ::JDAQTriggerCounter+;
#pragma link C++ class KM3NETDAQ::JDAQTriggerMask+;
#pragma link C++ class KM3NETDAQ::JDAQEventHeader+;
#pragma link C++ class KM3NETDAQ::JDAQEvent+;


#pragma link C++ defined_in "../offline/Vec.hh";
#pragma link C++ defined_in "../offline/Hit.hh";
#pragma link C++ defined_in "../offline/Trk.hh";
#pragma link C++ defined_in "../offline/Evt.hh";
#pragma link C++ defined_in "../offline/Head.hh";
#pragma link C++ defined_in "../offline/MultiHead.hh";
#pragma link C++ defined_in "../tools/reconstruction.hh";

#pragma link C++ class AAObject+;
#pragma link C++ class Vec+;
#pragma link C++ class std::vector<Vec>+;
#pragma link C++ class Trk+;
#pragma link C++ class Hit+;
#pragma link C++ class Evt+;
#pragma link C++ class Head+;
#pragma link C++ class MultiHead+;

#endif
