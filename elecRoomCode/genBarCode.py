import matplotlib.pyplot as plt
import numpy as np
import barcode

from barcode.writer import ImageWriter

data = np.loadtxt("./uwbElecInput", dtype=str)

gcuPre = 'SIZE 50 mm, 60.1 mm\n'
gcuPre += 'BLINE 3.5 mm, 0 mm\n'
gcuPre += 'SPEED 3\nDENSITY 12\n'
gcuPre += 'DIRECTION 0,0\nREFERENCE 0,0\n'
gcuPre += 'OFFSET 0 mm\nSET REWIND OFF\n'
gcuPre += 'SET PEEL OFF\nSET CUTTER OFF\n'
gcuPre += 'SET PARTIAL_CUTTER OFF\nSET TEAR ON\n'
gcuPre += 'CLS\nBARCODE 295,423,"128M",38,0,180,1,2,"!104'

gcuMid = '"\nTEXT 295,369,"3",180,1,1,"'

gcuSuf = '"\nPRINT 1,1\n'

for i in range(np.size(data, 0)):
	tmpStr = ''
	tmpStr = tmpStr + data[i, 0]
	tmpStr = tmpStr + 'F-'
	tmpStr = tmpStr + data[i, 1]
	tmpStr = tmpStr + 'H-'
	tmpStr = tmpStr + data[i, 2]
	tmpStr = tmpStr + 'J-'
	tmpStr = tmpStr + data[i, 3]
	tmpStr = tmpStr + 'S'
	print(tmpStr)
	gcu = gcuPre + data[i, 6] + gcuMid + data[i, 6] + gcuSuf
	cid = gcuPre + tmpStr + gcuMid + tmpStr + gcuSuf
	print(gcu)
	print(cid)

	gcuOut = open('gcus/' + 'GCU-' + data[i, 4] + '.prn', mode='w')
	cidOut = open('labels/' + 'Wire-' + data[i, 4] + '.prn', mode='w')

	gcuOut.write(gcu)
	cidOut.write(cid)

	gcuOut.close()
	cidOut.close()

# 	code = barcode.generate('code128', tmpStr,
# 							writer=barcode.writer.ImageWriter(),
# 							output='labels/' + tmpStr + '-' + data[i, 4],
# 							writer_options={"background": "white",
# 											"module_width": 0.2,
# 											"module_height": 10.0,
# 											"font_size": 10,
# 											"quiet_zone": 6.5,
# 										   },
# 							text=tmpStr)
# 
# 	code = barcode.generate('code128', data[i, 6],
# 							writer=barcode.writer.ImageWriter(),
# 							output='gcus/' + tmpStr + '-' + data[i, 4],
# 							writer_options={"background": "white",
# 											"module_width": 0.2,
# 											"module_height": 10.0,
# 											"font_size": 10,
# 											"quiet_zone": 6.5,
# 										   },
# 							text=data[i, 6])
