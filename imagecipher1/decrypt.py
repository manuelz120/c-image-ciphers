#!/usr/bin/python

import sys, os
from PIL import Image

filename = str(sys.argv[1])
image = Image.open(filename)
pix = image.load()

width = str(image.size[0])
height = str(image.size[1])


bytesFile = filename + 'bytes.txt'
decryptedBytesFile = filename + 'bytes.txt.decrypted.txt'
decryptedImageFile = decryptedBytesFile + '.png'
outputFileName = filename + '.decrypted.png'

os.system('python imageToBytes.py ' + sys.argv[1] + ' ' + sys.argv[2])
os.system('./main ' + bytesFile + ' 2 ' + width + ' ' + height + ' ' + str(len(sys.argv[2])) + ' 100842898')
print('./main ' + bytesFile + ' 2 ' + width + ' ' + height + ' ' + str(len(sys.argv[2])) + ' 100842898')
os.system('python bytesToImage.py ' + decryptedBytesFile + ' ' + width + ' ' + height)
os.system('rm -f ' + bytesFile)
os.system('rm -f ' + decryptedBytesFile)
#os.system('mv ' + decryptedImageFile + ' ' + outputFileName)

base = os.path.splitext(filename)[0]
os.rename(decryptedImageFile, base + "-decrypted.png")
