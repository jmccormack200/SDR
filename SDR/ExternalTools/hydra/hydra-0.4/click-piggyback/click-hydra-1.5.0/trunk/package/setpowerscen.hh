/*********************************************************
 * choiiss
 * SVN_FILE : $Id : $
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 * 
 ***********************************************************/
 
#ifndef CLICK_SETPOWERSCEN_HH
#define CLICK_SETPOWERSCEN_HH
#include <click/element.hh>
#include <click/glue.hh>

CLICK_DECLS

class PowerControl;

class SetPowerScen : public Element { public:
  
  SetPowerScen();
  ~SetPowerScen();
  
  const char *class_name() const		{ return "SetPowerScen"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  int configure(Vector<String> &, ErrorHandler *);

  Packet *simple_action(Packet *);

  void add_handlers();

protected:
  PowerControl* _power_control;
  unsigned _verbose;

  static String read_handler(Element *e, void *);  
  static int write_handler(const String &arg, Element *e,
			   void *, ErrorHandler *errh);
private:
    unsigned _packet_count;
    unsigned _cur_power;
    float _cur_target_snr;
    float _c_map_power;
    unsigned _scen_num;
    float  _tx_amp; 
    float _tx_pow; 
    float _tx_loss;
    bool _scen1_down;

  unsigned scen_stairstep();
};

CLICK_ENDDECLS
#endif
