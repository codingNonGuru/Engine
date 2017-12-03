import bpy
import struct
import string

def PrintPolygons(object):
    print(object.name)
    for polygon in object.data.polygons:
        for vertex in polygon.vertices:
            print(vertex)
        print (" ")

def WriteMesh(object):
    print(object.name)
    file = open(object.name + ".mesh", "wb")

    attributeCount = 3
    data = struct.pack('>i', attributeCount)
    file.write(data)

    attributeName = "position"
    attributeNameFiller = chr(0) * (32 - len(attributeName))
    data = bytearray(attributeName + attributeNameFiller, 'utf-8')
    file.write(data)

    elementCount = len(object.data.vertices)
    data = struct.pack('>i', elementCount)
    file.write(data)

    elementSize = 12
    data = struct.pack('>i', elementSize)
    file.write(data)

    attributeType = "vec3"
    attributeTypeFiller = chr(0) * (8 - len(attributeType))
    data = bytearray(attributeType + attributeTypeFiller, 'utf-8')
    file.write(data)
    
    for vertex in object.data.vertices:
        data = struct.pack('>f', vertex.co.x)
        file.write(data)
        data = struct.pack('>f', vertex.co.y)
        file.write(data)
        data = struct.pack('>f', vertex.co.z)
        file.write(data)

    file.close()
        
        
