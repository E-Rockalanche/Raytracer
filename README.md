# Raytracer

**CSCI 405 Assignment2**

**By Eric Roberts**

Raytracer to render simple scenes.

Capable of rendering spheres, planes, quads, triangles, and .obj models.

To execute type one of:
```
./assignment2 ./scenes/cornell.scene
./assignment2 ./scenes/planets.scene
./assignment2 ./scenes/samus.scene
```

## Scenes

.scene files contain the information to build a scene

each line should define an attribute or object in the scene

### model

model denotes one of plane, recprism, rectangle, sphere, triangle

models consist of a set of parameters and a material name

#### plane

takes an origin and w and h vectors

ex)
```
# horizontal plane
model plane 0 0 0   1 0 0   0 0 -1  floorMaterial
```

#### rectangle

takes an origin and w and h vectors

the rectangle is bounded by the w and h vectors

ex)
```
# vertical rectangle
model rectangle 0 0 0   1 0 0   0 1 0   wallMaterial
```

#### recprism

takes an origin and w, h, and d vectors

the prism is bounded by the vectors w, h, and d

ex)
```
# cube
model recprism 0 0 0   1 0 0   0 1 0   0 0 1   cubeMaterial
```

#### triangle

takes three vertices

ex)
```
# upside down triangle
model triangle 0 0 0   1 1 0   -1 1 0   triangleMaterial
```

#### sphere

takes an origin and a radius

ex)
```
# sphere with radius = 5
model sphere 0 0 0   5   ballMaterial
```

### polygonmodel

takes a position and a .obj filename

ex)
```
# her name is not metroid
polygonmodel samus.obj
```

### mtllib

specifies a .mtl file to use

ex)
```
# can define transparent and mirror like materials
mtllib materials.mtl
```

### light

adds a light to the scene. Takes a position and colour

ex)
```
# red light at (0, 0, -10)
light 0 0 -10   1 0.5 0.5
```

### camera

sets the camera position and direction. Takes three vectors: camera position, lookat position, up direction

ex)
```
# camera at (0, 5, 10) looking at (0, 0, 0)
camera 0 5 10   0 0 0   0 1 0
```

### attenuation

sets the light attenuation for the scene. light is attenuated by 1/(1 + distance/attenuation)

ex)
```
# appropriate attenuation for scale of scene
attenuation 1000
```

## .obj and .mtl files

see https://en.wikipedia.org/wiki/Wavefront_.obj_file