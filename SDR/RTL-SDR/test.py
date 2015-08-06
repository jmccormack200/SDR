#!/usr/bin/env python
import time
# Gr is the basic gnu radio module
# gr is always required
from gnuradio import gr
# audio and analog are libraries of blocks
from gnuradio import audio, analog

# Here we define a top block that inherits from
# the gr class top block.
class my_top_block(gr.top_block):
	def __init__(self):
		gr.top_block.__init__(self)
		
		# standard quality sample rate and amplitude value
		sample_rate = 32000
		ampl = 0.1
		startFreq0 = 350
		startFreq1 = 440
		# We create two sine waves with the same amplitude and sameple rate
		# from above.

		src0 = analog.sig_source_f(sample_rate, analog.GR_SIN_WAVE, startFreq0, ampl)
		src1 = analog.sig_source_f(sample_rate, analog.GR_SIN_WAVE, startFreq1, ampl)
		
		# dst is the "destination" or audio sink block
		dst = audio.sink(sample_rate, "")

		# We then connect the two sine waves to the destination block
		self.connect(src0, (dst, 0))
		self.connect(src1, (dst, 1))
		
		for a in range(1000):
			src0.set_frequency(startFreq0 + a)
			src1.set_frequency(startFreq1 + a)
			time.sleep(0.5)
if __name__ == '__main__':
	try:
		my_top_block().run()
	except [[KeyboardInterrupt]]:
		pass
