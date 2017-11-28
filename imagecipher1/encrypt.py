#!/usr/bin/python

import sys, os
from PIL import Image

filename = str(sys.argv[1])
image = Image.open(filename)
pix = image.load()

width = str(image.size[0])
height = str(image.size[1])


bytesFile = filename + 'bytes.txt'
encryptedBytesFile = filename + 'bytes.txt.encrypted.txt'
encryptedImageFile = encryptedBytesFile + '.png'
outputFileName = filename + '.encrypted.png'

os.system('python imageToBytes.py ' + sys.argv[1] + ' ' + sys.argv[2])
#os.system('valgrind --tool=massif ./main ' + bytesFile + ' 1 ' + width + ' ' + height + ' ' + str(len(sys.argv[2])))
os.system('./main ' + bytesFile + ' 1 ' + width + ' ' + height + ' ' + str(len(sys.argv[2])))
print('./main ' + bytesFile + ' 1 ' + width + ' ' + height + ' ' + str(len(sys.argv[2])))
os.system('python bytesToImage.py ' + encryptedBytesFile + ' ' + width + ' ' + height)
os.system('rm -f ' + bytesFile)
os.system('rm -f ' + encryptedBytesFile)

base = os.path.splitext(filename)[0]
os.rename(encryptedImageFile, base + "-encrypted.png")
