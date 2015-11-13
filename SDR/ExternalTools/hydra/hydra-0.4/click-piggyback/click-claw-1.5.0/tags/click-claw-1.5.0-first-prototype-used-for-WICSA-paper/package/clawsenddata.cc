/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/ether.h>
#include <click/confparse.hh>
#include <click/packet.hh>
#include <click/error.hh>
#include <click/standard/scheduleinfo.hh>
#include <click/straccum.hh>

#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "clawsenddata.hh"

CLICK_DECLS

ClawSendData::ClawSendData() : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawSendData::~ClawSendData()
{
}

void* ClawSendData::cast(const char *n)
{
  if( strcmp(n, "ClawSendData") == 0 )
    return (ClawSendData *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return Element::cast(n);
}


int ClawSendData::configure(Vector<String> &conf, ErrorHandler *errh)
{
  Element* conn_e;
  // FIXME: These two arguments should not be optional.
  int res = cp_va_parse(conf, this, errh, 
		    cpElement, "inter layer connector", &conn_e,
    		    cpOptional, cpKeywords,
                    "VERBOSE", cpUnsigned, "noisy?", &_verbose, 
		    cpEnd);
  
  if (res < 0)
    return res;

  ClawConnProtModule* conn = 0;
  if(conn_e) 
    conn = (ClawConnProtModule *) conn_e->cast("ClawConnProtModule");

  if(!conn) 
    return errh->error("ClawConn must be defined as an optional configuration");

  hydra_chatter(9, _verbose, "%{element}:configure: ClawConn = 0x%x\n", this, conn);
  claw_set_conn(conn); 

  return 0;
}


int ClawSendData::claw_regintr(int , char* , int )
{
  int res = -1;

  // FIXME: add intrval = (offset, packet pattern) to DB
 
  return res;
}

int ClawSendData::claw_getinfo(const char* , void* , int* )
{
  // need to change as the addhandler of Click
  int res  = -1;

  return res;
}

int ClawSendData::claw_setinfo(const char* , void* , int* )
{
  int res  = -1;


  return res;
}


Packet* ClawSendData::simple_action(Packet *pkt)
{
  // FIXME: add routine that checks intr_val with recved pkt

  _c_pkt = pkt->uniqueify();

  // It is blocking call, since it is synchronous event
  claw_notify_conn();

  return _c_pkt;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(ClawSendData)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)