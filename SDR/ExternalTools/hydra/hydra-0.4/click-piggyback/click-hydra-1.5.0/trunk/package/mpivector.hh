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
 
/*********************************************************
 * 
 *  File	: $Source: /home/cvs/click-developing/elements/hydra/mpivector.hh,v $
 *  
 ***********************************************************/

#ifndef CLICK_MPIVECTOR_HH
#define CLICK_MPIVECTOR_HH
#include <click/element.hh>
#include <click/atomic.hh>

#include <click/hydra_anno.hh>

CLICK_DECLS

/*
 * =c
 * MPIVector()
 * =s 
 * convert or generate MAC frame
 * =d
 * Description goes here.
 * =a MPIVector
 */


class MPIVector : public Element {
public:
  MPIVector();
  ~MPIVector();

  const char *class_name() const		{ return "MPIVector"; }
  const char *port_count() const		{ return "1/1"; }
  const char *processing() const		{ return AGNOSTIC; }

  void* cast(const char *n);
  int configure(Vector<String> &conf, ErrorHandler *errh);
  
  Packet *simple_action(Packet *);

  void add_handlers();

  //virtual WritablePacket* prepare_packet(Packet* p);
  virtual Packet* prepare_packet(Packet* p);
  virtual int header_size() {return _hdr_size;}

protected:
  unsigned _hdr_size;
  unsigned _verbose;

  static String read_param(Element *e, void *vparam);
  static int write_param(const String &in_s, Element *e, void *vparam, ErrorHandler *errh);

};

CLICK_ENDDECLS
#endif