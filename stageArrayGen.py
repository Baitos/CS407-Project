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
import random

pngPath = sys.argv[1]
png = Image.open(pngPath).convert("RGBA")
width, height = png.size

print(width, height)

pixels = np.array(png)
colorIdDict = {}
idCounter = int(sys.argv[2])

resultArray = np.zeros((height,width), dtype=int)

for y in range(height):
    for x in range(width):
        color = tuple(pixels[y,x])

        if color not in colorIdDict:
            colorIdDict[color] = (idCounter, (x,y))
            idCounter += 1
            resultArray[y,x] = colorIdDict[color][0]
        
        else:
            resultArray[y,x] = colorIdDict[color][0]

filename = "D:/Desktop/projects/Retro Runners/data/Desert/" + "StageArray.txt"

with open(filename, 'w') as outputFile:
    for row in resultArray:
        # Convert each element in the row to a string and join them with a space
        # You can use other delimiters like ',' or '\t' if preferred
        row_string = ','.join(map(str, row))
        # Write the row string to the file, followed by a newline character
        outputFile.write(row_string + ',\n')
    

#print("Here is your resulting array:")
#print(resultArray)
print("Here is your colorID Dictionary:")
for color in colorIdDict:
    print("Color ID: " + str(colorIdDict[color][0]) + " Coordinates: " + str(colorIdDict[color][1]))

