/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 ***********************************************************/
 
#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/hydra_anno.hh>
#include <clicknet/80211.h>

#include "ratecontrolil.hh"

CLICK_DECLS

  // set default rate as the highest rate
RateControlIL::RateControlIL()
  : _snr(50.0), _rate(54)
{
  hydra_chatter(9, 8, "%{element}::constructor", this);
}

RateControlIL::~RateControlIL()
{
}

void*
RateControlIL::cast(const char *n)
{
  if( strcmp(n, "RateControlIL") == 0 )
    return (RateControlIL *) this;
  else
    return RateControl::cast(n);
}

int
RateControlIL::configure(Vector<String> &conf, ErrorHandler *errh)
{
  if (cp_va_parse(conf, this, errh,
 		  cpOptional,
		  cpKeywords, 
		  "VERBOSE", cpUnsigned, "debugging level", &_verbose, 
		  cpEnd) < 0) {
    return -1;
  }


  hydra_chatter(9, _verbose, "%{element}::configure", this);
  init_tables();
  return 0;
}

Packet *
RateControlIL::simple_action(Packet *p_in)
{
  hydra_chatter(8, _verbose, "%{element}::simple_action", this);

  set_txrate(p_in);

  return p_in;
}


void
RateControlIL::set_txrate(Packet* p_in)
{

  // just setting the snr and calculate at get_txrate
  struct click_hydra_anno* cha = (struct click_hydra_anno *)p_in->all_user_anno();
  _snr = (float) cha->avg_snr; 

  // FIXME: maybe we need to covert CTSRSSI into original CTS

  hydra_chatter(8, _verbose, "%{element}::set_txrate: %f", this, _snr);

}


WritablePacket*
RateControlIL::get_txrate(Packet* p_in)
{

  WritablePacket* p_out = p_in->uniqueify(); 
  struct click_hydra_anno* cha = (struct click_hydra_anno *) p_out->all_user_anno();

  // put rate control algorithm here
  // using both the p_in->length() and _snr
  _rate = calculate_datarate_ber(_snr, p_out->length());

  hydra_chatter(8, _verbose, "%{element}::get_txrate: _snr = %f, _rate = %d", this, _snr, _rate);
  
  cha->rate_mbps = (uint16_t)_rate;

  return p_out;
}

using namespace std;

#define SNR_COUNT 7 
#define MAX_SNR  30
#define RATE_COUNT 8
#define MAX_RATE  54


void RateControlIL::init_tables()
{
  _snr_index.resize(SNR_COUNT, 0);
  _snr_index[0] = 0; _snr_index[1] = 5;
  _snr_index[2] = 10; _snr_index[3] = 15;
  _snr_index[4] = 20; _snr_index[5] = 25;
  _snr_index[6] = 30; 
/*
  _snr_index_rev(MAX_SNR+1, 0)
  _snr_index_rev[0] = 0; _snr_index_rev[5] = 1;
  _snr_index_rev[10] = 2; _snr_index_rev[15] = 3;
  _snr_index_rev[20] = 4; _snr_index_rev[25] = 5;
  _snr_index_rev[30] = 6;
*/
  // init rate index
  _rate_index.resize(RATE_COUNT, 0);
  _rate_index[0] = 6; _rate_index[1] = 9;
  _rate_index[2] = 12; _rate_index[3] = 18;
  _rate_index[4] = 24; _rate_index[5] = 36;
  _rate_index[6] = 48; _rate_index[7] = 54;

  // init rate index rev
/*
  _rate_index_rev(MAX_RATE+1, 0);
  _rate_index_rev[6]  = 0; _rate_index_rev[9]  = 1; 
  _rate_index_rev[12] = 2; _rate_index_rev[18] = 3; 
  _rate_index_rev[24] = 4; _rate_index_rev[36] = 5; 
  _rate_index_rev[48] = 6; _rate_index_rev[54] = 7; 
*/

  //init ber table
  int i;
  _ber.resize(SNR_COUNT*RATE_COUNT, 0.0);
  i=0; // 0 db
  _ber[i*RATE_COUNT + 0] = 2.84e-2; _ber[i*RATE_COUNT + 1] = 7.40e-2;
  _ber[i*RATE_COUNT + 2] = 1.74e-1; _ber[i*RATE_COUNT + 3] = 2.91e-1;
  _ber[i*RATE_COUNT + 4] = 3.47e-1; _ber[i*RATE_COUNT + 5] = 4.75e-1;
  _ber[i*RATE_COUNT + 6] = 4.97e-1; _ber[i*RATE_COUNT + 7] = 5.02e-1;
  i=1; // 5 db
  _ber[i*RATE_COUNT + 0] = 4.68e-3; _ber[i*RATE_COUNT + 1] = 5.63e-2;
  _ber[i*RATE_COUNT + 2] = 1.41e-1; _ber[i*RATE_COUNT + 3] = 2.74e-1;
  _ber[i*RATE_COUNT + 4] = 3.37e-1; _ber[i*RATE_COUNT + 5] = 4.69e-1;
  _ber[i*RATE_COUNT + 6] = 4.95e-1; _ber[i*RATE_COUNT + 7] = 4.97e-1;
  i=2; // 10 db
  _ber[i*RATE_COUNT + 0] = 4.38e-6; _ber[i*RATE_COUNT + 1] = 1.46e-3;
  _ber[i*RATE_COUNT + 2] = 6.82e-2; _ber[i*RATE_COUNT + 3] = 4.33e-3;
  _ber[i*RATE_COUNT + 4] = 1.76e-1; _ber[i*RATE_COUNT + 5] = 4.65e-1;
  _ber[i*RATE_COUNT + 6] = 4.95e-1; _ber[i*RATE_COUNT + 7] = 4.96e-1;
  i=3; // 15 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 7.53e-5;
  _ber[i*RATE_COUNT + 2] = 2.81e-2; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 1.48e-4; _ber[i*RATE_COUNT + 5] = 5.86e-2;
  _ber[i*RATE_COUNT + 6] = 4.37e-1; _ber[i*RATE_COUNT + 7] = 4.37e-1;
  i=4; // 20 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 0.0;
  _ber[i*RATE_COUNT + 2] = 1.00e-2; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 0.0; _ber[i*RATE_COUNT + 5] = 2.27e-3;
  _ber[i*RATE_COUNT + 6] = 2.63e-2; _ber[i*RATE_COUNT + 7] = 8.38e-2;
  i=5; // 25 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 0.0;
  _ber[i*RATE_COUNT + 2] = 3.37e-3; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 0.0; _ber[i*RATE_COUNT + 5] = 0.0;
  _ber[i*RATE_COUNT + 6] = 0.0; _ber[i*RATE_COUNT + 7] = 2.5e-5;
  i=6; // 30 db
  _ber[i*RATE_COUNT + 0] = 0.0; _ber[i*RATE_COUNT + 1] = 0.0;
  _ber[i*RATE_COUNT + 2] = 3.15e-3; _ber[i*RATE_COUNT + 3] = 0.0;
  _ber[i*RATE_COUNT + 4] = 0.00; _ber[i*RATE_COUNT + 5] = 0.0;
  _ber[i*RATE_COUNT + 6] = 0.00; _ber[i*RATE_COUNT + 7] = 0.0;

}

void RateControlIL::interpolate_bers(double snr, vector<double>& bers)
{
  hydra_chatter(9, _verbose, "%{element}:: interpolate: recv snr = %f\n", this, snr);
  if(snr <= (double)_snr_index[0])
  {
    //printf("snr[0] = %f\n", (double)_snr_index[0]);
    for(int i=0; i<RATE_COUNT; i++)
      bers[i] = _ber[i]; 
    return;
  }
  else if(snr >= (double)_snr_index[SNR_COUNT-1])
  {
    //printf("snr[SNR_COUNT-1] = %f\n", (double)_snr_index[SNR_COUNT-1]);
    for(int i=0; i<RATE_COUNT; i++)
      bers[i] = _ber[(SNR_COUNT-1)*RATE_COUNT+i]; 
    return;
  }

   int lower_index = (int)snr/5; 
   //printf("RateControlIL:: interpolate: index = %d, lower_snr = %d\n", lower_index, _snr_index[lower_index]);
   //hydra_chatter(8, _verbose, "%{element}:interpolate_ber: snr = %f, lower_snr = %d\n", this, snr, _snr_index[lower_index]);

   double slope = 0.0;
   double lower_ber;
   double higher_ber;
   //printf("RateControlIL:: interpolate: BERs");
   for(int i=0; i<RATE_COUNT; i++)
   {
     lower_ber = _ber[(lower_index + 0)*RATE_COUNT + i];
     higher_ber = _ber[(lower_index + 1)*RATE_COUNT + i];

     slope = (higher_ber - lower_ber)/5.0;
     bers[i] = lower_ber + slope*(snr - _snr_index[lower_index]);
     hydra_chatter(9, _verbose, "[rate=%d, ber=%f] ", _rate_index[i], bers[i]);
   } 
   hydra_chatter(9, _verbose, "\n");
} 

unsigned short 
RateControlIL::calculate_bers(unsigned int , vector<double>& bers)
{
  unsigned short rate = 6;

  for(int i = RATE_COUNT-1; i>0; i--)
  {
    if(bers[i] <= (double)_cutoff)
    {
      rate = _rate_index[i];
      break;
    }
  }

  hydra_chatter(9, _verbose, "%{element}:: calculate_bers rate = %d\n", this, rate);
  //hydra_chatter(9, _verbose, "\n");

  //printf("RateControlIL:: cutoff = %f, per = %f, rate = %d\n", cutoff, per, rate);
  //hydra_chatter(8, _verbose, "%{element}:calculate_pers: per = %f rate = %d\n", this, per, rate); 
  return rate; 
}

unsigned short 
RateControlIL::calculate_pers(unsigned int len, vector<double>& bers)
{
  unsigned short rate = 6;
  double per = 1.0;
  double bsr = 1.0;

  for(int i = RATE_COUNT-1; i>0; i--)
  {
    bsr = 1.0 - bers[i];
    per = 1.0;
    // FIXME: need to change to simple calculation such as taylor series
    for(unsigned int j=0; j<len; j++)
      per *= bsr; 
    per = 1 - per;

    hydra_chatter(9, _verbose, "[rate=%d, per=%f]", _rate_index[i], per);
    if(per <= (double) _cutoff)
    {
      rate = _rate_index[i];
      break;
    }
  }

  hydra_chatter(9, _verbose, "%{element}:: calculate_pers rate = %d\n", this, rate);

  //printf("RateControlIL:: cutoff = %f, per = %f, rate = %d\n", cutoff, per, rate);
  //hydra_chatter(8, _verbose, "%{element}:calculate_pers: per = %f rate = %d\n", this, per, rate); 
  return rate; 
}
 
unsigned short 
RateControlIL::calculate_datarate_ber(double snr, unsigned int data_length)
{
  vector<double> error_rates(RATE_COUNT); 

  interpolate_bers(snr, error_rates);

  return calculate_bers(data_length, error_rates);
}

unsigned short 
RateControlIL::calculate_datarate_per(double snr, unsigned int data_length)
{
  vector<double> error_rates(RATE_COUNT); 

  interpolate_bers(snr, error_rates);

  return calculate_pers(data_length, error_rates);
}


enum {H_VERBOSE};

String
RateControlIL::read_handler(Element *e, void *thunk)
{
  RateControlIL *src = (RateControlIL *)e;
  switch((uintptr_t) thunk) 
  {
    case H_VERBOSE: 
      return String(src->_verbose) + "\n";
    default:   return "\n";
  }
  
}

int
RateControlIL::write_handler(const String &arg, Element *e,
			 void *vparam, ErrorHandler *errh) 
{
  RateControlIL *src = (RateControlIL *) e;
  String s = cp_uncomment(arg);
  switch((int)vparam) {
  case H_VERBOSE: {
    unsigned m;
    if (!cp_unsigned(s, &m)) 
      return errh->error("verbose parameter must be unsigned");
    src->_verbose = m;
    break;
  }
  }
  return 0;
}

void
RateControlIL::add_handlers()
{
  add_read_handler("verbose", read_handler, (void *) H_VERBOSE);
  add_write_handler("verbose", write_handler, (void *) H_VERBOSE);
}

CLICK_ENDDECLS
EXPORT_ELEMENT(RateControlIL)

