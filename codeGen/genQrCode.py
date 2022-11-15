import sys
import numpy as np
import image
import qrcode

if __name__ == "__main__":
	path = sys.argv[1]
	mop = sys.argv[2]
	inFile = ""
	if mop == "Module":
		inFile = path + "modInput"
		path = path + "Module/"
	else:
		inFile = path + "pmtInput"
		path = path + "PMT/"
	inCode = np.loadtxt(inFile, dtype=str)
	for code in inCode:
		print(code)
		qr = qrcode.QRCode(version=2,
						   error_correction=qrcode.constants.ERROR_CORRECT_H,
						   border=2,
						   box_size=10)
		qr.add_data(code)
		img = qr.make_image(fill_color='black', back_color='white')
		img.save(path + code + ".jpg")
