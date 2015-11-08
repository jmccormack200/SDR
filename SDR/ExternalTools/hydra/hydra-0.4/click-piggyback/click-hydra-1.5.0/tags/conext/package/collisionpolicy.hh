/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/collisionpolicy.hh,v $
 *  
 ***********************************************************/
#ifndef CLICK_COLLISIONPOLICY_HH
#define CLICK_COLLISIONPOLICY_HH
#include <click/element.hh>
#include <click/glue.hh>
CLICK_DECLS

/*
* =c
* 
* CollisionPolicy
* 
* =s Hydra
* 
* Policy holder for collision avoidance and collision reducing schemes
* 
* =d
* 
* Description goes here
* 
* Keyword arguments are:
*
* =over 8
*
* =item MAX_RETRY
*
* maximum number of retransmission before dropping packet, default is 7
*
* =h verbose read/write
* Enable/disable rts/cts for a packet.
* 
* =a MACTXScheduler */

class CollisionPolicy : public Element { 
public:
  
  CollisionPolicy();
  ~CollisionPolicy();
  
  const char *class_name() const		{ return "CollisionPolicy"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  
  int configure(Vector<String> &, ErrorHandler *);
  Packet *simple_action(Packet *);

  void add_handlers();

  // for retransmission of MACTXScheduler
  virtual void success(const Packet* p);
  virtual void fail(const Packet* p);
  virtual bool retry(const Packet* p);
  virtual void reset_retry(const Packet* p);

  // for control message such as backoff slot number or time slot
  // return packet must be uniquefied.
  virtual Packet* process_control(Packet* p);
  virtual Packet* set_control(Packet* p);

protected:
  static String read_param(Element*, void*);
  static int write_param(const String&, Element*, void*, ErrorHandler*);

  unsigned int _max_tries;
  unsigned int _num_tries;

  int _num_success;
  int _num_fail;
  int _num_tx;

  unsigned _verbose;

private:
};

CLICK_ENDDECLS
#endif
