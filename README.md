# miniRT (work in progress)

> A ray tracer project developed at [42](https://42.fr) by [azolotar](https://github.com/sjapi) and [haaghaja](https://github.com/106c13)

**miniRT** is a ray tracer for the 42 curriculum. It renders basic shapes with lighting using clean C and the **minilibX** library.

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/solar.png" width="800" />
</p>

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/rgb_cone.png" width="800" />
</p>

<p align="center">
  <img src="https://github.com/sjapi/miniRT/blob/main/img/readme_example.png" width="800" />
</p>

---

## Shapes

- Sphere
- Plane
- Cylinder
- Cone
- Custom OBJ (based on \"tribbles\", custom `.obj` format)

## Textures & Materials

- Checkerboard
- Image textures
- Bump mapping
- Mirror (sphere only)

## Features

- Diffuse shading
- Specular highlights
- Multi-sample anti-aliasing
- Bounding boxes
- Skybox

## Modes

- **Render Mode**
- **Camera Mode**
- **Light Mode**
- **Object Mode**

## Controls

- `C` — Enter CAMERA MODE. If already in CAMERA MODE, switch back to RENDER MODE.
  - `W` / `A` / `S` / `D` — You know :)
  - `Q` / `E` — Move top / bottom
  - `H` / `L` — Yaw left / right
  - `J` / `K` — Pitch up / down
  - `+` / `-` — FOV increase / decrease

- `R` — Enter RENDER MODE. If in OBJECT MODE, deselects the selected object.

- To enter OBJECT MODE click on object with mouse
  - `W` — Move along +Y
  - `S` — Move along -Y
  - `A` — Move along +X
  - `Q` — Move along -X
  - `E` — Move along +Z
  - `D` — Move along -Z
  - `X` — Rotate around X axis
  - `Y` — Rotate around Y axis
  - `Z` — Rotate around Z axis

---

## Build & Run

```bash
make
./miniRT scenes/sp.rt
```
