#Goal is to take in a png and generate the stage array for SDL in a 1:1. 
#Every element in the array is equal to 1 pixel. 

#You will be returned a 2d array with each unique RGBA value found 
# in the png is associated with a unique colorID

#You will also be returned each colorID's (x,y) coordinates where they were first encountered 
#Use the given (x,y) to determine what tile belongs with which ID 

#function call
# python3 ./stageArrayGen.py png_path startingIdNumber

from PIL import Image
import numpy as np
import sys

pngPath = sys.argv[1]
png = Image.open(pngPath).convert("RGBA")
width, height = png.size

pixels = np.array(png)
colorIdDict = {}
idCounter = int(sys.argv[2])

resultArray = np.zeros((width,height), dtype=int)

for x in range(width):
    for y in range(height):
        color = tuple(pixels[x,y])

        if color not in colorIdDict:
            colorIdDict[color] = (idCounter, (y,x))
            idCounter += 1
        
        resultArray[x,y] = colorIdDict[color][0]

print("Here is your resulting array:")
print(resultArray)
print("\nHere is your colorID Dictionary:")
for color in colorIdDict:
    print("Color ID: " + str(colorIdDict[color][0]) + " Coordinates: " + str(colorIdDict[color][1]))

