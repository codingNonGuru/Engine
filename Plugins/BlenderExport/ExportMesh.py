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

    hasTextureIndices = False
    try:
        object.vertex_groups['TextureIndex']
        hasTextureIndices = True
        attributeCount = 4
    except:
        pass

    data = struct.pack('=I', attributeCount)
    file.write(data)

# ATTRIBUTE HEADER : POSITION

    attributeName = "position"
    attributeNameFiller = chr(0) * (32 - len(attributeName))
    data = bytearray(attributeName + attributeNameFiller, 'utf-8')
    file.write(data)

    elementCount = len(object.data.vertices)
    data = struct.pack('=I', elementCount)
    file.write(data)

    elementSize = 12
    data = struct.pack('=I', elementSize)
    file.write(data)

    attributeType = "vec3"
    attributeTypeFiller = chr(0) * (8 - len(attributeType))
    data = bytearray(attributeType + attributeTypeFiller, 'utf-8')
    file.write(data)

# ATTRIBUTE HEADER : NORMAL

    attributeName = "normal"
    attributeNameFiller = chr(0) * (32 - len(attributeName))
    data = bytearray(attributeName + attributeNameFiller, 'utf-8')
    file.write(data)

    elementCount = len(object.data.vertices)
    data = struct.pack('=I', elementCount)
    file.write(data)

    elementSize = 12
    data = struct.pack('=I', elementSize)
    file.write(data)

    attributeType = "vec3"
    attributeTypeFiller = chr(0) * (8 - len(attributeType))
    data = bytearray(attributeType + attributeTypeFiller, 'utf-8')
    file.write(data)

# ATTRIBUTE HEADER : INDEX

    attributeName = "index"
    attributeNameFiller = chr(0) * (32 - len(attributeName))
    data = bytearray(attributeName + attributeNameFiller, 'utf-8')
    file.write(data)

    elementCount = len(object.data.polygons) * 3
    data = struct.pack('=I', elementCount)
    file.write(data)

    elementSize = 4
    data = struct.pack('=I', elementSize)
    file.write(data)

    attributeType = "uint"
    attributeTypeFiller = chr(0) * (8 - len(attributeType))
    data = bytearray(attributeType + attributeTypeFiller, 'utf-8')
    file.write(data)

# ATTRIBUTE HEADER : TEXTURE INDEX

    if hasTextureIndices:
        attributeName = "textureIndex"
        attributeNameFiller = chr(0) * (32 - len(attributeName))
        data = bytearray(attributeName + attributeNameFiller, 'utf-8')
        file.write(data)

        elementCount = len(object.data.vertices)
        data = struct.pack('=I', elementCount)
        file.write(data)

        elementSize = 4
        data = struct.pack('=I', elementSize)
        file.write(data)

        attributeType = "float"
        attributeTypeFiller = chr(0) * (8 - len(attributeType))
        data = bytearray(attributeType + attributeTypeFiller, 'utf-8')
        file.write(data)


# ATTRIBUTE DATA : POSITION
    
    for vertex in object.data.vertices:
        data = struct.pack('=f', vertex.co.x)
        file.write(data)
        data = struct.pack('=f', vertex.co.y)
        file.write(data)
        data = struct.pack('=f', vertex.co.z)
        file.write(data)

# ATTRIBUTE DATA : POSITION
    
    for vertex in object.data.vertices:
        data = struct.pack('=f', vertex.normal.x)
        file.write(data)
        data = struct.pack('=f', vertex.normal.y)
        file.write(data)
        data = struct.pack('=f', vertex.normal.z)
        file.write(data)

# ATTRIBUTE DATA : INDEX

    for polygon in object.data.polygons:
        for vertex in polygon.vertices:
            data = struct.pack('=I', vertex)
            file.write(data)

# ATTRIBUTE DATA : TEXTURE INDEX

    if hasTextureIndices:
        vertexGroup = object.vertex_groups['TextureIndex']

        index = 0
        for vertex in object.data.vertices:
            weight = 0.0
            try:
                weight = vertexGroup.weight(index)
            except:
                pass

            print(weight)

            data = struct.pack('=f', weight)
            file.write(data)
            index += 1

# END

    file.close()
        
        
