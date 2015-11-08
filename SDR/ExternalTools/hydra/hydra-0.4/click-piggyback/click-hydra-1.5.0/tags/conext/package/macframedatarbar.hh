/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_MACFRAMEDATARBAR_HH
#define CLICK_MACFRAMEDATARBAR_HH
#include <click/element.hh>
#include <click/atomic.hh>
#include <click/etheraddress.hh>
#include <clicknet/80211.h>

#include "macframe80211.hh"

CLICK_DECLS


class MACFrameDataRbar : public MACFrame80211 {
public:
  MACFrameDataRbar();
  ~MACFrameDataRbar();

  const char *class_name() const		{ return "MACFrameDataRbar"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);

  Packet *simple_action(Packet *);

  void add_handlers();

  WritablePacket* prepare_packet(Packet* p);
  WritablePacket* prepare_packet(Packet* p, EtherAddress& dst);
  const unsigned char* dst(Packet* p);  
  const unsigned char* src(Packet* p);  
  WritablePacket* set_header(Packet* p);
  int get_header(Packet* p, void* hdr);
  int header_size();// { return _data_hdr_size;}

protected:

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

  unsigned _mode;
  EtherAddress _bssid;

  struct data_rbar _data_hdr;
  size_t _data_hdr_size;
private:
};

CLICK_ENDDECLS
#endif
