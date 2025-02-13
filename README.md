# 3dViewer
## Описание
3dViewer предоставляет собой программу для отрисовки 3d объектов на сцене.
Сцена оборудована сеткой и возможностью изменять визуал под пользователя.

Для удобной отрисовки объектов присутсвует api json запросов. 
Изменения порта прослушивание возможно из интрфейса программы. 

## JSON API
С помощью api вы можете отрисовать:
- line
- cube
- sphere
- plane
- torus
- obj
- texture obj

### line
```yaml
'type': 'line',                             # (string)
'points': [x1, y1, z1, x2, y2, z2],         # (double, double, double, double, double, double) 
'color': color                              # (string)
```

### cube
```yaml
'type': 'cube',                             # (string)
'center': [x, y, z],                        # (double, double, double)
'size': size,                               # (double)
'color': color                              # (string)
```

### sphere
```yaml
'type': 'sphere',                           # (string)
'center': [x, y, z],                        # (double, double, double)
'radius': radius,                           # (double)
'color': color                              # (string)
```

### plane
```yaml
'type': 'plane',                            # (string)
'center': [x, y, z],                        # (double, double, double)
'width': width,                             # (double)
'height': height,                           # (double)
'color': color                              # (string)
```

### torus
```yaml
'type': 'torus',                            # (string)
'center': [x, y, z],                        # (double, double, double)
'radius': radius,                           # (double)
'minorRadius': minorRadius,                 # (double)
'rings': rings,                             # (int)
'rotationX': rotationX,                     # (double) deg
'rotationY': rotationY,                     # (double) deg
'color': color                              # (string)
```

### obj
```yaml
'type': 'obj',                              # (string)
'path': 'file:///path/to/file.obj',         # (string)
'center': [x, y, z],                        # (double, double, double)
'scale': scale,                             # (double)
'rotationX': rotationX,                     # (double) deg
'rotationY': rotationY,                     # (double) deg
'color': color                              # (string)
```

### texture obj
```yaml
'type': 'textureObj',                       # (string)
'objPath': 'file:///path/to/file.obj',      # (string)
'texturePath': 'file:///path/to/texture.png',         # (string)
'center': [x, y, z],                        # (double, double, double)
'scale': scale,                             # (double)
'rotationX': rotationX,                     # (double) deg
'rotationY': rotationY,                     # (double) deg
```

### clear all
```yaml
'type': 'clearAll',                         # (string)
```

### color names
<details>
  <summary>Colors</summary>
```yaml
"aliceblue", "antiquewhite", "aqua", "aquamarine", "azure", "beige", "bisque", "black", "blanchedalmond", "blue", "blueviolet", "brown", "burlywood", "cadetblue", "chartreuse", "chocolate", "coral", "cornflowerblue", "cornsilk", "crimson", "cyan", "darkblue", "darkcyan", "darkgoldenrod", "darkgray", "darkgreen", "darkgrey", "darkkhaki", "darkmagenta", "darkolivegreen", "darkorange", "darkorchid", "darkred", "darksalmon", "darkseagreen", "darkslateblue", "darkslategray", "darkslategrey", "darkturquoise", "darkviolet", "deeppink", "deepskyblue", "dimgray", "dimgrey", "dodgerblue", "firebrick", "floralwhite", "forestgreen", "fuchsia", "gainsboro", "ghostwhite", "gold", "goldenrod", "gray", "green", "greenyellow", "grey", "honeydew", "hotpink", "indianred", "indigo", "ivory", "khaki", "lavender", "lavenderblush", "lawngreen", "lemonchiffon", "lightblue", "lightcoral", "lightcyan", "lightgoldenrodyellow", "lightgray", "lightgreen", "lightgrey", "lightpink", "lightsalmon", "lightseagreen", "lightskyblue", "lightslategray", "lightslategrey", "lightsteelblue", "lightyellow", "lime", "limegreen", "linen", "magenta", "maroon", "mediumaquamarine", "mediumblue", "mediumorchid", "mediumpurple", "mediumseagreen", "mediumslateblue", "mediumspringgreen", "mediumturquoise", "mediumvioletred", "midnightblue", "mintcream", "mistyrose", "moccasin", "navajowhite", "navy", "oldlace", "olive", "olivedrab", "orange", "orangered", "orchid", "palegoldenrod", "palegreen", "paleturquoise", "palevioletred", "papayawhip", "peachpuff", "peru", "pink", "plum", "powderblue", "purple", "red", "rosybrown", "royalblue", "saddlebrown", "salmon", "sandybrown", "seagreen", "seashell", "sienna", "silver", "skyblue", "slateblue", "slategray", "slategrey", "snow", "springgreen", "steelblue", "tan", "teal", "thistle", "tomato", "transparent", "turquoise", "violet", "wheat", "white", "whitesmoke", "yellow", "yellowgreen"
```
</details>

## Установка
```sh
git clone https://github.com/DmB0l/3dViewer_on_3dCore.git
mkdir build && cd build
cmake ..
make
```

## License
Project sources distributed under [MIT license](https://github.com/DmB0l/3dViewer_on_3dCore/blob/main/LICENSE), third parties distributed under their own licences
