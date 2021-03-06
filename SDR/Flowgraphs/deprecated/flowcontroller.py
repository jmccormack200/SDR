#!/usr/bin/env python2
##################################################
# GNU Radio Python Flow Graph
# Title: Nonbroadcastwithfreqandmac
# Generated: Mon Feb  1 16:46:29 2016
#
# The name of this file may be misleading if
# threading is not used in the end. I just needed
# to change the title to ensure that it would
# not be overwritten when I went to build it again
# later. 
#
# This makes me realize I need to create a way
# to ensure that changes to the flow graph can
# be made without requiring massive code rewrites
# as we add features.
#
##################################################
import threading
from optparse import OptionParser
from gnuradio.eng_option import eng_option
from gnuradio import eng_notation
from nonbroadcastwithFreqandMacWithThreads import nonbroadcastwithFreqandMac
import mac
import pmt
import wx

from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('radiostats.html')

# http://www.secdev.org/projects/scapy/
# Saving that link for later

def start_flowgraph(flowgraph):
    tb.start(True)
    tb.wait()


if __name__ == '__main__':

    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option("", "--ampl", dest="ampl", type="eng_float", default=eng_notation.num_to_str(0.7),
        help="Set TX BB amp [default=%default]")
    parser.add_option("-a", "--args", dest="args", type="string", default='',
        help="Set USRP device args [default=%default]")
    parser.add_option("-t", "--arq-timeout", dest="arq_timeout", type="eng_float", default=eng_notation.num_to_str(.1*0 + 0.04),
        help="Set ARQ timeout [default=%default]")
    parser.add_option("-d", "--dest-addr", dest="dest_addr", type="intx", default=-1,
        help="Set Destination address [default=%default]")
    parser.add_option("", "--iface", dest="iface", type="string", default='tun0',
        help="Set Interface name [default=%default]")
    parser.add_option("", "--max-arq-attempts", dest="max_arq_attempts", type="intx", default=5 * 2,
        help="Set Max ARQ attempts [default=%default]")
    parser.add_option("", "--mtu", dest="mtu", type="intx", default=128,
        help="Set MTU [default=%default]")
    parser.add_option("-l", "--ogradio-addr", dest="ogradio_addr", type="intx", default=0,
        help="Set Local address [default=%default]")
    parser.add_option("", "--ogrx-freq", dest="ogrx_freq", type="eng_float", default=eng_notation.num_to_str(915e6),
        help="Set RX freq [default=%default]")
    parser.add_option("", "--ogtx-freq", dest="ogtx_freq", type="eng_float", default=eng_notation.num_to_str(915e6),
        help="Set TX freq [default=%default]")
    parser.add_option("", "--port", dest="port", type="string", default="12345",
        help="Set TCP port [default=%default]")
    parser.add_option("-r", "--rate", dest="rate", type="eng_float", default=eng_notation.num_to_str(1e6),
        help="Set Sample rate [default=%default]")
    parser.add_option("-A", "--rx-antenna", dest="rx_antenna", type="string", default="TX/RX",
        help="Set RX antenna [default=%default]")
    parser.add_option("", "--rx-gain", dest="rx_gain", type="eng_float", default=eng_notation.num_to_str(65-20),
        help="Set RX gain [default=%default]")
    parser.add_option("", "--rx-lo-offset", dest="rx_lo_offset", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set RX LO offset [default=%default]")
    parser.add_option("", "--samps-per-sym", dest="samps_per_sym", type="intx", default=4,
        help="Set Samples/symbol [default=%default]")
    parser.add_option("", "--tx-gain", dest="tx_gain", type="eng_float", default=eng_notation.num_to_str(45),
        help="Set TX gain [default=%default]")
    parser.add_option("", "--tx-lo-offset", dest="tx_lo_offset", type="eng_float", default=eng_notation.num_to_str(0),
        help="Set TX LO offset [default=%default]")
    (options, args) = parser.parse_args()
    tb = nonbroadcastwithFreqandMac(ampl=options.ampl, args=options.args, arq_timeout=options.arq_timeout, dest_addr=options.dest_addr, iface=options.iface, max_arq_attempts=options.max_arq_attempts, mtu=options.mtu, ogradio_addr=options.ogradio_addr, ogrx_freq=options.ogrx_freq, ogtx_freq=options.ogtx_freq, port=options.port, rate=options.rate, rx_antenna=options.rx_antenna, rx_gain=options.rx_gain, rx_lo_offset=options.rx_lo_offset, samps_per_sym=options.samps_per_sym, tx_gain=options.tx_gain, tx_lo_offset=options.tx_lo_offset)

    thread = threading.Thread(target=start_flowgraph, args=())
    print(tb.get_tx_freq())
    tb.set_tx_freq(100000000.0)
    print(tb.get_tx_freq())

    app.run(
        host="0.0.0.0",
        port=int("80"),
        debug=True
    )