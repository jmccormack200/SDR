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

#include "clawrecvcts.hh"

CLICK_DECLS

ClawRecvCTS::ClawRecvCTS() : _verbose(0)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

ClawRecvCTS::~ClawRecvCTS()
{
}

void* ClawRecvCTS::cast(const char *n)
{
  if( strcmp(n, "ClawRecvCTS") == 0 )
    return (ClawRecvCTS *) this;
  else if( strcmp(n, "ClawAdaptor") == 0 )
    return (ClawAdaptor *) this;

  return Element::cast(n);
}


int ClawRecvCTS::configure(Vector<String> &conf, ErrorHandler *errh)
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


int ClawRecvCTS::claw_regintr(int , char* , int )
{
  int res = -1;

  // FIXME: add intrval = (offset, packet pattern) to DB
 
  return res;
}

int ClawRecvCTS::claw_getinfo(const char* infoname, void* infoval, int* infolen)
{
  // need to change as the addhandler of Click
  int res  = -1;

  if( strcmp(infoname, "snr_int") == 0 )
  {
    if(*infolen != sizeof(unsigned int))
      return res;
   
    unsigned int snr_int; 
    remove_snr_int(snr_int);
    *(unsigned int *)infoval = snr_int;
  }
  else if( strcmp(infoname, "snr") == 0 )
  {
    if(*infolen != sizeof(double))
      return res;

    struct click_hydra_anno * cha = (struct click_hydra_anno *) _c_pkt->all_user_anno();
    *(double *) infoval = cha->avg_snr;
    hydra_chatter(9, _verbose, "%{element}:get_info: avg_snr=%f\n", this, cha->avg_snr);
    hydra_chatter(9, _verbose, "%{element}:get_info: infoval=%f\n", this, *(double *) infoval);
    return res;
  }

  return res;
}

int ClawRecvCTS::claw_setinfo(const char* , void* , int* )
{
  int res  = -1;


  return res;
}


int ClawRecvCTS::remove_snr_int(unsigned int& snr_int)
{
  int org_len = _c_pkt->length();
  unsigned char* data = _c_pkt->data();

  memcpy(&snr_int, data + org_len - sizeof(snr_int), sizeof(snr_int));

  _c_pkt->take(sizeof(snr_int));

  return 0;
}

Packet* ClawRecvCTS::simple_action(Packet *pkt)
{
  // FIXME: add routine that checks intr_val with recved pkt

  _c_pkt = pkt->uniqueify();

  // It is blocking call, since it is synchronous event
  claw_notify_conn();

  return _c_pkt;
}


CLICK_ENDDECLS
EXPORT_ELEMENT(ClawRecvCTS)
//ELEMENT_LIBS(-L/home/choiiss/cur_claw/claw_prot/lib -lclaw)
