#ifndef CLICK_CHECKCRCBER_HH
#define CLICK_CHECKCRCBER_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/etheraddress.hh>
#include <clicknet/80211.h>
CLICK_DECLS

class MACFrame80211;
class PowerControl;

class CheckCRCBER : public Element {
public:
  CheckCRCBER();
  ~CheckCRCBER();

  const char *class_name() const { return "CheckCRCBER"; }
  const char *port_count() const { return "1/1"; }
  const char *processing() const { return AGNOSTIC; }
  int configure(Vector<String> &conf, ErrorHandler *errh);
  int initialize(ErrorHandler *errh);
  
  Packet *simple_action(Packet *);
protected:
  void success_rts(Packet* pkt);
  void success_cts(Packet* pkt);
  void success_data(Packet* pkt);
  void success_ack(Packet* pkt);
  void fail_rts(Packet* pkt);
  void fail_cts(Packet* pkt);
  void fail_data(Packet* pkt);
  void fail_data_for_draw(Packet* pkt);
  void fail_ack(Packet* pkt);
  void fail(Packet* pkt);

protected:
  EtherAddress _my_mac_addr;

  String _filename;
  String _logfilename;
  FILE* _fptr_log;

  MACFrame80211* _mac_rts;
  MACFrame80211* _mac_cts;
  MACFrame80211* _mac_data;
  MACFrame80211* _mac_ack;
  int _mac_data_hdr_size;

  char _send_pattern[IEEE80211_MTU];
  unsigned _verbose;

  PowerControl* _power_control;

private:
  atomic_uint32_t _drops;

};

CLICK_ENDDECLS
#endif
