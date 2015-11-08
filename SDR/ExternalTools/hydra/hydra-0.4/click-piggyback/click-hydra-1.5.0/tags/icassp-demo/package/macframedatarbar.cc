/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/hydra_anno.hh>

#include "macframedatarbar.hh"

CLICK_DECLS

MACFrameDataRbar::MACFrameDataRbar() 
  : _mode(FC1_DIR_NODS), _data_hdr_size(sizeof(struct data_rbar))
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

MACFrameDataRbar::~MACFrameDataRbar()
{
}


void*
MACFrameDataRbar::cast(const char *n)
{
  if( strcmp(n, "MACFrameDataRbar") == 0 )
    return (MACFrameDataRbar *) this;

  return MACFrame80211::cast(n);
}

int
MACFrameDataRbar::configure(Vector<String> &conf, ErrorHandler *errh)
{
  //hydra_chatter(9, 9, "%{element}::configure start", this);
  int res = cp_va_parse(conf, this, errh,
			cpEtherAddress, "local MAC address", &_mac_addr,
			cpOptional,
			cpKeywords,
 			"MODE", cpUnsigned, "mode", &_mode,
			"BSSID", cpEtherAddress, "local MAC address", &_bssid,
			"VERBOSE", cpUnsigned, "noisy?", &_verbose,
			cpEnd);

  if (res < 0)
    return res;

  if(!_mac_addr || IS_BROADCAST(_mac_addr.data()) )
    return errh->error("mac address must be set to unicast address");
  if(_mode && _mode > 3)
    return errh->error("bssid must be defined < 4");
  if(_mode && !_bssid)
    return errh->error("bssid must be defined when mode is not ad hoc");

  // setting the basic Data information for future use 
  memset((void *)&_data_hdr, 0, _data_hdr_size);
  _data_hdr.i_fc.protocol_version = FC0_VERSION_0;
  _data_hdr.i_fc.type = FC0_TYPE_DATA;
  _data_hdr.i_fc.to_from_ds = _mode;
  switch (_mode) 
  {
    case FC1_DIR_NODS:
      memcpy(_data_hdr.i_addr2, _mac_addr.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr3, _bssid.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_TODS:
      memcpy(_data_hdr.i_addr1, _bssid.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr2, _mac_addr.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_FROMDS:
      memcpy(_data_hdr.i_addr2, _bssid.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr3, _mac_addr.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_DSTODS:
      /* XXX this is wrong */
      memcpy(_data_hdr.i_addr2, _mac_addr.data(), ETHER_ADDR_LEN);
      memcpy(_data_hdr.i_addr3, _bssid.data(), ETHER_ADDR_LEN);
      break;
    default:
     hydra_chatter(0, _verbose, "%{element}::configure: never this case");
     break;
  }

  hydra_chatter(9, _verbose, "%{element}::configure", this);
  return 0;
}

// assume it receives complete data packet and generate cts frame
// just filtering the data packet
Packet *
MACFrameDataRbar::simple_action(Packet *p)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  if(!p || !p->length())
  {
    if(p) p->kill();
    return 0;
  }

  return p;
}

WritablePacket*
MACFrameDataRbar::prepare_packet(Packet* )
{
  return 0;
}

WritablePacket*
MACFrameDataRbar::prepare_packet(Packet* p, EtherAddress& dst)
{
  assert(p);

  struct data_rbar data_hdr_out;
  // setting up the rts packet
  memcpy(&data_hdr_out, &_data_hdr, _data_hdr_size);

  switch (_mode) 
  {
    case FC1_DIR_NODS:
      memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_TODS:
      memcpy(data_hdr_out.i_addr3, dst.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_FROMDS:
      memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
      break;
    case FC1_DIR_DSTODS:
      memcpy(data_hdr_out.i_addr1, dst.data(), ETHER_ADDR_LEN);
      break;
  }

  WritablePacket* p_out = Packet::make(HYDRA_HEADROOM, p->data(), p->length(), HYDRA_TAILROOM);
  p_out->push(_data_hdr_size);
  memcpy((void *)p_out->data(), &data_hdr_out, _data_hdr_size);

  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();
  memcpy(&cha->mac_type, &data_hdr_out.i_fc, 2);
  memcpy(cha->dst, dst.data(), 6);
  memcpy(cha->src, _mac_addr.data(), 6);

  cha->type = HYDRA_TX_DATA;

  hydra_chatter(8, _verbose, "%{element}::prepare_packet: src = %s dst = %s", this, _mac_addr.s().c_str(), dst.s().c_str());
  return p_out;
}


const unsigned char*
MACFrameDataRbar::dst(Packet* p)
{
  assert(p);

  struct data_rbar* data_in = (struct data_rbar *) p->data();
  if(data_in->i_fc.type == FC0_TYPE_DATA)
  {
    switch (_mode) 
    {
      case FC1_DIR_NODS:
	return data_in->i_addr1;
      case FC1_DIR_TODS:
	return data_in->i_addr3;
      case FC1_DIR_FROMDS:
	return data_in->i_addr1;
      case FC1_DIR_DSTODS:
	return data_in->i_addr1;
    }
  }

  hydra_chatter(9, _verbose, "%{element}::dst", this);
  return 0;
}

const unsigned char*
MACFrameDataRbar::src(Packet* p)
{
  struct data_rbar* data_in = (struct data_rbar *) p->data();
  if(data_in->i_fc.type == FC0_TYPE_DATA)
  {
    switch (_mode) 
    {
      case FC1_DIR_NODS:
	return data_in->i_addr2;
      case FC1_DIR_TODS:
	return data_in->i_addr2;
      case FC1_DIR_FROMDS:
	return data_in->i_addr3;
      case FC1_DIR_DSTODS:
	return data_in->i_addr2;
    }
  }

  hydra_chatter(9, _verbose, "%{element}::src", this);
  return 0;
}

int MACFrameDataRbar::header_size() 
{
  hydra_chatter(9, _verbose, "%{element}::header_size", this);
  return _data_hdr_size;
}

// set default header
WritablePacket* 
MACFrameDataRbar::set_header(Packet* p)
{
  WritablePacket* p_out = p->uniqueify();

  p_out->push(_data_hdr_size);
  memcpy(p_out->data(), &_data_hdr, _data_hdr_size);

  return p_out;
}

int 
MACFrameDataRbar::get_header(Packet* p, void* hdr)
{
  memcpy(hdr, p->data(), _data_hdr_size);
  return _data_hdr_size;
}

String
MACFrameDataRbar::read_param(Element *e, void *vparam)
{
  return MACFrame80211::read_param( e, vparam);
}

int
MACFrameDataRbar::write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh)
{
  return MACFrame80211::write_param(in_s, e, vparam, errh);
}

void
MACFrameDataRbar::add_handlers()
{
  MACFrame80211::add_handlers();
}

CLICK_ENDDECLS
EXPORT_ELEMENT(MACFrameDataRbar)
