import socket
import json

class Viewer3dClient:
    RED = 0xff0000
    GREEN = 0x00ff00
    BLUE = 0x0000ff
    DARK_RED = 0x770000
    DARK_GREEN = 0x007700
    DARK_BLUE = 0x000077
    TRANSPARENT = 0x7f000000
    INVISIBLE = 0x01000000

    def __init__(self, host=None, port=None):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.host = host
        self.port = port

    def _send(self, obj):
        if self._socket:
            self._socket.sendto(json.dumps(obj).encode('utf-8'), (self.host, self.port))

    def line(self, x1, y1, z1, x2, y2, z2, color):
        self._send({
            'type': 'line',
            'points': [x1, y1, z1, x2, y2, z2],
            'color': color
        })

    def cube(self, x, y, z, size, color):
        self._send({
            'type': 'cube',
            'center': [x, y, z],
            'size': size,
            'color': color
        })

    def sphere(self, x, y, z, radius, color):
        self._send({
            'type': 'sphere',
            'center': [x, y, z],
            'radius': radius,
            'color': color
        })

    def plane(self, x, y, z, width, height, color):
        self._send({
            'type': 'plane',
            'center': [x, y, z],
            'width': width,
            'height': height,
            'color': color
        })

    def torus(self, x, y, z, radius, minorRadius, rings, rotationX, rotationY, color):
        self._send({
            'type': 'torus',
            'center': [x, y, z],
            'radius': radius,
            'minorRadius': minorRadius,
            'rings': rings,
            'rotationX': rotationX,
            'rotationY': rotationY,
            'color': color
        })

    def obj(self, path, x, y, z, scale, rotationX, rotationY, color):
        self._send({
            'type': 'obj',
            'path': path,
            'center': [x, y, z],
            'scale': scale,
            'rotationX': rotationX,
            'rotationY': rotationY,
            'color': color
        })

    def textureObj(self, objPath, texturePath, x, y, z, scale, rotationX, rotationY):
        self._send({
            'type': 'textureObj',
            'objPath': objPath,
            'texturePath': texturePath,
            'center': [x, y, z],
            'scale': scale,
            'rotationX': rotationX,
            'rotationY': rotationY
        })

    def clearAll(self):
        self._send({
            'type': 'clearAll'
        })


if __name__ == '__main__':
    client = Viewer3dClient("127.0.0.1", 5831)
    client.line(1, 2, 3, 4, 5, 6, "white")
    client.cube(20.5, 20.5, 2.5, 5, "white")
    client.sphere(30.5, 30.5, 2.5, 5, "white")
    client.plane(0, 0, 0, 5, 15, "white")

    client.obj("/home/user/3dViewer_on_3dCore/res/cow-nonormals.obj",
               40.5, 40.5, 10, 3, 90, 30, "pink")

    client.textureObj("/home/user/3dViewer_on_3dCore/res/drum.obj",
                      "/home/user/3dViewer_on_3dCore/res/drum_DefaultMaterial_BaseColor.png",
                      10, 3.5, 10, 3,90, 90)

    client.torus(60, 0, 60, 5, 1, 30, 0, 135, "white")
    # client.clearAll()

