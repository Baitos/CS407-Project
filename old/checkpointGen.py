#Goal is to take in a png and generate information for the checkpoint system
#Each checkpoint column requires a unique x value

#You will be returned a readable format containing each columns' x, y, and collider values
# in the png is associated with a unique colorID

#You will need to order the columns based on when they cross them in the game

#function call
# python3 ./checkpointGen.py png_path

from PIL import Image
import numpy as np
import sys
import random

pngPath = sys.argv[1]
png = Image.open(pngPath).convert("RGBA")
width, height = png.size

pixels = np.array(png)
colorIdDict = {}
colCount = 0

checkpointList = {}

resultArray = np.zeros((height,width), dtype=int)
emptyColor = -1
for x in range(width):
    for y in range(height):
        color = tuple(pixels[y,x])
        
        if color != (np.uint8(0),np.uint8(0),np.uint8(0),np.uint8(0)):
            if color not in colorIdDict:
                #print("COLOR: " + str(color))
                colorIdDict[color] = (x,y)
                #print(str(x) + " " + str(y))
            else :
                #print(str(colorIdDict[color][0]) + " " + str(colorIdDict[color][1]))
                #print(str(x) + " " + str(y))
                cords = colorIdDict[color]
                #print(cords)
                checkpointList[color] = (cords[0], cords[1], 1, y - cords[1])
                #print(checkpointList[color])
        
        #resultArray[y,x] = colorIdDict[color][0]

filename = "C:/Users/hohma/OneDrive/Desktop/CS 407/Stage Design/Spaceship/" + "StageArray.txt"

'''with open(filename, 'w') as outputFile:
    for row in resultArray:
        # Convert each element in the row to a string and join them with a space
        # You can use other delimiters like ',' or '\t' if preferred
        row_string = ','.join(map(str, row))
        # Write the row string to the file, followed by a newline character
        outputFile.write(row_string + ',\n')'''
    


#print("Here is your resulting array:")
#print(resultArray)
for checkpointKey in checkpointList:
    checkpoint = checkpointList[checkpointKey]
    print("X: " + str(checkpoint[0]) + " Y: " + str(checkpoint[1]) + " Width: " + str(checkpoint[2]) + " Height: " + str(checkpoint[3]))
'''
print("Here is your colorID Dictionary:")
for color in colorIdDict:
    print("Color ID: " + str(colorIdDict[color][0]) + " Coordinates: " + str(colorIdDict[color][1]))
    '''

