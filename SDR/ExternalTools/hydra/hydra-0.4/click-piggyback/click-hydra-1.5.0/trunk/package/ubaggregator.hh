/* 
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
 */

#ifndef HYDRA_UBAGGREGATOR_HH
#define HYDRA_UBAGGREGATOR_HH
#include <click/element.hh>
#include <click/glue.hh>

#include "prioritizer.hh"

CLICK_DECLS

/*
=c

UBAggregator()

=a 
*/

class UBAggregator : public Element { public:
  
  UBAggregator();
  ~UBAggregator();
  
  const char *class_name() const		{ return "UBAggregator"; }
  const char *port_count() const		{ return "2/1"; }
  const char *processing() const		{ return "ll/l"; }
  
  int configure(Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);

  void add_handlers();

  // Override the pull method to support aggregation
  Packet *pull(int port);

private:
  unsigned _max_broadcast;
  unsigned _max_unicast;
  unsigned _verbose;
  unsigned _unicast_len;
  unsigned _broadcast_len;
  Prioritizer *_prioritizer;

  Packet **_broadcast_buffer, **_unicast_buffer;

  static String read_param(Element *e, void *);  
  static int write_param(const String &arg, Element *e, void *, ErrorHandler *errh);

  static Packet *make_mpdu_with_pad(Packet *, bool more_data);
  unsigned aggregate(unsigned char **, u_int32_t *, u_int32_t *);
};

CLICK_ENDDECLS
#endif
