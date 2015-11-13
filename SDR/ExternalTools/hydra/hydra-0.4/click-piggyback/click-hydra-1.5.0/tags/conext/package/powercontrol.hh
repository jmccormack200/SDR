/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#ifndef CLICK_POWERCONTROL_HH
#define CLICK_POWERCONTROL_HH
#include <click/element.hh>
#include <click/glue.hh>

//#include <click/etheraddress.hh>
//#include <click/bighashmap.hh>
//#include <click/hydra_anno.hh>

CLICK_DECLS

class PowerControl : public Element { 

public:
  
  PowerControl();
  ~PowerControl();
  
  const char *class_name() const		{ return "PowerControl"; }
  const char *port_count() const		{ return "0/0"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *p);

  void add_handlers();
  
  virtual WritablePacket* get_power(Packet *p_in);
  virtual void set_power(unsigned power);


protected:
  unsigned _verbose;

  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
  
};

CLICK_ENDDECLS
#endif