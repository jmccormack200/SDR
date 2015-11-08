/*  	
 * Title:  Descramble (DSC)
 * Created By: Robert Daniels
 * Creation Date:
 * 
 * Description: Header for "Descrambling_11n.cc"
 *
 * Revision: v0.00 
 *
 * 
 * Copyright (C) 2009  The University of Texas at Austin.
 * 
 * This file is part of Hydra: A wireless multihop testbed.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */ 

#ifndef Descrambling_11N_H
#define Descrambling_11N_H

#include <itpp/itbase.h>

class Descrambling_11n
{
	public:
		Descrambling_11n();
		void descramble(itpp::bvec& in_bits);
		void scramble(itpp::bvec& in_bits, itpp::bvec const& binary_seed);
	private:
};
#endif
