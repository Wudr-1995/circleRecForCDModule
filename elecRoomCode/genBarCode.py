import matplotlib.pyplot as plt
import barcode

from barcode.writer import ImageWriter

code = barcode.generate('code128', '2F-2H-2J-2S-32CH-S',
						writer=barcode.writer.ImageWriter(),
						output='test',
						writer_options={"background": "white"},
						text='2F-2H-2J-2S-32CH-S\nN-9-04')
